/* BattleGame */
/* Scripts Created By: LebenHarvest */
/* @ ZwowZ.com */
/* Feel free to Use/Redistribute this script and its contents. */
/* Interested in more scripts or development for Trinitycore or other emulators? */
/* Feel free to contact me. */
/* Skype: leben.harvest */
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "Group.h"
//Game defaults
int32 Minions_Per_Wave = 1;

//players //TODO implement player tracking to record scores/stats etc
//g1
Player* G1Player1 = NULL;
Player* G1Player2 = NULL;
Player* G1Player3 = NULL;
Player* G1Player4 = NULL;
Player* G1Player5 = NULL;
//g2
Player* G2Player1 = NULL;
Player* G2Player2 = NULL;
Player* G2Player3 = NULL;
Player* G2Player4 = NULL;
Player* G2Player5 = NULL;

//char
char chrmsg[200];

//TELEPORTS
WorldLocation POS_TELE_MALL(870, 1698.93f, 1917.43f, 230.95f, 2.32f);//Server Mall/Spawn
WorldLocation POS_GROUP_ONE_TELE_REVIVE(189, 821.25f, 505.06f, 0, 5.4f);//GROUP 1 SPAWN
WorldLocation POS_GROUP_TWO_TELE_REVIVE(189, 927.47f, 373.69f, 0, 2.2);//GROUP 2 SPAWN
WorldLocation POS_GAME_LEADER_START(189, 815.93f, 331.41f, 0, 0.23f);//Game leader initial summon location

//CONTROL NPCS
WorldLocation POS_BATTLE_NPC_START_GAME(189, 821.25f, 505.06f, 0, 5.4f);//spawn loc of control npc
WorldLocation POS_BATTLE_NPC_NEXT_MINION_WAVE(189, 821.25f, 505.06f, 0, 5.4f); 
WorldLocation POS_BATTLE_NPC_CHECK_GAME(189, 821.25f, 505.06f, 0, 5.4f);
WorldLocation POS_BATTLE_NPC_END_GAME(189, 821.25f, 505.06f, 0, 5.4f);

enum Guardian_Battle_Data
{
	//red creatures
	NPC_SCARLET_GUARDIAN = 500647,
	NPC_SCARLET_CHAMPION1 = 500645,
	NPC_SCARLET_CHAMPION2 = 500649,
	NPC_SCARLET_SENTINEL1 = 500643,
	NPC_SCARLET_SENTINEL2 = 500655,
	NPC_SCARLET_SENTINEL3 = 500656,
	NPC_SCARLET_SENTINEL4 = 500657,
	NPC_SCARLET_SENTINEL5 = 500658,
	NPC_SCARLET_SENTINEL6 = 500659,

	//blue creatures
	NPC_UNDEAD_GUARDIAN = 500646,
	NPC_UNDEAD_CHAMPION1 = 500644,
	NPC_UNDEAD_CHAMPION2 = 500648,
	NPC_UNDEAD_SENTINEL1 = 500642,
	NPC_UNDEAD_SENTINEL2 = 500650,
	NPC_UNDEAD_SENTINEL3 = 500651,
	NPC_UNDEAD_SENTINEL4 = 500652,
	NPC_UNDEAD_SENTINEL5 = 500653,
	NPC_UNDEAD_SENTINEL6 = 500654,

	//CONTROLERS
	BATTLE_NPC_START_GAME = 75000,
	BATTLE_3v3_NPC_START_GAME = 57009,
	BATTLE_NPC_NEXT_MINION_WAVE = 57010,
	BATTLE_NPC_CHECK_GAME = 57011,
	BATTLE_NPC_END_GAME = 57012,

	//3v3 minions
	NPC_SCARLET_MINION_right = 75001,
	NPC_SCARLET_MINION_middle = 75002,
	NPC_SCARLET_MINION_left = 75003,

	NPC_UNDEAD_MINION_right = 75004,
	NPC_UNDEAD_MINION_middle = 75005,
	NPC_UNDEAD_MINION_left = 75006,

	//ai path helpers
	check_undead_minion_pathing_right = 76006,
	check_undead_minion_pathing_left = 76005,
	check_scarlet_minion_pathing_right = 76003,
	check_scarlet_minion_pathing_left = 76004,

	//middle
	check_undead_minion_pathing_middle = 76001,
	check_scarlet_minion_pathing_middle = 76002,
};

