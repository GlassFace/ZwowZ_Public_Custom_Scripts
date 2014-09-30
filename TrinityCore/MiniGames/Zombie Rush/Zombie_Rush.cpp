/* ZOMBIE RUSH MINI-GAME */
/*Scripts Created By: LebenHarvest */
/* @ ZwowZ.com */
/*Feel free to Use/Redistribute this script and its contents. */
/*Interested in more scripts or development for Trinitycore or other emulators?*/
/*Feel free to contact me. */
/*Skype: leben.harvest */
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "Group.h"

//ZOMBIE GAME
int8 GroupMembersDead = 0;
//current number of zombies spawned so far
int8 zombiesSpawned = 0;
//what round in the game you are on
int8 zombieRound = 0;
//how many waves of zombies per round
int8 zombieWaves = 0;
//the number of waves already spawned
int8 zombieWavesCount = 0;
//the number of zombies to be spawned per wave
int8 zombieSpawnPerWave = 0;
//stores how many players are in the group
int8 zombieGroupMembers = 0;
//stores number of zombies spawn per each person
int8 ZOMBIES_PER_PERSON = 3;
int8 MaxRounds = 5;
int8 inProgress = 0;
bool inZombieGame;
bool zombie_inUse = false;
char chrmsg[200];
Player* ZGLeader;
Group* playerGroup;
//README
/* SET THE MAP, X, Y, Z LOCATIONS, CREATE CREATURES AND SET SCRIPT NAMES. CREATE GAME OBJECT FOR CHEST(s) */

