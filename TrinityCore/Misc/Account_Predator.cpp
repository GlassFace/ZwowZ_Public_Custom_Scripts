/* Account Predator */
/* Created By LevenHarvest */
/* @ https://zwowz.com */
/* Skype: leben.harvest */
/* Feel free to modify/redistribute this script and its contents */
/* Command Usage: .predator warn "target", .predator toggle "target", .predator comment "comment about selected player" */
#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Language.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "AccountMgr.h"
int32 MinWarningsBeforeBan = 3;//3 warnings before character gets a ban
int32 MaxWarningsBeforeBan = 7;//ban @ 7 warnings.
std::string DayBan = "86400000";//1 day ban / Miliseconds
std::string WeekBan = "604800000";//7 day ban
int32 Accid = 0;

class Predator : public PlayerScript
{
public:
	Predator() : PlayerScript("Predator") {}

	void OnLogin(Player* player)
	{
		QueryResult result;
		Accid = player->GetSession()->GetAccountId();
		int32 predator = 0;
		int32 Warnings = 0;
		char charmsg[200];
		std::string Comment = "NULL";
		result = LoginDatabase.PQuery("SELECT `Id`, `CharacterName`, `Predator`, `Warnings`, `Comment` FROM `Account_Predator` WHERE `Id` = '%u'", Accid);
		if (result)
		{
			do
			{
				Field *fields = result->Fetch();
				Accid = fields[0].GetInt32();
				std::string chrName = fields[1].GetString();
				predator = fields[2].GetInt32();
				Warnings = fields[3].GetInt32();
				Comment = fields[4].GetString();

				if (Accid == player->GetSession()->GetAccountId() && predator == 1)
				{
					sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, Has logged in. This player is under surveilence! Warnings: %u, Comment: %s|r", Accid, player->GetName(), Warnings, Comment.c_str());
					sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
				}
			} while (result->NextRow());//loop
		}
	}

	void OnLogout(Player* player)
	{
		QueryResult result;
		Accid = player->GetSession()->GetAccountId();
		int32 predator = 0;
		int32 Warnings = 0;
		char charmsg[200];
		std::string Comment = "NULL";
		result = LoginDatabase.PQuery("SELECT `Id`, `CharacterName`, `Predator`, `Warnings`, `Comment` FROM `Account_Predator` WHERE `Id` = '%u'", Accid);
		if (result)
		{
		do {
				Field* fields = result->Fetch();
				int32 Id = fields[0].GetInt32();
				std::string chrName = fields[1].GetString();
				int32 Predator = fields[2].GetInt32();
				int32 Warnings = fields[3].GetInt32();

				if (Predator == 1 && Warnings >= 1)
				{
					sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, Has logged out.|r", Accid, chrName.c_str());
					sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
				}
			} while (result->NextRow());
		}
	}
};

class Predator_Commands : public CommandScript
{
public:
	Predator_Commands() : CommandScript("Predator_Commands") { }

