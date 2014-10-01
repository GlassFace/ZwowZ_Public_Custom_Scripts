/* BattleMaster - MiniGame */
/* Scripts Created By: LebenHarvest */
/* @ ZwowZ.com */
/* Feel free to Use/Redistribute this script and its contents. */
/* Interested in more scripts or development for Trinitycore or other emulators?*/
/* Feel free to contact me. */
/* Skype: leben.harvest */
#include "ScriptPCH.h"
#include "ScriptedCreature.h"

char gladmsg[200];
int32 FightInProgress = 0;

enum Creatures
{
	GLADIATOR_JAINA = 900901,
	GLADIATOR_VARIAN = 900902,
	GLADIATOR_THRALL = 900903,
};

static const Position SpawnPos[1] =
{
	//Add your own location
	{ 0f, 0f, 0f, 0f },//CHANGE ME TO LOCATION YOU WANT ENEMY TO SPAWN
};	//X/ /Y/ /Z/ /O//

class npc_battlemaster : public CreatureScript
{
public:
	npc_battlemaster() : CreatureScript("npc_battlemaster") { }

	bool OnGossipHello(Player *player, Creature *_creature)
	{
		if (FightInProgress == 1 && player->GetSession()->GetSecurity() < 2)//fight in progress
		{
			player->CLOSE_GOSSIP_MENU();
			_creature->MonsterWhisper("An encounter is already in progress! If the event is bugged ask a GM to reset for you.", player->GetGUID());
			return true;
		}
		//gm reset event
		if (player->GetSession()->GetSecurity() >= 1)
		{
			player->ADD_GOSSIP_ITEM(9, "RESET EVENT!", GOSSIP_SENDER_MAIN, 1);
		}
		if (player->isInCombat())
		{
			player->CLOSE_GOSSIP_MENU();
			_creature->MonsterWhisper("You Are In Combat!", player->GetGUID());
			return true;
		}else
		{
			player->ADD_GOSSIP_ITEM(9, "You will face Jaina A Cunning Opponent.", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(9, "You will face Varian A Mighty Opponent.", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(9, "You will face Thrall A Fierce Opponent.", GOSSIP_SENDER_MAIN, 4);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			player->PlayerTalkClass->ClearMenus();
			switch (uiAction)
			{
				case 1://reset
				{
					std::string gmName = player->GetSession()->GetPlayerName();
					sprintf(gladmsg, "Battlemaster reset by %s!", gmName.c_str());
					_creature->MonsterYell(gladmsg, 0, 0);
					FightInProgress = 0;//set fight not in progress
					break;
				}
				case 2://Summon Jaina
				{
					player->CLOSE_GOSSIP_MENU();
					_creature->MonsterYell("The Battle Against Jaina Has Begun!", LANG_UNIVERSAL, 0);
					_creature->SummonCreature(GLADIATOR_JAINA, SpawnPos[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);//One minute out of comabat despawn.
					FightInProgress = 1;//set fight in progress
					break;
				}
				case 3:// Summon Varian
				{
					player->CLOSE_GOSSIP_MENU();
					_creature->MonsterYell("The Battle Against King Varian Has Begun!", LANG_UNIVERSAL, 0);
					_creature->SummonCreature(GLADIATOR_VARIAN, SpawnPos[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
					FightInProgress = 1;//set fight in progress
					break;
				}
				case 4:// Summon Thrall
				{
					player->CLOSE_GOSSIP_MENU();
					_creature->MonsterYell("The Battle Against Thrall Has Begun!", LANG_UNIVERSAL, 0);
					_creature->SummonCreature(GLADIATOR_THRALL, SpawnPos[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
					player->PlayDirectSound(11803, 0);//Optional - L70etc song
					FightInProgress = 1;//set fight in progress
					break;
				}
			}
		}
		return true;
	}

	struct npc_battlemasterAI : public ScriptedAI
	{
		npc_battlemasterAI(Creature* c) : ScriptedAI(c), Summons(me) {}

		SummonList Summons;

		void JustSummoned(Creature* summon)
		{
			if (summon)
			{
				uint32 entry = summon->GetEntry();
				Summons.Summon(summon);
				//
				if (entry == GLADIATOR_JAINA || entry == GLADIATOR_VARIAN || entry == GLADIATOR_THRALL)
				{
					Player* SelNearestPlr = summon->SelectNearestPlayer(30.0f);
					if (SelNearestPlr)
						summon->Attack(SelNearestPlr, false);
					else
					{
						/* cant find a player to select */
					}
				}
			}
		}

		void SummonedCreatureDespawn(Creature* summon)
		{
			uint32 summonID = summon->GetEntry();
			if (summonID == GLADIATOR_JAINA || summonID == GLADIATOR_VARIAN || summonID == GLADIATOR_THRALL)
			{
				sprintf(gladmsg, "Gladiator unsummoned or was killed.!");
				me->MonsterYell(gladmsg, 0, 0);
				FightInProgress = 0;//set fight not in progress
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_battlemasterAI(creature);
	}
};

//Varian spells
#define Spell_One 1//change me
class varian_gladiator : public CreatureScript
{
public:
	varian_gladiator() : CreatureScript("varian_gladiator"){ }

	struct  varian_gladiatorAI : public ScriptedAI
	{
		varian_gladiatorAI(Creature *c) : ScriptedAI(c) {}

		int32 SpellOneTimer;
		void Reset()
		{
			SpellOneTimer = 15000;
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->MonsterSay("You fool, I will slay you with ease!!", LANG_UNIVERSAL, 0);
		}

		void JustDied(Unit* /*killer*/)
		{
			me->MonsterSay("Lucky... Shot..", LANG_UNIVERSAL, 0);
			FightInProgress = 0;
		}

		void JustSummoned(Creature* summon)
		{
			Player* SelNearestPlr = summon->SelectNearestPlayer(60.0f);
			if (SelNearestPlr)
				summon->Attack(SelNearestPlr, false);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (SpellOneTimer <= diff)
			{
				DoCast(me->getVictim(), Spell_One, true);
				me->MonsterSay("CHANGE ME!!!", LANG_UNIVERSAL, 0);
				SpellOneTimer = 15000;
			}else
				SpellOneTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new varian_gladiatorAI(pCreature);
	}
};

//Jaina spells
#define Spell_Two 1//change me
class jaina_gladiator : public CreatureScript
{
public:
	jaina_gladiator() : CreatureScript("jaina_gladiator") { }

	struct jaina_gladiatorAI : public ScriptedAI
	{
		jaina_gladiatorAI(Creature *c) : ScriptedAI(c) {}

		int32 SpellTwoTimer = 15000;//15 seconds by default
		void Reset()
		{//reset timers
			SpellTwoTimer = 15000;
		}

		void EnterCombat(Unit* /*who*/)
		{
			sprintf(gladmsg, "You will regret this action mortal....");
			me->MonsterSay(gladmsg, LANG_UNIVERSAL, 0);
		}

		void JustDied(Unit* /*killer*/)
		{
			sprintf(gladmsg, "You... not possible...");
			me->MonsterSay(gladmsg, LANG_UNIVERSAL, 0);
			me->DespawnOrUnsummon();
			FightInProgress = 0;
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (SpellTwoTimer <= diff)
			{
				me->CastSpell(me, Spell_Two, true);
				sprintf(gladmsg, "CHANGE ME!!!");
				me->MonsterSay(gladmsg, LANG_UNIVERSAL, 0);
				SpellTwoTimer = 15000;
			}else
				SpellTwoTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new jaina_gladiatorAI(pCreature);
	}
};

//Thrall Spells
#define Spell_Three 1//change me//change me
class thrall_gladiator : public CreatureScript
{
public:
	thrall_gladiator() : CreatureScript("thrall_gladiator"){ }

	struct  thrall_gladiatorAI : public ScriptedAI
	{
		thrall_gladiatorAI(Creature *c) : ScriptedAI(c) {}

		int32 SpellThreeTimer;
		void Reset()
		{
			SpellThreeTimer = 15000;
		}

		void EnterCombat(Unit* /*who*/)

		{
			me->MonsterSay("You challenge The Mighty Thrall mortal!", LANG_UNIVERSAL, 0);
		}

		void JustDied(Unit* /*killer*/)
		{
			me->MonsterSay("But.. How...", LANG_UNIVERSAL, 0);
			FightInProgress = 0;
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (SpellThreeTimer <= diff)
			{
				me->CastSpell(me, Spell_Three, true);
				sprintf(gladmsg, "CHANGE ME!!!");
				me->MonsterSay(gladmsg, LANG_UNIVERSAL, 0);
				SpellThreeTimer = 15000;
			}else
				SpellThreeTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new thrall_gladiatorAI(pCreature);
	}
};

void AddSC_npc_battlemaster()
{
	//ZWOWZ
	new thrall_gladiator();
	new varian_gladiator();
	new jaina_gladiator();
	new npc_battlemaster();
}