//chest gps loc
float CHEST_X = -466.13f;
float CHEST_Y = 94.94f;
float CHEST_Z = -94.95f;
float CHEST_O = 4.4f;
//creatures
WorldLocation POS_MINIGAME_START(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_START_GAME(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_NEXT_WAVE(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_END_GAME(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_END_ROUND(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_WAIT_FOR_LOOT(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_NPC_RESET(109, -466.13f, 95.94f, -94.95f, 4.4f);
//teles
WorldLocation POS_TELE_MALL(109, -466.13f, 95.94f, -94.95f, 4.4f);
WorldLocation POS_TELE_REVIVE(109, -466.13f, 95.94f, -94.95f, 4.4f);

//enemy spawn loc
WorldLocation POS_ENEMY_SPAWN(109, -466.13f, 95.94f, -94.95f, 4.4f);

enum zombie_minigame_DATA
{
	//ZOMBIE GAME NPC IDS
	MG_NPC_START_GAME = 500800,
	MG_NPC_START_NEXT_WAVE = 500801,
	MG_NPC_START_NEXT_ROUND = 500802,
	MG_NPC_WAIT_FOR_END_ROUND = 500803,
	MG_NPC_WAIT_FOR_CHEST_LOOT = 500804,
	MG_NPC_END_GAME = 500805,
	//ENEMIES
	ZOMBIE_ENEMY = 500806,
	//CHEST GAME OBJECT ID
	CHEST = 12,
};

bool closeGossipNotify(Player *player, std::string message)
{
	player->PlayerTalkClass->SendCloseGossip();
	player->GetSession()->SendNotification(message.c_str());
	return true;
}

class zombie_minigame_master : public CreatureScript
{
public:
	zombie_minigame_master() : CreatureScript("zombie_minigame_master") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->isInCombat())
		{
			return closeGossipNotify(player, "You can't start the zombie MiniGame while being in combat.");
		}

		if (!player->GetGroup() || player->GetGroup() == NULL)
		{
			return closeGossipNotify(player, "You must be the group leader in a party/raid with 5 or at least 2 players");
		}

		Group* grp = player->GetGroup();

		if (grp->GetMembersCount() > 5 || grp->GetMembersCount() < 2)
		{
			return closeGossipNotify(player, "You must be the group leader in a party/raid with 5 or at least 2 players");
		}

		if (grp->GetLeaderGUID() != player->GetGUID())
		{
			return closeGossipNotify(player, "Only the party leader can activate the Zombie MiniGame!");
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Teleport me to the Zombie Rush MiniGame!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "No thanks.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		Group* grp = player->GetGroup();
		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
		{
			if (inZombieGame == true)
			{
				return closeGossipNotify(player, "A game is currently in progress!");
			}
			inZombieGame = true;//game is true new groups can not join until MiniGame is over!
			grp->ConvertToRaid();
			player->TeleportTo(POS_MINIGAME_START);
			zombie_inUse = true;
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 2:
		{
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		}
		return true;
	}
};

class zombie_minigame_ender : public CreatureScript
{
public:
	zombie_minigame_ender() : CreatureScript("zombie_minigame_ender") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (!player)
			return false;
		if (!player->GetGroup())
			return false;
		if (player->GetGroup() == NULL)
			return false;
		if (!creature)
			return false;

		if (player->GetGroup()->GetLeaderGUID() == player->GetGUID())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "End the Zombie MiniGame!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		}
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		Group* grp = player->GetGroup();
		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
		{
			creature->MonsterYell("The group leader ended the zombie rush minigame!", 0, 0);
			inProgress = 1;
			creature->SummonCreature(MG_NPC_END_GAME, POS_NPC_END_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1 * 1000);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 2:
		{
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		}
		return true;
	}
};

class zombie_minigame_controller : public CreatureScript
{
public:
	zombie_minigame_controller() : CreatureScript("zombie_minigame_controller") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (!player->GetGroup())
			return false;
		if (player->GetGroup() == NULL)
			return false;

		if ((player->GetGroup()->GetMembersCount() <= 5 && player->GetGroup()->GetMembersCount() >= 2) && player->GetGroup()->GetLeaderGUID() == player->GetGUID())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Summon group to the Zombie Rush MiniGame!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			if (player->GetGroup()->GetLeaderGUID() == player->GetGUID())
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Start the Game!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		}
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{

		player->PlayerTalkClass->ClearMenus();
		Group* grp = player->GetGroup();
		zombie_inUse = true;
		if (grp){
			GroupReference* grpRef = grp->GetFirstMember();
			if (grp->GetLeaderGUID() == player->GetGUID())
				GroupReference grpRef = player->GetGroupRef();

			switch (uiAction)
			{
			case GOSSIP_ACTION_INFO_DEF + 1:
			{
				for (grpRef; grpRef != NULL; grpRef = grpRef->next())
				{
					Player* groupMember = grpRef->getSource();
					ChatHandler(player->GetSession()).PSendSysMessage("Sending summon request to...");
					if (!groupMember)
						continue;
					if (groupMember->GetGUID() == player->GetGUID())
						break;
					player->SetSelection(groupMember->GetGUID());
					player->CastSpell(groupMember, 7720, true);
					ChatHandler(player->GetSession()).PSendSysMessage("%s", groupMember->GetName());
				}
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 2:
			{
				creature->MonsterYell("Round 1 starts in 30 seconds!", 0, 0);
				/* spawn MG_NPC_START_GAME to despawn after 30 seconds and start the game */
				creature->SummonCreature(MG_NPC_START_GAME, POS_NPC_START_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1 * 1000);
				/* the main NPC will not be seen again by the group so teleport him away */
				creature->UpdatePosition(POS_NPC_RESET, true);
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 3:
			{
				creature->MonsterYell("The group leader ended the zombie rush minigame!", 0, 0);
				creature->SummonCreature(MG_NPC_END_GAME, POS_NPC_END_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500);
				break;
			}
			}
		}
		player->PlayerTalkClass->SendCloseGossip();
		return true;
	}

	struct zombie_minigame_controllerAI : public ScriptedAI
	{
		zombie_minigame_controllerAI(Creature* c) : ScriptedAI(c), Summons(me) {}

		SummonList Summons;

		void JustSummoned(Creature* summon)
		{
			if (summon)
			{
				uint32 entry = summon->GetEntry();
				Summons.Summon(summon);
				//
				if (entry >= MG_NPC_START_GAME && entry <= MG_NPC_WAIT_FOR_END_ROUND || entry == MG_NPC_WAIT_FOR_CHEST_LOOT || entry == MG_NPC_END_GAME)
				{

				}
				else
				{
					Player* SelNearestPlr = summon->SelectNearestPlayer(240.0f);
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
			if (summonID == MG_NPC_START_GAME)
			{
				inProgress = 0;
				if (inProgress == 1)
				{
					char msg[200];
					sprintf(msg, "Game Ended or not in progress! Resetting Game!");
					me->MonsterYell(msg, 0, 0);
					ResetZombieGame();
					return;
				}
				/*New Game, reset everything. */
				ResetZombieGame();
				inProgress = 0;
				me->SetVisible(false);
				Player* SelNearestPlr = me->SelectNearestPlayer(240.0f);
				if (!SelNearestPlr)
				{
					ResetZombieGame();
					me->MonsterYell("No Player!", 0, 0); return;
				}
				if (SelNearestPlr == NULL)
				{
					ResetZombieGame();
					me->MonsterYell("No Player!", 0, 0); return;
				}
				if (!SelNearestPlr->GetGroup())
				{
					ResetZombieGame();
					me->MonsterYell("No Group", 0, 0); return;
				}
				if (SelNearestPlr->GetGroup() == NULL)
				{
					ResetZombieGame();
					me->MonsterYell("No Group!", 0, 0); return;
				}
				if (playerGroup = SelNearestPlr->GetGroup())
				{
					zombieGroupMembers = playerGroup->GetMembersCount();
					if (ZGLeader = sObjectAccessor->FindPlayer(playerGroup->GetLeaderGUID()))
					{
					}
					else
					{
						ResetZombieGame();
						me->MonsterYell("No Group!", 0, 0); return;
					}
				}
				else
				{
					ResetZombieGame();
					me->MonsterYell("No Group!", 0, 0); return;
				}

				/* Set the ZombieGame to true so they cant keep starting the game */
				inZombieGame = true;

				//me->MonsterYell("Round 1 is starting now!", 0, 0);
				//sendMessageToGroup("Round 1 is starting now!");
				me->MonsterYell("If you die anytime during the MiniGame you will be respawned at the end of the round! If the entire party dies you will be teleported to the mall.", 0, 0);
				/* NPC_START_NEXT_WAVE is despanwed when the next wave of zombies is to be sent */
				me->SummonCreature(MG_NPC_START_NEXT_WAVE, POS_NPC_NEXT_WAVE, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30 * 1000);
			}
			//No one in game
			if (inZombieGame == false)
			{
				ResetZombieGame();
				return;
			}
			//Game not in progress or Ended, reset.
			if (inProgress == 1)
			{
				me->MonsterYell("Game Ended/Not in Progress, reset!", 0, 0);
				ResetZombieGame();
				return;
			}
			//No Group, End game, reset.
			if (!ZGLeader->GetGroup())
			{
				me->MonsterYell("No Group Found!", 0, 0);
				ResetZombieGame();
				return;
			}
			if (!ZGLeader)
			{
				me->MonsterYell("No Group Leader Found!", 0, 0);
				ResetZombieGame();
				return;
			}

			switch (summonID)
			{
			case MG_NPC_WAIT_FOR_END_ROUND:
			{
				//End MiniGame
				if (inProgress == 1)
				{
					sprintf(chrmsg, "Game ended or not in progress! Resetting game!");
					me->MonsterYell(chrmsg, 0, 0);
					ResetZombieGame();
					return;
				}
				//revive group & reset if group failed game.
				reviveGroup();
				//All Enemy NPCS killed, Continue Game.
				if (zombiesSpawned < 1)
				{
					sprintf(chrmsg, "All enemies killed continuing game!");
					me->MonsterYell(chrmsg, 0, 0);
					zombiesSpawned = 0;
					/* NPC_START_NEXT_WAVE is despanwed when the next wave of zombies is to be sent */
					me->SummonCreature(MG_NPC_START_NEXT_WAVE, POS_NPC_NEXT_WAVE, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1 * 1000);
				}
				else
				{
					sprintf(chrmsg, "You must kill all enemies to continue the game!");
					me->MonsterYell(chrmsg, 0, 0);
					me->SummonCreature(MG_NPC_WAIT_FOR_END_ROUND, POS_NPC_END_ROUND, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30 * 1000);
					break;
				}
				break;
			}
				/* called when a wave of zombies needs to be spawned */
			case MG_NPC_START_NEXT_WAVE:
			{
				me->SetVisible(false);//Set npc to invisible /* Players may only summon new/existing group memebers during the end of a round.*/
				if (inProgress == 1)
				{
					sprintf(chrmsg, "Game Ended or not in progress! Resetting game!");
					me->MonsterYell(chrmsg, 0, 0);
					ResetZombieGame();
					return;
				}
				//check if zombies are still alive, if so spawn wait for end of round npc.
				if (zombiesSpawned > 0)
				{
					sprintf(chrmsg, "Waiting for all creatures to be slain to resume the game!");
					me->MonsterYell(chrmsg, 0, 0);
					me->SummonCreature(MG_NPC_WAIT_FOR_END_ROUND, POS_NPC_END_ROUND, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30 * 1000);
					return;
				}
				// wave check
				if (zombieWavesCount <= getNumberOfWaves())
				{
					//spawn
					sprintf(chrmsg, "Wave %u has spawned!", zombieWavesCount);
					me->MonsterYell(chrmsg, 0, 0);
					sendMessageToGroup(chrmsg);
					/* calc how many waves of zombies to send */
					zombieWaves = getNumberOfWaves();
					/* calc how many zombies per wave */
					zombieSpawnPerWave = getZombieSpawnCountPerWave();
					//
					spawnZombies(me, zombieSpawnPerWave);
					zombieWavesCount += 1;
					//
					me->SummonCreature(MG_NPC_START_NEXT_WAVE, POS_NPC_NEXT_WAVE, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20 * 1000);
				}
				else
				{
					//spawn
					sprintf(chrmsg, "Round %u Chest has spawned! You have 60 seconds to loot & prepare.", zombieRound);
					//round cleared +1
					zombieRound += 1;
					me->MonsterYell(chrmsg, 0, 0);
					sendMessageToGroup(chrmsg);
					if (zombieRound <= 5)
					{
						me->SummonCreature(MG_NPC_WAIT_FOR_CHEST_LOOT, POS_NPC_WAIT_FOR_LOOT, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30 * 1000);
						me->SummonGameObject(CHEST, CHEST_X, CHEST_Y, CHEST_Z, CHEST_O, TEMPSUMMON_TIMED_DESPAWN, 60 * 1000, 0, 0, 0);
					}
					//revice group & check if group failed game.
					reviveGroup();
					//resurrect dead players
				}
				break;
			}
			case MG_NPC_WAIT_FOR_CHEST_LOOT:
			{
				if (inProgress == 1)
				{//End Game
					char msg[200];
					sprintf(msg, "Game Ended or not in progress! Resetting MiniGame!");
					me->MonsterYell(msg, 0, 0);
					ResetZombieGame();
					return;
				}
				if (zombieRound <= MaxRounds)
				{//Continue Game
					sprintf(chrmsg, "Round %u is starting!", zombieRound);
					me->MonsterYell(chrmsg, 0, 0);
					sendMessageToGroup(chrmsg);
					me->SetVisible(false);/* Disallow players from summoning while a round is in progress. */
					zombieWaves = getNumberOfWaves();						/* calc how many waves of zombies to send */
					zombieSpawnPerWave = getZombieSpawnCountPerWave();		/* calc how many zombies per wave */
					zombieWavesCount = 1;//reset current wave
					sprintf(chrmsg, "30 Seconds left to loot chests & prepare! The next wave starts in 30 seconds!");
					me->MonsterYell(chrmsg, 0, 0);
					sendMessageToGroup(chrmsg);
					me->SummonCreature(MG_NPC_START_NEXT_WAVE, POS_NPC_START_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30 * 1000);
				}
				else{
					me->SummonCreature(MG_NPC_END_GAME, POS_NPC_END_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3 * 1000);
				}
				break;
			}
			case MG_NPC_END_GAME:
			{
				sprintf(chrmsg, "The Game Has Ended.");
				me->MonsterYell(chrmsg, 0, 0);
				sendMessageToGroup(chrmsg);
				inProgress = 0;//0 - Inactive
				ResetZombieGame();
				me->SetVisible(true);
				break;
			}
			default:
				if (summonID == ZOMBIE_ENEMY)
					sprintf(chrmsg, "Zombie Killed.");
				me->MonsterYell(chrmsg, 0, 0);
				sendMessageToGroup(chrmsg);
				break;
			}
		}

		bool sendMessageToGroup(const char *message)
		{
			if (!ZGLeader->GetGroup())
				return true;
			if (ZGLeader->GetGroup() == NULL)
				return true;
			if (!playerGroup)
				return true;

			Player* GroupMember;
			const Group::MemberSlotList members = playerGroup->GetMemberSlots();
			for (Group::member_citerator itr = members.begin(); itr != members.end(); ++itr)
			{
				GroupMember = (Unit::GetPlayer(*me, itr->guid));
				if (GroupMember && GroupMember->GetSession())
				{
					if (!GroupMember)
						continue;
					GroupMember->GetSession()->SendAreaTriggerMessage(message);
				}
			}
			return true;
		}

		//reset revive
		void resetRevive() { GroupMembersDead = 0; }

		void reviveGroup()
		{
			GroupReference* grpRef = playerGroup->GetFirstMember();
			for (grpRef; grpRef != NULL; grpRef = grpRef->next())
			{
				Player* groupMember = grpRef->getSource();
				if (!groupMember)
					continue;

				//get players name
				std::string plrname = groupMember->GetName();
				char chrmsg[200];

				//we use instanced maps so unless area ids are needed no need to change
				Map * pMap = me->ToCreature()->GetMap();
				int32 memberscount = 0;
				int32 membersDead = 0;
				if (pMap)
				{
					Map::PlayerList const &Players = pMap->GetPlayers();
					for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
					{
						if (Player * pPlayer = itr->getSource())
						{
							//update mebers count.
							memberscount += 1;
							//Check if they are alive.
							if (!pPlayer->isAlive())
								membersDead += 1;
						}
					}
				}

				//yell group count
				sprintf(chrmsg, "Group Members = %u!", memberscount);
				me->MonsterYell(chrmsg, 0, 0);
				//yell dead count
				sprintf(chrmsg, "Group Members Dead = %u!", membersDead);
				me->MonsterYell(chrmsg, 0, 0);

				if (membersDead == memberscount)
				{//end game, players defeated
					sprintf(chrmsg, "The entire group has died! Resetting game! Teleporting losers to mall!");
					me->MonsterYell(chrmsg, 0, 0);
					sendMessageToGroup(chrmsg);
					RevivePlayers();
					TeleGroupToMall();
					ResetZombieGame();
				}
				if (membersDead >= 1 && zombiesSpawned <= 0)
				{//revive players
					RevivePlayers();
					membersDead = 0;
					sprintf(chrmsg, "Round %u cleared! Dead players revived!", zombieRound);
					me->MonsterYell(chrmsg, 0, 0);
				}
			}
		}

		void TeleGroupToMall()
		{//Teleport players to defined location
			Map * pMap = me->ToCreature()->GetMap();
			if (pMap)
			{
				Map::PlayerList const &Players = pMap->GetPlayers();
				for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
				{
					if (Player * pPlayer = itr->getSource())
					{
						if (pPlayer)
						{
							pPlayer->ResurrectPlayer(100, false);
							pPlayer->TeleportTo(POS_TELE_MALL);//steps
						}
					}
				}
			}
		}

		void RevivePlayers()
		{//revive players
			Map * pMap = me->ToCreature()->GetMap();
			char chrmsg[200];
			if (pMap)
			{
				Map::PlayerList const &Players = pMap->GetPlayers();
				for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
				{
					if (Player * pPlayer = itr->getSource())
					{
						//Check if they are dead.
						if (!pPlayer->isAlive())
						{
							std::string plrname = pPlayer->GetName();
							sprintf(chrmsg, "%s Revived!.", plrname.c_str());
							me->MonsterYell(chrmsg, 0, 0);
							pPlayer->ResurrectPlayer(100, false);
							pPlayer->TeleportTo(POS_TELE_REVIVE);
						}
					}
				}//reset revive counter
				resetRevive();
			}
		}

		void spawnZombies(Creature* creature, uint32 count)
		{
			zombiesSpawned = zombiesSpawned + count;
			for (uint32 i = 1; i <= count; i++)
			{
				creature->SummonCreature(ZOMBIE_ENEMY, POS_ENEMY_SPAWN);
			}
		}

		int getZombieSpawnCountPerWave()
		{
			return (zombieGroupMembers * ZOMBIES_PER_PERSON);
		}

		int getNumberOfWaves()
		{
			if (zombieRound >= 5)
			return 7;
			if (zombieRound >= 4)
			return 6;
			if (zombieRound >= 3)
			return 5;
			if (zombieRound >= 2)
			return 4;
			if (zombieRound <= 1)
			return 3;
		}

		int getZombieSpawnCount(Creature* creature)
		{
			return (zombieGroupMembers * ZOMBIES_PER_PERSON);
		}

		void CleanupNPCS()
		{
			//Cleanup NPC'S
			std::list<Creature*> zombie_enemy;
			zombie_enemy.clear();

			GetCreatureListWithEntryInGrid(zombie_enemy, me, ZOMBIE_ENEMY, 240.0f);
			for (auto itr : zombie_enemy)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
		}

		bool ResetZombieGame()
		{//reset the minigame
			CleanupNPCS();
			//player checks
			playerGroup = NULL;
			ZGLeader = NULL;
			//control npc updates
			me->UpdatePosition(POS_NPC_RESET, true);
			me->SetVisible(true);
			me->Respawn();
			//ZOMBIES game settings
			inZombieGame = false;
			inProgress = 0;
			zombiesSpawned = 0;
			zombieRound = 1;
			zombieWaves = 3;
			zombieWavesCount = 1;
			zombieSpawnPerWave = 0;
			zombieGroupMembers = 0;
			ZOMBIES_PER_PERSON = 3;
			return true;
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new zombie_minigame_controllerAI(creature);
	}
};

/*-----------------------
Zombie Enemy Minigame AI
----------------------*/

class MiniGame_Zombie : public CreatureScript
{
public:
	MiniGame_Zombie() : CreatureScript("MiniGame_Zombie") { }

	struct MiniGame_ZombieAI : public ScriptedAI
	{
		MiniGame_ZombieAI(Creature* c) : ScriptedAI(c) {}

		void JustDied(Unit* killer)
		{
			zombiesSpawned -= 1;

			if (me)
				me->DespawnOrUnsummon();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new MiniGame_ZombieAI(creature);
	}
};

void AddSC_Zombie_Mini_Game()
{
	new zombie_minigame_master();
	new zombie_minigame_ender();
	new zombie_minigame_controller();
	new MiniGame_Zombie();
}