	ChatCommand* GetCommands() const override
	{
		static ChatCommand CommandTable[] =
		{
			{ "warn", SEC_GAMEMASTER, false, &HandleWarnCommand, "", NULL },
			{ "comment", SEC_GAMEMASTER, true, &HandleCommentCommand, "", NULL },
			{ "toggle", SEC_GAMEMASTER, false, &HandleTogglePredatorCommand, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};

		static ChatCommand commandTable[] =
		{
			{ "predator", SEC_GAMEMASTER, false, NULL, "", CommandTable },
			{ NULL, 0, false, NULL, "", NULL }
		};
		return commandTable;
	}

	static bool HandleWarnCommand(ChatHandler* handler, const char* args)
	{
		Player* target;
		uint64 targetGuid;
		std::string targetName;
		std::string gmName = handler->GetSession()->GetPlayerName();
		std::string Comment = "NULL";

		uint32 parseGUID = MAKE_NEW_GUID(atol((char*)args), 0, HIGHGUID_PLAYER);

		if (sObjectMgr->GetPlayerNameByGUID(parseGUID, targetName))
		{
			target = sObjectMgr->GetPlayerByLowGUID(parseGUID);
			targetGuid = parseGUID;
		}
		else if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
			return false;

		if (!target)
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (target)
		{
			QueryResult result = LoginDatabase.PQuery("SELECT `Id`, `CharacterName`, `Predator`, `Warnings`, `Comment` FROM `Account_Predator` WHERE `Id`='%u'", target->GetSession()->GetAccountId());
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					Accid = fields[0].GetInt32();
					std::string chrName = fields[1].GetString();
					int32 Predator = fields[2].GetInt32();
					int32 Warnings = fields[3].GetInt32();
					Comment = fields[4].GetString();
					std::string plrban;
					//ban
					std::string banString;
					banString = "Predator Auto-Ban - Exceeded warning limit!";
					//
					char charmsg[200];

					if (Warnings >= MaxWarningsBeforeBan)//7+ strikes //1 week account ban
					{
						sWorld->BanAccount(BAN_CHARACTER, target->GetName(), WeekBan, banString, "Predator Auto-Ban");
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, has been banned by the PREDATOR System! Too many warnings! Warnings:%u, Last Comment: %s|r", Accid, chrName.c_str(), Warnings, Comment.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
						return false;
					}
					if (Warnings >= MinWarningsBeforeBan)//3+ strikes //1 day account ban
					{
						sWorld->BanAccount(BAN_CHARACTER, target->GetName(), DayBan, banString, "Predator Auto-Ban");
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, has been banned by the PREDATOR System! Too many warnings! Warnings:%u, Last Comment: %s|r", Accid, chrName.c_str(), Warnings, Comment.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
						return false;
					}
					//
					if (Predator == 0)
					{
						LoginDatabase.PExecute("UPDATE Account_Predator SET Predator=1, Warnings=Warnings+1 WHERE Id='%u'", target->GetSession()->GetAccountId());
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, has been flagged by %s for the PREDATOR System!|r", chrName.c_str(), target->GetName(), gmName.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
					}else
					{
						LoginDatabase.PExecute("UPDATE Account_Predator SET Warnings=Warnings+1 WHERE Id='%u'", target->GetSession()->GetAccountId());
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, The Warnings of this account has been increased by 1 by %s!|r", target->GetSession()->GetAccountId(), target->GetName(), gmName.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
					}
				} while (result->NextRow());
			}else
			{//create initial Predator entry for warning.
				char charmsg[200];
				LoginDatabase.PExecute("INSERT INTO Account_Predator VALUES('%u', '%s', '%u', '%u','%s')", target->GetSession()->GetAccountId(), target->GetName(), 1, 1, Comment.c_str());
				sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, Predator has been enabled for this account by %s!|r", target->GetSession()->GetAccountId(), target->GetName(), gmName.c_str());
				sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
			}
		}
		return true;
	}

	static bool HandleCommentCommand(ChatHandler* handler, const char* args)
	{
		if (!*args)
			return false;
		Player* target = handler->getSelectedPlayer();
		std::string gmName = handler->GetSession()->GetPlayerName();

		if (!target)
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (target)
		{
			QueryResult result = LoginDatabase.PQuery("SELECT `Id` FROM `Account_Predator` WHERE `Id`='%u'", target->GetSession()->GetAccountId());
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					int32 Id = fields[0].GetInt32();

					char* comString = handler->extractQuotedArg((char*)args);
					if (!comString)
					return false;

					std::string comment = comString;
					char charmsg[200];
					LoginDatabase.PExecute("UPDATE Account_Predator SET Comment='%s' WHERE Id='%u'", comment.c_str(), target->GetSession()->GetAccountId());
					sprintf(charmsg, "|cff9cff00[PREDATOR] Predator comment updated for Account: %u, Character: %s by %s! Comment: %s|r", target->GetSession()->GetAccountId(), target->GetName(), gmName.c_str(), comString);
					sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
				} while (result->NextRow());
			}
		}
		return true;
	}

	static bool HandleTogglePredatorCommand(ChatHandler* handler, const char* args)
	{
		Player* target;
		uint64 targetGuid;
		std::string targetName;
		std::string gmName = handler->GetSession()->GetPlayerName();
		std::string Comment = "NULL";

		uint32 parseGUID = MAKE_NEW_GUID(atol((char*)args), 0, HIGHGUID_PLAYER);

		if (sObjectMgr->GetPlayerNameByGUID(parseGUID, targetName))
		{
			target = sObjectMgr->GetPlayerByLowGUID(parseGUID);
			targetGuid = parseGUID;
		}
		else if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
			return false;

		if (!target)
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
			handler->SetSentErrorMessage(true);
			return false;
		}

		//check player exist in table
		if (Accid == target->GetSession()->GetAccountId())
		{

		}
		else
		{//create initial Predator entry for warning.
			LoginDatabase.PExecute("REPLACE INTO Account_Predator VALUES('%u', '%s', '%u', '%u','%s')", target->GetSession()->GetAccountId(), target->GetName(), 0, 0, Comment.c_str());
		}

		if (target)
		{
			QueryResult result = LoginDatabase.PQuery("SELECT `Id`, `CharacterName`, `Predator` FROM Account_Predator WHERE `Id`='%u'", target->GetSession()->GetAccountId());
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					int32 Id = fields[0].GetInt32();
					std::string chrName = fields[1].GetString();
					int32 Predator = fields[2].GetInt32();
					char* comString = handler->extractQuotedArg((char*)args);
					if (comString)
					return false;
					char charmsg[200];

					if (Predator == 1)
					{
						LoginDatabase.PExecute("UPDATE Account_Predator SET Predator=0 WHERE Id='%u'", target->GetSession()->GetAccountId());
						sprintf(charmsg, "|cff9cff00[PREDATOR] Predator turned off for Account: %u, Character: %s by %s!|r", target->GetSession()->GetAccountId(), chrName.c_str(), gmName.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
					}else
					{
						LoginDatabase.PExecute("UPDATE Account_Predator SET Predator=1 WHERE Id='%u'", target->GetSession()->GetAccountId());
						sprintf(charmsg, "|cff9cff00[PREDATOR] Predator turned on for Account: %u, Character: %s by %s!|r", target->GetSession()->GetAccountId(), target->GetName(), gmName.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
					}
				} while (result->NextRow());
			}
		}
		return true;
	}
};

void AddSC_Account_Predator()
{
	new Predator();
	new Predator_Commands();
}