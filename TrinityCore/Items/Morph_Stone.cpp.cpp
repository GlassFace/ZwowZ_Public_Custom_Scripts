/* Morph stone */
/* Created By LevenHarvest */
/* @ https://zwowz.com */
/* Skype: leben.harvest */
/* Feel free to modify/redistribute this script and its contents */
class Morph_Stone : public ItemScript
{
public:
	Morph_Stone() : ItemScript("Morph_Stone") { }
	char chrmsg[200];

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
	{
		player->PlayerTalkClass->ClearMenus();
		//fun stuff
		player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff1EFF0C Morph|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "", 0, true);
		player->ADD_GOSSIP_ITEM(0, "|cff1EFF0C Demorph|r", GOSSIP_SENDER_MAIN, 2);
		player->PlayerTalkClass->SendGossipMenu(907, item->GetGUID());
		return true;
	}

	void OnGossipSelect(Player* player, Item* item, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
			//demorph
		case 2:
		{
			player->DeMorph();
			int32 DisplayId = player->GetDisplayId();
			//tell player
			sprintf(chrmsg, "|cff9cff00[Morph Stone] Default display Id restored to: %u.|r", DisplayId);
			ChatHandler(player->GetSession()).SendSysMessage(chrmsg);
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		}
	}

	void OnGossipSelectCode(Player* player, Item* item, uint32 sender, uint32 action, const char* code)
	{
		player->PlayerTalkClass->ClearMenus();
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (action)
			{
			case GOSSIP_ACTION_INFO_DEF + 1:
			{
				if (code > 0)
				{
					//convert const char to int
					uint32 CodeInt = atoi((char*)code);

					if (CodeInt< 99999 && CodeInt> 1)//X less than max # && Greater than 1, Morph Player
					{
						//set display id
						player->SetDisplayId(CodeInt);
						//tell player
						sprintf(chrmsg, "|cff9cff00[Morph Stone] Display Id set to %u.|r", CodeInt);
						ChatHandler(player->GetSession()).SendSysMessage(chrmsg);
					}
					else
					{//Invalid Display ID
						//tell player
						sprintf(chrmsg, "|cff9cff00[Morph Stone] Not a valid display Id.|r");
						ChatHandler(player->GetSession()).SendSysMessage(chrmsg);
					}
				}
				player->CLOSE_GOSSIP_MENU();
				break;
			}
			}
		}
	}
};

//void AddSC_Morph_Stone()
{
	new Morph_Stone();
}