//World map positions
//red creatures
WorldLocation POS_3v3_NPC_SCARLET_GUARDIAN(189, 675, 406, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_CHAMPION1(189, 659, 355, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_CHAMPION2(189, 726, 415, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL1(189, 705, 300, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL2(189, 738, 328, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL3(189, 774, 359, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL4(189, 750, 249, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL5(189, 780, 277, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_SENTINEL6(189, 817, 310, 0, 5.4);
WorldLocation POS_3v3_NPC_SCARLET_MINION_1(189, 642, 372, 0, 5.4);//SENTINEL 1
WorldLocation POS_3v3_NPC_SCARLET_MINION_2(189, 700, 372, 0, 5.4);//RANDOM MID SPAWN
WorldLocation POS_3v3_NPC_SCARLET_MINION_3(189, 713, 430, 0, 5.4);//SENTNEL 3

//blue creatures
WorldLocation POS_3v3_NPC_UNDEAD_GUARDIAN(189, 909, 129, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_CHAMPION1(189, 931, 177, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_CHAMPION2(189, 865, 117, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL1(189, 776, 220, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL2(189, 802, 251, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL3(189, 839, 284, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL4(189, 881, 233, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL5(189, 847, 202, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_SENTINEL6(189, 815, 174, 0, 2.4);
WorldLocation POS_3v3_NPC_UNDEAD_MINION_1(189, 946, 158, 0, 2.4);//SENTINEL 1
WorldLocation POS_3v3_NPC_UNDEAD_MINION_2(189, 888, 159, 0, 2.4);//RANDOM MID SPAWN
WorldLocation POS_3v3_NPC_UNDEAD_MINION_3(189, 877, 102, 0, 2.4);//SENTNEL 3

//MINIONS RIGHT LANES PATHS
//red creatures
WorldLocation POS_3v3_RIGHT_SCARLET_MINION_MOVETO_GUARDIAN(189, 909, 129, 0, 0);
WorldLocation POS_3v3_LEFT_SCARLET_MINION_MOVETO_CHAMPION(189, 931, 177, 0, 0);
WorldLocation POS_3v3_RIGHT_SCARLET_MINION_MOVETO_SENTINEL(189, 815, 174, 0, 0);
//blue creatures
WorldLocation POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_GUARDIAN(189, 675, 406, 0, 0);
WorldLocation POS_3v3_LEFT_UNDEAD_MINION_MOVETO_CHAMPION(189, 659, 355, 0, 0);
WorldLocation POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_SENTINEL(189, 817, 310, 0, 0);

//MINIONS LEFT LANES PATHS
//red creatures
WorldLocation POS_3v3_LEFT_SCARLET_MINION_MOVETO_GUARDIAN(189, 909, 129, 0, 0);
WorldLocation POS_3v3_RIGHT_SCARLET_MINION_MOVETO_CHAMPION(189, 865, 117, 0, 0);
WorldLocation POS_3v3_LEFT_SCARLET_MINION_MOVETO_SENTINEL(189, 881, 233, 0, 0);
//blue creatures
WorldLocation POS_3v3_LEFT_UNDEAD_MINION_MOVETO_GUARDIAN(189, 675, 406, 0, 0);
WorldLocation POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_CHAMPION(189, 726, 415, 0, 0);
WorldLocation POS_3v3_LEFT_UNDEAD_MINION_MOVETO_SENTINEL(189, 750, 250, 0, 0);

//RANDOM MINION MOVE TO POSITIONS
//red creatures
WorldLocation POS_3v3_RANDOM_SCARLET_MINION_MOVETO_GUARDIAN(189, 675, 406, 0, 0);
WorldLocation POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_random(189, 700, 372, 0, 0);
WorldLocation POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_left(189, 736, 403, 0, 0);
WorldLocation POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_right(189, 666, 346, 0, 0);

//blue creatures
WorldLocation POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_GUARDIAN(189, 909, 129, 0, 0);
WorldLocation POS_3v3_RANDOM_SCARLET_MINION_MOVETO_random(189, 888, 156, 0, 0);
WorldLocation POS_3v3_RANDOM_SCARLET_MINION_MOVETO_left(189, 885, 128, 0, 0);
WorldLocation POS_3v3_RANDOM_SCARLET_MINION_MOVETO_right(189, 922, 178, 0, 0);

//TELEPORTS
WorldLocation POS_3v3_GROUP_ONE_TELE_REVIVE(189, 924, 116, 0, 2.4);//GROUP 1 SPAWN
WorldLocation POS_3v3_GROUP_TWO_TELE_REVIVE(189, 667, 416, 0, 5.4);//GROUP 2 SPAWN
WorldLocation POS_3v3_GAME_LEADER_START(189, 600, 414, 0, 5.4);//Game leader start

//Game status
int32 GameInProgress3v3 = 0;

//Player/Group 1 variables
int32 Group1GuardianAlive3v3 = 0;
int32 Group1Champion1Alive3v3 = 0;
int32 Group1Champion2Alive3v3 = 0;
int32 Group1Sentinel1Alive3v3 = 0;
int32 Group1Sentinel2Alive3v3 = 0;
int32 Group1Sentinel3Alive3v3 = 0;
int32 Group1Sentinel4Alive3v3 = 0;
int32 Group1Sentinel5Alive3v3 = 0;
int32 Group1Sentinel6Alive3v3 = 0;

//Player/Group 2 variables
int32 Group2GuardianAlive3v3 = 0;
int32 Group2Champion1Alive3v3 = 0;
int32 Group2Champion2Alive3v3 = 0;
int32 Group2Sentinel1Alive3v3 = 0;
int32 Group2Sentinel2Alive3v3 = 0;
int32 Group2Sentinel3Alive3v3 = 0;
int32 Group2Sentinel4Alive3v3 = 0;
int32 Group2Sentinel5Alive3v3 = 0;
int32 Group2Sentinel6Alive3v3 = 0;

//Game leaders
Player* GameLeader3v3 = NULL;
Player* GameLeader3v32 = NULL;

//team queue trackers
int32 Team1Queued3v3 = 0;
int32 Team2Queued3v3 = 0;

//GROUPS
Group* grp13v3 = NULL;
Group* grp23v3 = NULL;

class Battle_Game_master3v3 : public CreatureScript
{
public:
	Battle_Game_master3v3() : CreatureScript("Battle_Game_master3v3") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{	//Join game in progress
		if (GameInProgress3v3 == 1 && player != GameLeader3v3 && player != GameLeader3v32)//
		{//both groups are not full, then allow players to join.
			if (GameLeader3v3->GetGroup()->GetMembersCount() < 3 && GameLeader3v32->GetGroup()->GetMembersCount() < 3)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Join Game in progress!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			}else
			{//tell player the game is full
				return closeGossipNotify(player, "Sorry, the game is full!");
			}
		}//allow game leaders to start the game.
		if (player == GameLeader3v3 && GameInProgress3v3 == 1 && Team2Queued3v3 == 1)//
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Start The Game!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		}//New Game
		if (GameInProgress3v3 == 0 && Team2Queued3v3 == 0 && player != GameLeader3v3 && player != GameLeader3v32)//game not in progress & group two not queued. Do not allow players to start game with self.
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Queue for new Guardian Battle!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		//GM reset game
		if (player->GetSession()->GetSecurity() >= 1)
		{
			player->ADD_GOSSIP_ITEM(9, "RESET EVENT!", GOSSIP_SENDER_MAIN, 1);
		}
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		return true;
	}

	// Teleport to Player
	static bool HandleAppearToGameLeader3v3(Player* plr, char const* args)
	{
		Player* target;
		uint64 targetGuid;
		std::string targetName;

		Player* _player = plr->GetSession()->GetPlayer();
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (uiAction)
		{
			case GOSSIP_ACTION_INFO_DEF + 1:
			{
				//first team put into queue first
				if (Team1Queued3v3 == 0)
				{
					GameLeader3v3 = player;//set game owner
					Team1Queued3v3 = 1;
					return closeGossipNotify(player, "You have been queued as Game Leader #1 for the Guardian Battle!");
				}
				//let second team queue
				if (Team2Queued3v3 == 0)
				{
					GameLeader3v32 = player;//set game owner 2
					Team2Queued3v3 = 1;
					//set game in progress
					GameInProgress3v3 = 1;//set game in progress because players are being summoned
					//Summon players to event Lobby.
					if (Team1Queued3v3 == 1 && Team2Queued3v3 == 1)
					{
						//tele leader
						GameLeader3v3->TeleportTo(POS_3v3_GAME_LEADER_START);
						//GameLeader3v32->TeleportTo(POS_3v3_GAME_LEADER_START);
					}
					return closeGossipNotify(player, "The Guardian Battle is starting, You are Game Leader #2!");
				}else
				{
					return closeGossipNotify(player, "You are already in queue or a battle is currently being started!");
				}
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 2:
			{
				player->PlayerTalkClass->SendCloseGossip();
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 3:
			{
				std::string gmName = player->GetSession()->GetPlayerName();
				sprintf(chrmsg, "Guardian Battle reset by %s!", gmName.c_str());
				GameInProgress3v3 = 0;//reset battle
				return closeGossipNotify(player, chrmsg);
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 5:
			{
				if (GameInProgress3v3 == 1 && player != GameLeader3v3)
				{
					return closeGossipNotify(player, "Sorry, only the Game Leaders can start the game!");
				}

				//Summon players to event spawns and create groups.
				if (Team1Queued3v3 == 1 && Team2Queued3v3 == 1)
				{//setup game
					creature->SummonCreature(BATTLE_NPC_START_GAME, POS_BATTLE_NPC_START_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);//5 seconds

					//Create Groups

					//create group one
					grp13v3 = new Group(); //Create the Group
					grp13v3->Create(GameLeader3v3);
					grp13v3->SendUpdate();

					//create group two
					grp23v3 = new Group(); //Create the Group
					grp23v3->Create(GameLeader3v32);
					grp23v3->SendUpdate();

					//summon the groups to start positions
					for (GroupReference* itr = grp13v3->GetFirstMember(); itr != NULL; itr = itr->next())
					{
						Player* player = itr->getSource();

						if (!player || !player->GetSession())
							continue;

						player->IsInAnEvent = 1;

						//teleport group one
						player->TeleportTo(POS_3v3_GROUP_ONE_TELE_REVIVE);

						//Set Undead faction
						player->setFaction(2);
					}

					for (GroupReference* itr = grp23v3->GetFirstMember(); itr != NULL; itr = itr->next())
					{
						Player* player = itr->getSource();

						if (!player || !player->GetSession())
							continue;

						//unbind groups/players instances
						grp23v3->ResetInstances(INSTANCE_RESET_GROUP_DISBAND, false, NULL);
						grp23v3->ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, NULL);
						player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false);
						player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true);

						Map* map = player->GetMap();//players map
						if (map)//player has a map
						{
							//Map* glmap = GameLeader->GetMap();//set game leader map
							int32 glmap = 189;//event map
							player->UnbindInstance(glmap, GameLeader3v3->GetDungeonDifficulty(), true);
							//new bind
							InstancePlayerBind* bind = player->GetBoundInstance(glmap, GameLeader3v3->GetDifficulty(map->IsRaid()));

							Group* Tehgroup = grp23v3;//= GameLeader->GetGroup();

							if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(GameLeader3v3->GetInstanceId()))
								player->BindToInstance(save, !save->CanReset());
						}

						// stop flight if need
						if (player->isInFlight())
						{
							player->GetMotionMaster()->MovementExpired();
							player->CleanupAfterTaxiFlight();
						}
						// save only in non-flight case
						else
							player->SaveRecallPosition();

						// to point to see at target with same orientation
						float x, y, z;
						GameLeader3v3->GetContactPoint(player, x, y, z);

						player->TeleportTo(GameLeader3v3->GetMapId(), x, y, z, player->GetAngle(GameLeader3v3), TELE_TO_GM_MODE);
						player->SetPhaseMask(GameLeader3v3->GetPhaseMask(), true);

						player->TeleportTo(POS_3v3_GAME_LEADER_START);
						ChatHandler(player->GetSession()).PSendSysMessage("Entering event...");

						//Set Scarlet faction
						//player->setFaction(11);

						player->IsInAnEvent = 1;

						//teleport group two to spawn
						player->TeleportTo(POS_3v3_GROUP_TWO_TELE_REVIVE);

						//Set Scarlet faction
						player->setFaction(11);
					}
					//set game in progress
					GameInProgress3v3 = 1;//set game in progress because players are being summoned
					return closeGossipNotify(player, "The Guardian Battle is starting!");
				}else
				{
					return closeGossipNotify(player, "Your group is already queued/Bad Group!");
				}
				break;
			}
			case GOSSIP_ACTION_INFO_DEF + 6:
			{
				Group* grp = player->GetGroup();//get group

				if (!grp)//if no group allow join
				{
					if (grp13v3->GetMembersCount() > grp23v3->GetMembersCount())//Group one has more members
					{//add player to group two
						grp23v3->AddMember(player);
						player->IsInAnEvent = 1;
						//teleport group two to spawn
						player->TeleportTo(POS_3v3_GROUP_TWO_TELE_REVIVE);
						//set faction
						//Set Scarlet faction
						player->setFaction(11);
					}else
					{//add player to group one
						grp13v3->AddMember(player);
						player->IsInAnEvent = 1;
						//teleport group two to spawn
						player->TeleportTo(POS_3v3_GROUP_ONE_TELE_REVIVE);
						//set faction
						//Set Undead faction
						player->setFaction(2);
					}
				}else
				{
					return closeGossipNotify(player, "Your are already in a group, Please leave or disband your group!");
				}
				break;
			}
		}
		player->PlayerTalkClass->SendCloseGossip();
		return true;
	}

	struct Battle_Game_master3v3AI : public ScriptedAI
	{
		Battle_Game_master3v3AI(Creature* c) : ScriptedAI(c), Summons(me) {}

		SummonList Summons;

		void JustSummoned(Creature* summon)
		{
			if (summon)
			{
				uint32 entry = summon->GetEntry();
				Summons.Summon(summon);

				//set npc factions and initial settings
				if (entry == NPC_UNDEAD_CHAMPION1 || entry == NPC_UNDEAD_SENTINEL1 || entry == NPC_UNDEAD_CHAMPION2 || entry == NPC_UNDEAD_SENTINEL2 || entry == NPC_UNDEAD_SENTINEL3 || entry == NPC_UNDEAD_SENTINEL4 || entry == NPC_UNDEAD_SENTINEL5 || entry == NPC_UNDEAD_SENTINEL6 || entry == NPC_UNDEAD_GUARDIAN)
				{
					if (!me->HasAura(SPELL_ROOT_FOR_EVER))
						me->AddAura(SPELL_ROOT_FOR_EVER, me);

					summon->setFaction(2);//undead faction
				}
				if (entry == NPC_SCARLET_CHAMPION1 || entry == NPC_SCARLET_SENTINEL1 || entry == NPC_SCARLET_CHAMPION2 || entry == NPC_SCARLET_SENTINEL2 || entry == NPC_SCARLET_SENTINEL3 || entry == NPC_SCARLET_SENTINEL4 || entry == NPC_SCARLET_SENTINEL5 || entry == NPC_SCARLET_SENTINEL6 || entry == NPC_SCARLET_GUARDIAN)
				{
					if (!me->HasAura(SPELL_ROOT_FOR_EVER))
						me->AddAura(SPELL_ROOT_FOR_EVER, me);

					summon->setFaction(1);//scarlet faction
				}

				//SET MINION FACTIONS
				if (entry == NPC_UNDEAD_MINION)
				{
					summon->setFaction(2);//UNDEAD faction
				}
				if (entry == NPC_SCARLET_MINION)
				{
					summon->setFaction(1);//SCARLET faction
				}

				if (entry == BATTLE_NPC_START_GAME || entry == BATTLE_NPC_NEXT_MINION_WAVE ||
					entry == BATTLE_NPC_CHECK_GAME || entry == BATTLE_NPC_END_GAME)
				{
					summon->SetUnitMovementFlags(MOVEMENTFLAG_ROOT);
					//do nothing
				}else
				{//attack - minions
					Player* SelNearestPlr = summon->SelectNearestPlayer(2000.0f);
					if (SelNearestPlr && !SelNearestPlr->isGameMaster())
					{
						//summon->Attack(SelNearestPlr, false);
						//sprintf(chrmsg, "Minions attack nearest players/enemy called!");
						//me->MonsterYell(chrmsg, 0, 0);
					}else
					{
						//summon->GetMotionMaster()->MovePath();
						/* cant find a player to select */
					}
				}
			}
		}

		void SummonedCreatureDespawn(Creature* summon)
		{
			uint32 summonID = summon->GetEntry();
			if (summonID == BATTLE_NPC_START_GAME)
			{	//spawn group two back into world
				for (GroupReference* itr = grp13v3->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* player = itr->getSource();

					player->IsInAnEvent = 1;

					if (!player || !player->GetSession())
						continue;

					player->TeleportTo(POS_3v3_GROUP_ONE_TELE_REVIVE);
					//Set Scarlet faction
					player->setFaction(2);
				}
				for (GroupReference* itr = grp23v3->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* player = itr->getSource();

					player->IsInAnEvent = 1;

					if (!player || !player->GetSession())
						continue;

					player->TeleportTo(POS_3v3_GROUP_TWO_TELE_REVIVE);
					//Set Scarlet faction
					player->setFaction(2);
				}
				//Update and check out map //always update factions
				Map * pMap = me->ToCreature()->GetMap();
				char chrmsg[200];
				if (pMap)
				{
					Map::PlayerList const &Players = pMap->GetPlayers();
					for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
					{
						if (Player * pPlayer = itr->getSource())
						{
							if (GameInProgress3v3 == 0)
							{//SOMETHING WENT WRONG before game could start
								//teleport to mall
								pPlayer->TeleportTo(POS_TELE_MALL);
								//Set default faction
								pPlayer->setFactionForRace(pPlayer->getRace());
							}

							//update player factions
							if (pPlayer->GetGroup()->GetGUID() == grp13v3->GetGUID())
							{
								pPlayer->setFaction(2);
							}
							if (pPlayer->GetGroup()->GetGUID() == grp23v3->GetGUID())
							{
								pPlayer->setFaction(11);
							}

							//update player pvp flags
							if (!pPlayer->HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP))
							{
								pPlayer->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);
								pPlayer->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP);
								pPlayer->ToPlayer()->UpdatePvP(true, true);
							}

							//remove non game players
							if (!pPlayer->GetGroup())// || pPlayer->GetGroup()->GetGUID() != grp1->GetGUID() || pPlayer->GetGroup()->GetGUID() != grp2->GetGUID())
							{
								sprintf(chrmsg, "Player removed from game area, Not in current game!");
								me->MonsterYell(chrmsg, 0, 0);
							}
						}
					}
				}
				sprintf(chrmsg, "Game start called!");
				summon->MonsterYell(chrmsg, 0, 0);
				sprintf(chrmsg, "A wave of minions will spawn every 20 seconds!");
				me->MonsterYell(chrmsg, 0, 0);

				//set group 1 variables
				Group1GuardianAlive3v3 = 1;
				Group1Champion1Alive3v3 = 1;
				Group1Champion2Alive3v3 = 1;
				Group1Sentinel1Alive3v3 = 1;
				Group1Sentinel2Alive3v3 = 1;
				Group1Sentinel3Alive3v3 = 1;
				Group1Sentinel4Alive3v3 = 1;
				Group1Sentinel5Alive3v3 = 1;
				Group1Sentinel6Alive3v3 = 1;

				//set group 2 variables
				Group2GuardianAlive3v3 = 1;
				Group2Champion1Alive3v3 = 1;
				Group2Champion2Alive3v3 = 1;
				Group2Sentinel1Alive3v3 = 1;
				Group2Sentinel2Alive3v3 = 1;
				Group2Sentinel3Alive3v3 = 1;
				Group2Sentinel4Alive3v3 = 1;
				Group2Sentinel5Alive3v3 = 1;
				Group2Sentinel6Alive3v3 = 1;

				//start summoning waves
				me->SummonCreature(BATTLE_NPC_NEXT_MINION_WAVE, POS_BATTLE_NPC_NEXT_MINION_WAVE, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);//Spawn minions every 30 seconds.
				me->SummonCreature(BATTLE_NPC_CHECK_GAME, POS_BATTLE_NPC_CHECK_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);//1 second check.

				//Group one SCARLET spawns
				me->SummonCreature(NPC_SCARLET_SENTINEL1, POS_3v3_NPC_SCARLET_SENTINEL1, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_SENTINEL2, POS_3v3_NPC_SCARLET_SENTINEL2, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_SENTINEL3, POS_3v3_NPC_SCARLET_SENTINEL3, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_SENTINEL4, POS_3v3_NPC_SCARLET_SENTINEL4, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_SENTINEL5, POS_3v3_NPC_SCARLET_SENTINEL5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_SENTINEL6, POS_3v3_NPC_SCARLET_SENTINEL6, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_CHAMPION1, POS_3v3_NPC_SCARLET_CHAMPION1, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_CHAMPION2, POS_3v3_NPC_SCARLET_CHAMPION2, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_SCARLET_GUARDIAN, POS_3v3_NPC_SCARLET_GUARDIAN, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				//Group two UNDEAD spawns
				me->SummonCreature(NPC_UNDEAD_SENTINEL1, POS_3v3_NPC_UNDEAD_SENTINEL1, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_SENTINEL2, POS_3v3_NPC_UNDEAD_SENTINEL2, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_SENTINEL3, POS_3v3_NPC_UNDEAD_SENTINEL3, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_SENTINEL4, POS_3v3_NPC_UNDEAD_SENTINEL4, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_SENTINEL5, POS_3v3_NPC_UNDEAD_SENTINEL5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_SENTINEL6, POS_3v3_NPC_UNDEAD_SENTINEL6, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_CHAMPION1, POS_3v3_NPC_UNDEAD_CHAMPION1, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_CHAMPION2, POS_3v3_NPC_UNDEAD_CHAMPION2, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
				me->SummonCreature(NPC_UNDEAD_GUARDIAN, POS_3v3_NPC_UNDEAD_GUARDIAN, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);//
			}
			switch (summonID)
			{
				/* called when a wave of zombies needs to be spawned */
			case BATTLE_NPC_NEXT_MINION_WAVE:
			{
				//SPAWN WAVES FOR EACH GROUP
				//sprintf(chrmsg, "A wave of minions has spawned!");
				//me->MonsterYell(chrmsg, 0, 0);
				spawnMinions(me);
				RevivePlrs();

				if (GameInProgress3v3 == 1)
					//Continue Spawning waves.
					me->SummonCreature(BATTLE_NPC_NEXT_MINION_WAVE, POS_BATTLE_NPC_NEXT_MINION_WAVE, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);//Spawn minions every 20 seconds.
				break;
			}
			case BATTLE_NPC_CHECK_GAME:
			{
				//Group 1 lose variables
				if (Group1GuardianAlive3v3 == 0)
				{
					sprintf(chrmsg, "Group 2 Wins!");
					me->MonsterYell(chrmsg, 0, 0);
					//end game winners grp2
					me->SummonCreature(BATTLE_NPC_END_GAME, POS_BATTLE_NPC_END_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 8000);
					return;
				}

				//Group 2 lose variables
				if (Group2GuardianAlive3v3 == 0)
				{
					sprintf(chrmsg, "Group 1 Wins!");
					me->MonsterYell(chrmsg, 0, 0);
					//end game winners grp1
					me->SummonCreature(BATTLE_NPC_END_GAME, POS_BATTLE_NPC_END_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 8000);
					return;
				}

				if (GameInProgress3v3 == 1)
					me->SummonCreature(BATTLE_NPC_CHECK_GAME, POS_BATTLE_NPC_CHECK_GAME, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);//1 second check.
				break;
			}
			case BATTLE_NPC_END_GAME:
			{
				sprintf(chrmsg, "The Game Has Ended!");
				me->MonsterYell(chrmsg, 0, 0);
				GameInProgress3v3 = 0;
				ResetGuardianBattle();//reset event
				break;
			}
			case NPC_UNDEAD_GUARDIAN:
			{
				sprintf(chrmsg, "Undead Guardian Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group one undead
				Group1GuardianAlive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_CHAMPION1:
			{
				sprintf(chrmsg, "Undead Champion 1 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Champion1Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_CHAMPION2:
			{
				sprintf(chrmsg, "Undead Champion 2 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Champion2Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL1:
			{
				sprintf(chrmsg, "Undead Sentinel 1 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel1Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL2:
			{
				sprintf(chrmsg, "Undead Sentinel 2 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel2Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL3:
			{
				sprintf(chrmsg, "Undead Sentinel 3 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel3Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL4:
			{
				sprintf(chrmsg, "Undead Sentinel 4 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel4Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL5:
			{
				sprintf(chrmsg, "Undead Sentinel 5 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel5Alive3v3 = 0;
				break;
			}
			case NPC_UNDEAD_SENTINEL6:
			{
				sprintf(chrmsg, "Undead Sentinel 6 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group1Sentinel6Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_GUARDIAN:
			{
				sprintf(chrmsg, "Scarlet Guardian Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2GuardianAlive3v3 = 0;
				break;
			}
			case NPC_SCARLET_CHAMPION1:
			{
				sprintf(chrmsg, "Scarlet Champion 1 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Champion1Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_CHAMPION2:
			{
				sprintf(chrmsg, "Scarlet Champion 2 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Champion2Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL1:
			{
				sprintf(chrmsg, "Scarlet Sentinel 1 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel1Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL2:
			{
				sprintf(chrmsg, "Scarlet Sentinel 2 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel2Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL3:
			{
				sprintf(chrmsg, "Scarlet Sentinel 3 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel3Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL4:
			{
				sprintf(chrmsg, "Scarlet Sentinel 4 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel4Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL5:
			{
				sprintf(chrmsg, "Scarlet Sentinel 5 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel5Alive3v3 = 0;
				break;
			}
			case NPC_SCARLET_SENTINEL6:
			{
				sprintf(chrmsg, "Scarlet Sentinel 6 Slain!");
				me->MonsterYell(chrmsg, 0, 0);
				//Group two scarlet
				Group2Sentinel6Alive3v3 = 0;
				break;
			}
			default:
				break;
			}
		}

		void RevivePlrs()
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
						if (pPlayer->GetAreaId() == me->GetAreaId())
						{
							//Check if they are dead. and revive
							if (!pPlayer->isAlive() && pPlayer->GetGroup()->GetGUID() == grp13v3->GetGUID())
							{//team 1
								std::string plrname = pPlayer->GetName();
								sprintf(chrmsg, "Team 1: %s Revived!.", plrname.c_str());
								me->MonsterYell(chrmsg, 0, 0);
								pPlayer->ResurrectPlayer(100, false);
								pPlayer->TeleportTo(POS_3v3_GROUP_ONE_TELE_REVIVE);
								pPlayer->setFaction(2);
							}

							if (!pPlayer->isAlive() && pPlayer->GetGroup()->GetGUID() == grp23v3->GetGUID())
							{//team 2
								std::string plrname = pPlayer->GetName();
								sprintf(chrmsg, "Team 2: %s Revived!.", plrname.c_str());
								me->MonsterYell(chrmsg, 0, 0);
								pPlayer->ResurrectPlayer(100, false);
								pPlayer->TeleportTo(POS_3v3_GROUP_TWO_TELE_REVIVE);
								pPlayer->setFaction(11);
							}

							//update player factions
							if (pPlayer->GetGroup()->GetGUID() == grp13v3->GetGUID())
							{
								pPlayer->setFaction(2);
							}
							if (pPlayer->GetGroup()->GetGUID() == grp23v3->GetGUID())
							{
								pPlayer->setFaction(11);
							}

							//update player pvp flags
							if (!pPlayer->HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP))
							{
								pPlayer->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);
								pPlayer->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP);
								pPlayer->ToPlayer()->UpdatePvP(true, true);
							}
						}
					}
				}
			}
		}

		void spawnMinions(Creature* creature)
		{
			if (GameInProgress3v3 == 1)
			{
				for (uint32 i = 1; i <= Minions_Per_Wave; i++)
				{//called every 20 seconds
					//SCARLET MINIONS
					creature->SummonCreature(NPC_SCARLET_MINION_right, POS_3v3_NPC_SCARLET_MINION_1);
					creature->SummonCreature(NPC_SCARLET_MINION_middle, POS_3v3_NPC_SCARLET_MINION_2);
					creature->SummonCreature(NPC_SCARLET_MINION_left, POS_3v3_NPC_SCARLET_MINION_3);
					//UNDEAD MINIONS
					creature->SummonCreature(NPC_UNDEAD_MINION_right, POS_3v3_NPC_UNDEAD_MINION_1);
					creature->SummonCreature(NPC_UNDEAD_MINION_middle, POS_3v3_NPC_UNDEAD_MINION_2);
					creature->SummonCreature(NPC_UNDEAD_MINION_left, POS_3v3_NPC_UNDEAD_MINION_3);
				}
			}
		}

		void CleanupNPCS()
		{
			//Cleanup NPC'S

			//SCARLETS
			std::list<Creature*> Scarlet_Minion1;
			Scarlet_Minion1.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Minion1, me, NPC_SCARLET_MINION_right, 5000.0f);
			for (auto itr : Scarlet_Minion1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Minion2;
			Scarlet_Minion2.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Minion2, me, NPC_SCARLET_MINION_middle, 5000.0f);
			for (auto itr : Scarlet_Minion2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Minion3;
			Scarlet_Minion3.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Minion3, me, NPC_SCARLET_MINION_left, 5000.0f);
			for (auto itr : Scarlet_Minion3)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel1;
			Scarlet_Sentinel1.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel1, me, NPC_SCARLET_SENTINEL1, 5000.0f);
			for (auto itr : Scarlet_Sentinel1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel2;
			Scarlet_Sentinel2.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel2, me, NPC_SCARLET_SENTINEL2, 5000.0f);
			for (auto itr : Scarlet_Sentinel2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel3;
			Scarlet_Sentinel3.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel3, me, NPC_SCARLET_SENTINEL3, 5000.0f);
			for (auto itr : Scarlet_Sentinel3)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel4;
			Scarlet_Sentinel4.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel4, me, NPC_SCARLET_SENTINEL4, 5000.0f);
			for (auto itr : Scarlet_Sentinel4)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel5;
			Scarlet_Sentinel5.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel5, me, NPC_SCARLET_SENTINEL5, 5000.0f);
			for (auto itr : Scarlet_Sentinel5)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Sentinel6;
			Scarlet_Sentinel6.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Sentinel6, me, NPC_SCARLET_SENTINEL6, 5000.0f);
			for (auto itr : Scarlet_Sentinel6)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Champion1;
			Scarlet_Champion1.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Champion1, me, NPC_SCARLET_CHAMPION1, 5000.0f);
			for (auto itr : Scarlet_Champion1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Champion2;
			Scarlet_Champion2.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Champion2, me, NPC_SCARLET_CHAMPION2, 5000.0f);
			for (auto itr : Scarlet_Champion2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Scarlet_Guardian;
			Scarlet_Guardian.clear();

			GetCreatureListWithEntryInGrid(Scarlet_Guardian, me, NPC_SCARLET_GUARDIAN, 5000.0f);
			for (auto itr : Scarlet_Guardian)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}

			//UNDEADS
			std::list<Creature*> Undead_Minion1;
			Undead_Minion1.clear();

			GetCreatureListWithEntryInGrid(Undead_Minion1, me, NPC_UNDEAD_MINION_right, 5000.0f);
			for (auto itr : Undead_Minion1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Minion2;
			Undead_Minion2.clear();

			GetCreatureListWithEntryInGrid(Undead_Minion2, me, NPC_UNDEAD_MINION_middle, 5000.0f);
			for (auto itr : Undead_Minion2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Minion3;
			Undead_Minion3.clear();

			GetCreatureListWithEntryInGrid(Undead_Minion3, me, NPC_UNDEAD_MINION_left, 5000.0f);
			for (auto itr : Undead_Minion3)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel1;
			Undead_Sentinel1.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel1, me, NPC_UNDEAD_SENTINEL1, 5000.0f);
			for (auto itr : Undead_Sentinel1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel2;
			Undead_Sentinel2.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel2, me, NPC_UNDEAD_SENTINEL2, 5000.0f);
			for (auto itr : Undead_Sentinel2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel3;
			Undead_Sentinel3.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel3, me, NPC_UNDEAD_SENTINEL3, 5000.0f);
			for (auto itr : Undead_Sentinel3)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel4;
			Undead_Sentinel4.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel4, me, NPC_UNDEAD_SENTINEL4, 5000.0f);
			for (auto itr : Undead_Sentinel4)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel5;
			Undead_Sentinel5.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel5, me, NPC_UNDEAD_SENTINEL5, 5000.0f);
			for (auto itr : Undead_Sentinel5)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Sentinel6;
			Undead_Sentinel6.clear();

			GetCreatureListWithEntryInGrid(Undead_Sentinel1, me, NPC_UNDEAD_SENTINEL6, 5000.0f);
			for (auto itr : Undead_Sentinel6)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Champion1;
			Undead_Champion1.clear();

			GetCreatureListWithEntryInGrid(Undead_Champion1, me, NPC_UNDEAD_CHAMPION1, 5000.0f);
			for (auto itr : Undead_Champion1)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Champion2;
			Undead_Champion2.clear();

			GetCreatureListWithEntryInGrid(Undead_Champion2, me, NPC_UNDEAD_CHAMPION2, 5000.0f);
			for (auto itr : Undead_Champion2)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
			std::list<Creature*> Undead_Guardian;
			Undead_Guardian.clear();

			GetCreatureListWithEntryInGrid(Undead_Guardian, me, NPC_UNDEAD_GUARDIAN, 5000.0f);
			for (auto itr : Undead_Guardian)
			{
				if (!itr)
					continue;

				if (itr->isSummon())
					itr->ForcedDespawn();
			}
		}

		bool ResetGuardianBattle()
		{//reset the minigame
			CleanupNPCS();
			//Game Settings
			GameInProgress3v3 = 0;
			Minions_Per_Wave = 1;

			//RESET GAME LEADERS
			GameLeader3v3 = NULL;
			GameLeader3v32 = NULL;

			//Group 1 reset
			grp13v3 == NULL;
			Group1GuardianAlive3v3 = 0;

			Group1Champion1Alive3v3 = 0;
			Group1Champion2Alive3v3 = 0;
			Group1Sentinel1Alive3v3 = 0;
			Group1Sentinel2Alive3v3 = 0;
			Group1Sentinel3Alive3v3 = 0;
			Group1Sentinel4Alive3v3 = 0;
			Group1Sentinel5Alive3v3 = 0;
			Group1Sentinel6Alive3v3 = 0;

			//Group 2 reset
			grp23v3 == NULL;
			Group2GuardianAlive3v3 = 0;

			Group2Champion1Alive3v3 = 0;
			Group2Champion2Alive3v3 = 0;
			Group2Sentinel1Alive3v3 = 0;
			Group2Sentinel2Alive3v3 = 0;
			Group2Sentinel3Alive3v3 = 0;
			Group2Sentinel4Alive3v3 = 0;
			Group2Sentinel5Alive3v3 = 0;
			Group2Sentinel6Alive3v3 = 0;

			//team queue reset
			Team1Queued3v3 = 0;
			Team2Queued3v3 = 0;

			sprintf(chrmsg, "3v3 Event reset!!!");
			me->MonsterYell(chrmsg, 0, 0);
			return true;
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Battle_Game_master3v3AI(creature);
	}
};

/*-----------------------
Minion AI 3v3
----------------------*/
class Battle_Game_Scarlet_Minion3v3_RIGHT_LANE : public CreatureScript
{
public:
	Battle_Game_Scarlet_Minion3v3_RIGHT_LANE() : CreatureScript("Battle_Game_Scarlet_Minion3v3_RIGHT_LANE") { }
	struct Battle_Game_Scarlet_Minion3v3_RIGHT_LANEAI : public ScriptedAI
	{
		Battle_Game_Scarlet_Minion3v3_RIGHT_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			//moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_scarlet_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_RIGHT_SCARLET_MINION_MOVETO_CHAMPION);
				}
				if (me->FindNearestCreature(check_scarlet_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(2, POS_3v3_RIGHT_SCARLET_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};
	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Scarlet_Minion3v3_RIGHT_LANEAI(pCreature);
	}
};

class Battle_Game_Undead_Minion3v3_RIGHT_LANE : public CreatureScript
{
public:
	Battle_Game_Undead_Minion3v3_RIGHT_LANE() : CreatureScript("Battle_Game_Undead_Minion3v3_RIGHT_LANE") { }
	struct Battle_Game_Undead_Minion3v3_RIGHT_LANEAI : public ScriptedAI
	{
		Battle_Game_Undead_Minion3v3_RIGHT_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_undead_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_CHAMPION);
				}
				if (me->FindNearestCreature(check_undead_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(2, POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};

	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Undead_Minion3v3_RIGHT_LANEAI(pCreature);
	}
};

class Battle_Game_Scarlet_Minion3v3_LEFT_LANE : public CreatureScript
{
public:
	Battle_Game_Scarlet_Minion3v3_LEFT_LANE() : CreatureScript("Battle_Game_Scarlet_Minion3v3_LEFT_LANE") { }
	struct Battle_Game_Scarlet_Minion3v3_LEFT_LANEAI : public ScriptedAI
	{
		Battle_Game_Scarlet_Minion3v3_LEFT_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			//moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_scarlet_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_LEFT_SCARLET_MINION_MOVETO_CHAMPION);
				}
				if (me->FindNearestCreature(check_scarlet_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(2, POS_3v3_LEFT_SCARLET_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};
	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Scarlet_Minion3v3_LEFT_LANEAI(pCreature);
	}
};

class Battle_Game_Undead_Minion3v3_LEFT_LANE : public CreatureScript
{
public:
	Battle_Game_Undead_Minion3v3_LEFT_LANE() : CreatureScript("Battle_Game_Undead_Minion3v3_LEFT_LANE") { }
	struct Battle_Game_Undead_Minion3v3_LEFT_LANEAI : public ScriptedAI
	{
		Battle_Game_Undead_Minion3v3_LEFT_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_undead_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_LEFT_UNDEAD_MINION_MOVETO_CHAMPION);
				}
				if (me->FindNearestCreature(check_undead_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(2, POS_3v3_LEFT_UNDEAD_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};

	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Undead_Minion3v3_LEFT_LANEAI(pCreature);
	}
};

class Battle_Game_Scarlet_Minion3v3_MIDDLE_LANE : public CreatureScript
{
public:
	Battle_Game_Scarlet_Minion3v3_MIDDLE_LANE() : CreatureScript("Battle_Game_Scarlet_Minion3v3_MIDDLE_LANE") { }
	struct Battle_Game_Scarlet_Minion3v3_MIDDLE_LANEAI : public ScriptedAI
	{
		Battle_Game_Scarlet_Minion3v3_MIDDLE_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			//moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_scarlet_minion_pathing_middle, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_RANDOM_SCARLET_MINION_MOVETO_random);
				}
				if (me->FindNearestCreature(check_scarlet_minion_pathing_middle, 10, true))
				{
					if (roll_chance_i(50))
					{
						me->GetMotionMaster()->MovePoint(2, POS_3v3_RANDOM_SCARLET_MINION_MOVETO_left);
					}else
					{
						me->GetMotionMaster()->MovePoint(3, POS_3v3_RANDOM_SCARLET_MINION_MOVETO_right);
					}
				}
				if (me->FindNearestCreature(check_scarlet_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(4, POS_3v3_RIGHT_SCARLET_MINION_MOVETO_GUARDIAN);
				}
				if (me->FindNearestCreature(check_scarlet_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(5, POS_3v3_LEFT_SCARLET_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};
	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Scarlet_Minion3v3_MIDDLE_LANEAI(pCreature);
	}
};

class Battle_Game_Undead_Minion3v3_MIDDLE_LANE : public CreatureScript
{
public:
	Battle_Game_Undead_Minion3v3_MIDDLE_LANE() : CreatureScript("Battle_Game_Undead_Minion3v3_MIDDLE_LANE") { }
	struct Battle_Game_Undead_Minion3v3_MIDDLE_LANEAI : public ScriptedAI
	{
		Battle_Game_Undead_Minion3v3_MIDDLE_LANEAI(Creature * c) : ScriptedAI(c) { }
		Player* SelNearPlr = me->SelectNearestPlayer(2000.0f);
		int moveCount;
		void Reset()
		{
			moveCount = 0;
			//me->SetSpeed(MOVE_WALK, 1.8f, true);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type != POINT_MOTION_TYPE)
				return;
			moveCount++;
		}

		void UpdateAI(uint32 const diff)
		{
			if (!me->isInCombat() && !me->isMoving())
			{
				if (!me->FindNearestCreature(check_undead_minion_pathing_middle, 10, true))
				{
					me->GetMotionMaster()->MovePoint(1, POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_random);
				}
				if (me->FindNearestCreature(check_undead_minion_pathing_middle, 10, true))
				{
					if (roll_chance_i(50))
					{
						me->GetMotionMaster()->MovePoint(2, POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_left);
					}
					else
					{
						me->GetMotionMaster()->MovePoint(3, POS_3v3_RANDOM_UNDEAD_MINION_MOVETO_right);
					}
				}
				if (me->FindNearestCreature(check_undead_minion_pathing_right, 10, true))
				{
					me->GetMotionMaster()->MovePoint(4, POS_3v3_RIGHT_UNDEAD_MINION_MOVETO_GUARDIAN);
				}
				if (me->FindNearestCreature(check_undead_minion_pathing_left, 10, true))
				{
					me->GetMotionMaster()->MovePoint(5, POS_3v3_LEFT_UNDEAD_MINION_MOVETO_GUARDIAN);
				}
			}
			ScriptedAI::UpdateAI(diff);
		}
	};

	CreatureAI * GetAI(Creature * pCreature) const
	{
		return new Battle_Game_Undead_Minion3v3_MIDDLE_LANEAI(pCreature);
	}
};

void AddSC_npc_battlegame()
{
	//Guardian battles
	new Battle_Game_master3v3();
	//Lane AI scripts
	new Battle_Game_Scarlet_Minion3v3_RIGHT_LANE();
	new Battle_Game_Undead_Minion3v3_RIGHT_LANE();
	new Battle_Game_Scarlet_Minion3v3_LEFT_LANE();
	new Battle_Game_Undead_Minion3v3_LEFT_LANE();
	new Battle_Game_Scarlet_Minion3v3_MIDDLE_LANE();
	new Battle_Game_Undead_Minion3v3_MIDDLE_LANE();
}