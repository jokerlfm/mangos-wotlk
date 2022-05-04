#include "NingerEntity.h"
#include "NingerManager.h"
#include "NingerConfig.h"
#include "NingerStrategy_Base.h"
#include "NingerAction_Base.h"
#include "NingerAction_Druid.h"
#include "NingerAction_Hunter.h"
#include "NingerAction_Mage.h"
#include "NingerAction_Paladin.h"
#include "NingerAction_Priest.h"
#include "NingerAction_Rogue.h"
#include "NingerAction_Shaman.h"
#include "NingerAction_Warlock.h"
#include "NingerAction_Warrior.h"

#include "Accounts/AccountMgr.h"
#include "Groups/Group.h"
#include "World/World.h"

NingerEntity::NingerEntity()
{
	ninger_id = 0;
	account_id = 0;
	account_name = "";
	character_id = 0;
	target_level = 0;
	target_specialty = 0;
	checkDelay = 5 * IN_MILLISECONDS;
	entityState = NingerEntityState::NingerEntityState_OffLine;
	offlineDelay = 0;
}

void NingerEntity::Update(uint32 pmDiff)
{
	if (offlineDelay >= 0)
	{
		offlineDelay -= pmDiff;
	}
	if (checkDelay >= 0)
	{
		checkDelay -= pmDiff;
	}
	if (checkDelay < 0)
	{
		checkDelay = urand(2 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
		switch (entityState)
		{
		case NingerEntityState::NingerEntityState_None:
		{
			checkDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
			break;
		}
		case NingerEntityState::NingerEntityState_OffLine:
		{
			break;
		}
		case NingerEntityState::NingerEntityState_Enter:
		{
			entityState = NingerEntityState::NingerEntityState_CheckAccount;
			sLog.outBasic("Ninger %s is ready to go online.", account_name.c_str());
			break;
		}
		case NingerEntityState::NingerEntityState_CheckAccount:
		{
			if (account_name.empty())
			{
				entityState = NingerEntityState::NingerEntityState_None;
			}
			else
			{
				uint32 queryAccountId = 0;
				std::ostringstream accountQueryStream;
				accountQueryStream << "SELECT id FROM account where username = '" << account_name << "'";
				QueryResult* ningerAccountQR = LoginDatabase.Query(accountQueryStream.str().c_str());
				if (ningerAccountQR)
				{
					Field* fields = ningerAccountQR->Fetch();
					queryAccountId = fields[0].GetUInt32();
				}
				delete ningerAccountQR;
				if (queryAccountId > 0)
				{
					if (account_id != queryAccountId)
					{
						account_id = queryAccountId;
						std::ostringstream sqlStream;
						sqlStream << "update ninger set account_id = " << account_id << " where ninger_id = " << ninger_id;
						std::string sql = sqlStream.str();
						CharacterDatabase.DirectExecute(sql.c_str());
					}
					sLog.outBasic("Ninger %s is ready.", account_name.c_str());
					entityState = NingerEntityState::NingerEntityState_CheckCharacter;
				}
				else
				{
					sLog.outBasic("Ninger %s is not ready.", account_name.c_str());
					entityState = NingerEntityState::NingerEntityState_CreateAccount;
				}
			}
			break;
		}
		case NingerEntityState::NingerEntityState_CreateAccount:
		{
			if (account_name.empty())
			{
				entityState = NingerEntityState::NingerEntityState_None;
			}
			else
			{
				if (sAccountMgr.CreateAccount(account_name, NINGER_MARK, 2) == AccountOpResult::AOR_OK)
				{
					entityState = NingerEntityState::NingerEntityState_CheckAccount;
				}
				else
				{
					sLog.outBasic("Ninger id %d account creation failed.", ninger_id);
					entityState = NingerEntityState::NingerEntityState_None;
				}
			}
			break;
		}
		case NingerEntityState::NingerEntityState_CheckCharacter:
		{
			std::ostringstream queryStream;
			queryStream << "SELECT guid FROM characters where account = " << account_id;
			QueryResult* characterQR = CharacterDatabase.Query(queryStream.str().c_str());
			if (characterQR)
			{
				Field* characterFields = characterQR->Fetch();
				character_id = characterFields[0].GetUInt32();
				if (character_id > 0)
				{
					sLog.outBasic("Ninger account_id %d character_id %d is ready.", account_id, character_id);
					//entityState = NingerEntityState::NingerEntityState_DoEnum;
					entityState = NingerEntityState::NingerEntityState_DoLogin;
					break;
				}
			}
			delete characterQR;
			sLog.outBasic("Ninger account_id %d character_id is not ready.", account_id);
			entityState = NingerEntityState::NingerEntityState_CreateCharacter;
			break;
		}
		case NingerEntityState::NingerEntityState_CreateCharacter:
		{
			std::string currentName = "";
			bool nameValid = false;
			while (sNingerManager->ningerNameMap.find(sNingerManager->nameIndex) != sNingerManager->ningerNameMap.end())
			{
				currentName = sNingerManager->ningerNameMap[sNingerManager->nameIndex];
				std::ostringstream queryStream;
				queryStream << "SELECT count(*) FROM characters where name = '" << currentName << "'";
				QueryResult* checkNameQR = CharacterDatabase.Query(queryStream.str().c_str());
				if (!checkNameQR)
				{
					sLog.outBasic("Name %s is available", currentName.c_str());
					nameValid = true;
				}
				else
				{
					Field* nameCountFields = checkNameQR->Fetch();
					uint32 nameCount = nameCountFields[0].GetUInt32();
					if (nameCount == 0)
					{
						nameValid = true;
					}
				}
				delete checkNameQR;
				sNingerManager->nameIndex++;
				if (nameValid)
				{
					break;
				}
			}
			if (!nameValid)
			{
				sLog.outError("No available names");
				entityState = NingerEntityState::NingerEntityState_None;
				return;
			}
			uint8 gender = 0, skin = 0, face = 0, hairStyle = 0, hairColor = 0, facialHair = 0;
			while (true)
			{
				gender = urand(0, 100);
				if (gender < 50)
				{
					gender = 0;
				}
				else
				{
					gender = 1;
				}
				face = urand(0, 5);
				hairStyle = urand(0, 5);
				hairColor = urand(0, 5);
				facialHair = urand(0, 5);

				WorldSession* createSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, account_name.c_str(), 0, 0, false);
				Player* newPlayer = new Player(createSession);
				if (!newPlayer->Create(sObjectMgr.GeneratePlayerLowGuid(), currentName, target_race, target_class, gender, skin, face, hairStyle, hairColor, facialHair, 0))
				{
					newPlayer->CleanupsBeforeDelete();
					delete createSession;
					delete newPlayer;
					sLog.outError("Character create failed, %s %d %d ", currentName.c_str(), target_race, target_class);
					sLog.outBasic("Try again");
					continue;
				}
				newPlayer->GetMotionMaster()->Initialize();
				newPlayer->setCinematic(2);
				newPlayer->SetAtLoginFlag(AT_LOGIN_NONE);
				newPlayer->SaveToDB();
				character_id = newPlayer->GetObjectGuid().GetCounter();
				if (character_id > 0)
				{
					createSession->isNinger = true;
					sWorld.AddSession(createSession);
					std::ostringstream replyStream;
					replyStream << "ninger character created : account - " << account_id << " character - " << newPlayer->GetObjectGuid().GetCounter() << " " << currentName;
					std::string replyString = replyStream.str();
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
					sLog.outBasic(replyString.c_str());
					break;
				}
			}
			if (character_id > 0)
			{
				std::ostringstream sqlStream;
				sqlStream << "update ninger set character_id = " << character_id << " where ninger_id = " << ninger_id;
				std::string sql = sqlStream.str();
				CharacterDatabase.DirectExecute(sql.c_str());
				entityState = NingerEntityState::NingerEntityState_CheckCharacter;
				break;
			}
			entityState = NingerEntityState::NingerEntityState_None;
			break;
		}
		case NingerEntityState::NingerEntityState_DoEnum:
		{
			WorldSession* loginSession = sWorld.FindSession(account_id);
			if (!loginSession)
			{
				loginSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, account_name.c_str(), 0, 0, false);
				sWorld.AddSession(loginSession);
			}
			loginSession->isNinger = true;
			WorldPacket wpEnum(CMSG_CHAR_ENUM, 4);
			loginSession->HandleCharEnumOpcode(wpEnum);
			sLog.outBasic("Enum character %d %d ", account_id, character_id);
			checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
			entityState = NingerEntityState::NingerEntityState_CheckEnum;
			break;
		}
		case NingerEntityState::NingerEntityState_CheckEnum:
		{
			checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
			break;
		}
		case NingerEntityState::NingerEntityState_DoLogin:
		{
			WorldSession* loginSession = sWorld.FindSession(account_id);
			if (!loginSession)
			{
				loginSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, account_name.c_str(), 0, 0, false);
				sWorld.AddSession(loginSession);
			}
			loginSession->isNinger = true;
			WorldPacket wpLogin(CMSG_PLAYER_LOGIN, 16);
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			wpLogin << playerGuid;
			BigNumber N;
			loginSession->InitializeAnticheat(N);
			loginSession->HandlePlayerLoginOpcode(wpLogin);
			std::ostringstream replyStream;
			replyStream << "log in character : account - " << account_id << " character - " << character_id;
			std::string replyString = replyStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
			sLog.outBasic(replyString.c_str());
			checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
			entityState = NingerEntityState::NingerEntityState_CheckLogin;
			break;
		}
		case NingerEntityState::NingerEntityState_CheckLogin:
		{
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				if (me->IsInWorld())
				{
					std::ostringstream replyStream;
					replyStream << "ninger character logged in : account - " << account_id << " character - " << character_id;
					std::string replyString = replyStream.str();
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
					sLog.outBasic(replyString.c_str());
					entityState = NingerEntityState::NingerEntityState_Initialize;
					break;
				}
			}
			checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
			break;
		}
		case NingerEntityState::NingerEntityState_Initialize:
		{
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				if (me->IsInWorld())
				{
					me->activeStrategyIndex = 0;
					me->strategyMap.clear();
					me->strategyMap[me->activeStrategyIndex] = new NingerStrategy_Base();
					me->strategyMap[me->activeStrategyIndex]->me = me;
					me->strategyMap[me->activeStrategyIndex]->Reset();
					me->strategyMap[STRATEGY_THE_UNDERBOG] = new NingerStrategy_The_Underbog();
					me->strategyMap[STRATEGY_THE_UNDERBOG]->me = me;
					me->strategyMap[STRATEGY_THE_UNDERBOG]->Reset();
					switch (target_class)
					{
					case Classes::CLASS_DEATH_KNIGHT:
					{
						me->ningerAction = new NingerAction_Base();
						break;
					}
					case Classes::CLASS_DRUID:
					{
						me->ningerAction = new NingerAction_Druid();
						break;
					}
					case Classes::CLASS_HUNTER:
					{
						me->ningerAction = new NingerAction_Hunter();
						break;
					}
					case Classes::CLASS_MAGE:
					{
						me->ningerAction = new NingerAction_Mage();
						break;
					}
					case Classes::CLASS_PALADIN:
					{
						me->ningerAction = new NingerAction_Paladin();
						break;
					}
					case Classes::CLASS_PRIEST:
					{
						me->ningerAction = new NingerAction_Priest();
						break;
					}
					case Classes::CLASS_ROGUE:
					{
						me->ningerAction = new NingerAction_Rogue();
						break;
					}
					case Classes::CLASS_SHAMAN:
					{
						me->ningerAction = new NingerAction_Shaman();
						break;
					}
					case Classes::CLASS_WARLOCK:
					{
						me->ningerAction = new NingerAction_Warlock();
						break;
					}
					case Classes::CLASS_WARRIOR:
					{
						me->ningerAction = new NingerAction_Warrior();
						break;
					}
					default:
					{
						me->ningerAction = new NingerAction_Base();
						break;
					}
					}
					me->ningerAction->me = me;
					me->ningerMovement = new NingerMovement(me);
					me->ningerAction->InitializeCharacter(target_level, target_specialty);
					offlineDelay = urand(2 * HOUR * IN_MILLISECONDS, 4 * HOUR * IN_MILLISECONDS);
					std::ostringstream replyStream;
					replyStream << "ninger initialized : account - " << account_id << " character - " << character_id << " " << me->GetName();
					std::string replyString = replyStream.str();
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
					sLog.outBasic(replyString.c_str());
					entityState = NingerEntityState::NingerEntityState_Equip;
					break;
				}
			}
			entityState = NingerEntityState::NingerEntityState_OffLine;
			break;
		}
		case NingerEntityState::NingerEntityState_Equip:
		{
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				if (me->IsInWorld())
				{
					if (me->ningerAction->InitializeEquipments())
					{
						for (std::unordered_map<uint32, NingerStrategy_Base*>::iterator nsbIt = me->strategyMap.begin(); nsbIt != me->strategyMap.end(); nsbIt++)
						{
							if (NingerStrategy_Base* nsb = nsbIt->second)
							{
								nsb->me = me;
								nsb->initialized = true;
							}
						}
						me->strategyMap[me->activeStrategyIndex]->randomTeleportDelay = urand(2 * IN_MILLISECONDS, 20 * IN_MILLISECONDS);
						entityState = NingerEntityState::NingerEntityState_Online;
						std::ostringstream msgStream;
						msgStream << me->GetName() << " Equiped all slots";
						sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
						break;
					}
				}
			}
			offlineDelay = 100;
			break;
		}
		case NingerEntityState::NingerEntityState_Online:
		{
			if (offlineDelay > 0)
			{
				checkDelay = urand(10 * MINUTE * IN_MILLISECONDS, 20 * MINUTE * IN_MILLISECONDS);
				ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
				if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
				{
					if (me->IsInWorld())
					{
						me->ningerAction->Prepare();
						if (Group* myGroup = me->GetGroup())
						{
							if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
							{
								if (leader->GetSession()->isNinger)
								{
									me->RemoveFromGroup();
								}
							}
						}
					}
					else
					{
						entityState = NingerEntityState::NingerEntityState_Exit;
					}
				}
			}
			else
			{
				entityState = NingerEntityState::NingerEntityState_Exit;
			}
			break;
		}
		case NingerEntityState::NingerEntityState_Exit:
		{
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				if (me->IsInWorld())
				{
					if (me->GetGroup())
					{
						entityState = NingerEntityState::NingerEntityState_Online;
						break;
					}
				}
			}
			sLog.outBasic("Ninger %d is ready to go offline.", ninger_id);
			entityState = NingerEntityState::NingerEntityState_DoLogoff;
			break;
		}
		case NingerEntityState::NingerEntityState_DoLogoff:
		{
			checkDelay = urand(10 * IN_MILLISECONDS, 20 * IN_MILLISECONDS);
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				if (me->IsInWorld())
				{
					if (WorldSession* ws = me->GetSession())
					{
						me->RemoveFromGroup();
						ws->LogoutPlayer();
						entityState = NingerEntityState::NingerEntityState_CheckLogoff;
						break;
					}
				}
			}
			entityState = NingerEntityState::NingerEntityState_OffLine;
			break;
		}
		case NingerEntityState::NingerEntityState_CheckLogoff:
		{
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
			{
				sLog.outError("Log out ninger %s failed", me->GetName());
				entityState = NingerEntityState::NingerEntityState_None;
				break;
			}
			entityState = NingerEntityState::NingerEntityState_OffLine;
			break;
		}
		case NingerEntityState::NingerEntityState_RedoLogin:
		{
			entityState = NingerEntityState::NingerEntityState_OffLine;
			break;
		}
		case NingerEntityState::NingerEntityState_CheckRedoLogin:
		{
			entityState = NingerEntityState::NingerEntityState_OffLine;
			break;
		}
		default:
		{
			checkDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
			break;
		}
		}
	}
}
