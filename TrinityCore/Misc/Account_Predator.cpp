/* Reset NPC */
/* Created By LevenHarvest */
/* @ https://zwowz.com */
/* Skype: leben.harvest */
/* Feel free to modify/redistribute this script and its contents */
/* Command Usage: .predator warn "target", .predator toggle "target", .predator comment ""*/
#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Language.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "AccountMgr.h"
int32 MinWarningsBeforeBan = 3;//3 warnings before character gets a ban
int32 MaxWarningsBeforeBan = 7;//Perm ban @ 7 warnings.
std::string DayBan = "86400000";//1 day ban / Miliseconds
std::string WeekBan = "604800000";//7 day ban
char charmsg[200];

class Predator : public PlayerScript
{
public:
	Predator() : PlayerScript("Predator") {}

	void OnLogin(Player* player)
	{
		QueryResult result = LoginDatabase.PQuery("SELECT Id, CharacterName, Predator, Warnings, Comment FROM Account_Predator WHERE Id='%u'", player->GetSession()->GetAccountId());
	do {
			Field* fields = result->Fetch();
			int32 Id = fields[0].GetInt32();
			std::string Name = fields[1].GetString();
			int32 Predator = fields[2].GetInt32();
			int32 Warnings = fields[3].GetInt32();
			std::string Comment = fields[4].GetString();

			if (Predator == 1 && Warnings >= 1)
			{
				sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, Has logged in. This player is under surveilence! This player has %u warnings. Most recent comment: %s|r", Id, player->GetName(), Warnings, Comment.c_str());
				sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
			}
		} while (result->NextRow());
		//Check if player already exist in predator table
		if (player)
		{
			int32 Id = player->GetSession()->GetAccountId();
			std::string Name = player->GetName();
			std::string Comment = "NULL";
			//
			QueryResult result = LoginDatabase.PQuery("SELECT Id FROM Account_Predator WHERE Id='%u'", Id);
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					int32 Id = fields[0].GetInt32();
				} while (result->NextRow());
			}else
			{//Create initial predator entry.
				LoginDatabase.PExecute("INSERT INTO Account_Predator(Id, CharacterName, Predator, Warnings, Comment) VALUES ('%u', '%s', '%u', '%u', '%s')", Id, Name.c_str(), 0, 0, Comment.c_str());
			}
		}
	}

	void OnLogout(Player* player)
	{
		QueryResult result3 = LoginDatabase.PQuery("SELECT Id, CharacterName, Predator, Warnings, Comment FROM Account_Predator WHERE Id='%u'", player->GetSession()->GetAccountId());
	do {
			Field* fields = result3->Fetch();
			int32 Id = fields[0].GetInt32();
			std::string Name = fields[1].GetString();
			int32 Predator = fields[2].GetInt32();
			int32 Warnings = fields[3].GetInt32();

			if (Predator == 1 && Warnings >= 1)
			{
				sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, Has logged out.|r", Id, player->GetName());
				sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
			}
		} while (result3->NextRow());
	}
};

class Predator_Commands : public CommandScript
{
public:
	Predator_Commands() : CommandScript("Predator_Commands") { }

	ChatCommand* GetCommands() const override
	{
		static ChatCommand gdCommandTable[] =
		{
			{ "warn", SEC_GAMEMASTER, false, &HandleWarnCommand, "", NULL },
			{ "comment", SEC_GAMEMASTER, true, &HandleCommentCommand, "", NULL },
			{ "toggle", SEC_GAMEMASTER, false, &HandleTogglePredatorCommand, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};

		static ChatCommand commandTable[] =
		{
			{ "predator", SEC_GAMEMASTER, false, NULL, "", gdCommandTable },
			{ NULL, 0, false, NULL, "", NULL }
		};
		return commandTable;
	}

	static bool HandleWarnCommand(ChatHandler* handler, const char* args)
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
			QueryResult result = LoginDatabase.PQuery("SELECT Id, CharacterName, Predator, Warnings, Comment FROM Account_Predator WHERE Id='%u'", target->GetSession()->GetAccountId());
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					int32 Id = fields[0].GetInt32();
					std::string chrName = fields[1].GetString();
					int32 Predator = fields[2].GetInt32();
					int32 Warnings = fields[3].GetInt32();
					std::string comment = fields[4].GetString();
					std::string plrban;
					//ban
					std::string banString;
					banString = "Predator Auro-Ban - Exceeded warning limit!";

					if (Warnings >= MaxWarningsBeforeBan)//7+ strikes //1 week account ban
					{
						sWorld->BanAccount(BAN_CHARACTER, target->GetName(), WeekBan, banString, "Predator Auto-Ban");
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, has been banned by the PREDATOR System! Too many warnings! Warnings:%u, Last Comment: %s|r", Id, chrName.c_str(), Warnings, comment.c_str());
						sWorld->SendGMText(LANG_GM_BROADCAST, charmsg);
						return false;
					}
					if (Warnings >= MinWarningsBeforeBan)//3+ strikes //1 day account ban
					{
						sWorld->BanAccount(BAN_CHARACTER, target->GetName(), DayBan, banString, "Predator Auto-Ban");
						sprintf(charmsg, "|cff9cff00[PREDATOR] Account: %u, Character: %s, has been banned by the PREDATOR System! Too many warnings! Warnings:%u, Last Comment: %s|r", Id, chrName.c_str(), Warnings, comment.c_str());
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
			QueryResult result = LoginDatabase.PQuery("SELECT Id FROM Account_Predator WHERE Id='%u'", target->GetSession()->GetAccountId());
			if (result)
			{
			do {
					Field* fields = result->Fetch();
					int32 Id = fields[0].GetInt32();

					char* comString = handler->extractQuotedArg((char*)args);
					if (!comString)
						return false;

					std::string comment = comString;
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
			QueryResult result = LoginDatabase.PQuery("SELECT CharacterName, Predator FROM Account_Predator WHERE Id='%u'", target->GetSession()->GetAccountId());
			if (!result)
			{
			do {
					Field* fields = result->Fetch();
					std::string uName = fields[0].GetString();
					int32 Predator = fields[1].GetInt32();
					char* comString = handler->extractQuotedArg((char*)args);
					if (!comString)
						return false;

					std::string comment = comString;

					if (Predator == 1)
					{
						LoginDatabase.PExecute("UPDATE Account_Predator SET Predator=0 WHERE Id='%u'", target->GetSession()->GetAccountId());
						sprintf(charmsg, "|cff9cff00[PREDATOR] Predator turned off for Account: %u, Character: %s by %s!|r", target->GetSession()->GetAccountId(), target->GetName(), gmName.c_str());
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