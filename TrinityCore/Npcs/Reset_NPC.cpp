/* Reset NPC */
/* Created By LevenHarvest */
/* @ https://zwowz.com */
/* Skype: leben.harvest */
/* Feel free to modify/redistribute this script and its contents */
#include "ScriptPCH.h"
#include "Player.h"
#define GOSSIP_ITEM_1 "Reset Talents"

class npc_reset : public CreatureScript
{
public: npc_reset() : CreatureScript("npc_reset"){}
		struct npc_resetAI : public ScriptedAI
		{
			//Creature Constructor
			npc_resetAI(Creature *c) : ScriptedAI(c) {}
		};

		CreatureAI* GetAI(Creature* pCreature) const
		{
			return new npc_resetAI(pCreature);
		}

		bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
		{
			ShowMenuItems(pPlayer, pCreature, uiAction);
			return true;
		}

		void ShowMenuItems(Player* pPlayer, Creature* pCreature, uint32 uiAction)
		{
			pPlayer->PlayerTalkClass->ClearMenus(); //CLEAR Previous Menu
			//MAIN MENU
			pPlayer->ADD_GOSSIP_ITEM(4, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1); //Talent reset gossip
			if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) //Reset Talents     
			{
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->ResetTalents(true);
				pPlayer->SendTalentsInfoData();
				char msg[200];
				sprintf(msg, "|cff9cff00[Reset NPC]: Talents Have been reset.");
				ChatHandler(pPlayer->GetSession()).SendSysMessage(msg); //tell player
			}
		}
};

void AddSC_npc_reset()
{
	new npc_reset();
}
