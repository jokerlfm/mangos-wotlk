#include "NierEntity.h"

#include "NierConfig.h"
#include "NierManager.h"
#include "NierScripts/NierScript_Base.h"
#include "NierScripts/NierScript_Druid.h"
#include "NierScripts/NierScript_Hunter.h"
#include "NierScripts/NierScript_Mage.h"
#include "NierScripts/NierScript_Paladin.h"
#include "NierScripts/NierScript_Priest.h"
#include "NierScripts/NierScript_Rogue.h"
#include "NierScripts/NierScript_Shaman.h"
#include "NierScripts/NierScript_Warlock.h"
#include "NierScripts/NierScript_Warrior.h"
#include "Accounts/AccountMgr.h"
#include "World/World.h"

NierEntity::NierEntity()
{
	updateDelay = 0;

	me = nullptr;
	entry = 0;
	master_id = 0;
	account_id = 0;
	account_name = "";
	character_id = 0;
	target_level = 0;
	target_specialty = 0;
	entityState = NierState::NierState_OffLine;

	gametime = 0;
}

void NierEntity::Update()
{
	uint32 diff = WorldTimer::getMSTime() - gametime;
	gametime = gametime + diff;
	if (updateDelay > 0)
	{
		updateDelay -= diff;
		return;
	}
	switch (entityState)
	{
	case NierState::NierState_None:
	{
		updateDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
		break;
	}
	case NierState::NierState_OffLine:
	{
		break;
	}
	case NierState::NierState_Enter:
	{
		updateDelay = 1000;
		entityState = NierState::NierState_CheckAccount;
		sLog.outBasic("Nier %d - %d is ready to go online.", entry, account_id);
		break;
	}
	case NierState::NierState_CheckAccount:
	{
		updateDelay = 1000;
		if (account_id > 0)
		{
			std::ostringstream queryStream;
			queryStream << "SELECT id FROM account where id = " << account_id;
			auto accountQR = LoginDatabase.Query(queryStream.str().c_str());
			if (accountQR)
			{
				sLog.outBasic("Nier %d is ready.", account_id);
				updateDelay = 1000;
				entityState = NierState::NierState_CheckCharacter;
				break;
			}
		}
		else
		{
			std::ostringstream accountNameStream;
			accountNameStream << NIER_MARK << entry;
			std::ostringstream queryStream;
			queryStream << "SELECT id FROM account where username = '" << accountNameStream.str() << "'";
			auto accountQR = LoginDatabase.Query(queryStream.str().c_str());
			if (accountQR)
			{
				Field* fields = accountQR->Fetch();
				account_id = fields[0].GetUInt32();
				if (account_id > 0)
				{
					sLog.outBasic("Nier %d is ready.", account_id);
					std::ostringstream updateStream;
					updateStream << "update nier set account_id = " << account_id << " where entry = " << entry;
					CharacterDatabase.DirectExecute(updateStream.str().c_str());
					entityState = NierState::NierState_CheckCharacter;
					break;
				}
			}
			entityState = NierState::NierState_CreateAccount;
		}
		break;
	}
	case NierState::NierState_CreateAccount:
	{
		updateDelay = 1000;
		std::ostringstream accountNameStream;
		accountNameStream << NIER_MARK << entry;

		if (sAccountMgr.CreateAccount(accountNameStream.str(), NIER_MARK) == AccountOpResult::AOR_OK)
		{
			entityState = NierState::NierState_CheckAccount;
		}
		else
		{
			sLog.outBasic("Nier entry %d account creation failed.", entry);
			entityState = NierState::NierState_None;
		}
		break;
	}
	case NierState::NierState_CheckCharacter:
	{
		updateDelay = 1000;
		std::ostringstream queryStream;
		queryStream << "SELECT guid FROM characters where account = " << account_id;
		auto characterQR = CharacterDatabase.Query(queryStream.str().c_str());
		if (characterQR)
		{
			Field* characterFields = characterQR->Fetch();
			character_id = characterFields[0].GetUInt32();
			if (character_id > 0)
			{
				sLog.outBasic("Nier account_id %d character_id %d is ready.", account_id, character_id);
				WorldSession* loginSession = sWorld.FindSession(account_id);
				if (!loginSession)
				{
					std::ostringstream accountNameStream;
					accountNameStream << NIER_MARK << entry;
					loginSession = new WorldSession(account_id, nullptr, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, accountNameStream.str(), 0, false, false);
					loginSession->isNier = true;
					loginSession->SetNoAnticheat();
					sWorld.AddSession(loginSession);
				}
				entityState = NierState::NierState_DoEnum;
				break;
			}
		}
		sLog.outBasic("Nier account_id %d character_id is not ready.", account_id);
		entityState = NierState::NierState_CreateCharacter;
		break;
	}
	case NierState::NierState_CreateCharacter:
	{
		updateDelay = 5000;
		std::string currentName = "";
		bool nameValid = false;
		uint32 nameIndex = urand(0, sizeof(sNierManager->nierNameMap));
		currentName = sNierManager->nierNameMap[nameIndex];
		std::ostringstream queryStream;
		queryStream << "SELECT count(*) FROM characters where name = '" << currentName << "'";
		auto checkNameQR = CharacterDatabase.Query(queryStream.str().c_str());
		if (!checkNameQR)
		{
			nameValid = true;
			sLog.outBasic("Name %s is available", currentName.c_str());
		}
		else
		{
			Field* nameCountFields = checkNameQR->Fetch();
			uint32 nameCount = nameCountFields[0].GetUInt32();
			if (nameCount == 0)
			{
				nameValid = true;
				sLog.outBasic("Name %s is available", currentName.c_str());
			}
			else
			{
				sLog.outBasic("Name %s is unavailable", currentName.c_str());
				updateDelay = 2000;
				break;
			}
		}

		if (!nameValid)
		{
			sLog.outBasic("No available names");
			entityState = NierState::NierState_None;
			return;
		}
		uint8 gender = 0, skin = 0, face = 0, hairStyle = 0, hairColor = 0, facialHair = 0;
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

		std::ostringstream accountNameStream;
		accountNameStream << NIER_MARK << entry;
		WorldSession* createSession = new WorldSession(account_id, nullptr, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, accountNameStream.str(), 0, false, false);
		Player* newPlayer = new Player(createSession);
		if (!newPlayer->Create(sObjectMgr.GeneratePlayerLowGuid(), currentName, target_race, target_class, gender, skin, face, hairStyle, hairColor, facialHair, 0))
		{
			newPlayer->CleanupsBeforeDelete();
			delete createSession;
			delete newPlayer;
			sLog.outError("Character create failed, %s %d %d ", currentName.c_str(), target_race, target_class);
			sLog.outBasic("Try again");
			break;
		}
		newPlayer->GetMotionMaster()->Initialize();
		newPlayer->setCinematic(2);
		newPlayer->SetAtLoginFlag(AT_LOGIN_NONE);
		newPlayer->SaveToDB();
		character_id = newPlayer->GetGUIDLow();
		createSession->SetNoAnticheat();
		createSession->isNier = true;
		sWorld.AddSession(createSession);
		std::ostringstream replyStream;
		replyStream << "nier character created : account - " << account_id << " character - " << newPlayer->GetGUIDLow() << " " << currentName;
		std::string replyString = replyStream.str();
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
		sLog.outBasic(replyString.c_str());
		std::ostringstream udpateStream;
		udpateStream << "update nier set character_id = " << character_id << " where entry = " << entry;
		CharacterDatabase.DirectExecute(udpateStream.str().c_str());
		entityState = NierState::NierState_CheckCharacter;
		break;
	}
	case NierState::NierState_DoEnum:
	{
		updateDelay = urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS);
		WorldSession* loginSession = sWorld.FindSession(account_id);
		if (loginSession)
		{
			//loginSession->SetNoAnticheat();
			loginSession->isNier = true;
			sLog.outBasic("Enum character %d %d ", account_id, character_id);
			updateDelay = urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS);
			entityState = NierState::NierState_DoLogin;
		}
		break;
	}
	case NierState::NierState_CheckEnum:
	{
		updateDelay = 5000;
		entityState = NierState::NierState_DoLogin;
		break;
	}
	case NierState::NierState_DoLogin:
	{
		updateDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
		WorldSession* loginSession = sWorld.FindSession(account_id);
		if (loginSession)
		{
			//loginSession->SetNoAnticheat();
			loginSession->isNier = true;
			WorldPacket wpLogin(CMSG_PLAYER_LOGIN, 16);
			ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
			wpLogin << playerGuid;
			loginSession->HandlePlayerLoginOpcode(wpLogin);
			std::ostringstream replyStream;
			replyStream << "log in character : account - " << account_id << " character - " << character_id;
			std::string replyString = replyStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
			sLog.outBasic(replyString.c_str());
			entityState = NierState::NierState_CheckLogin;
		}
		else
		{
			entityState = NierState::NierState_None;
		}
		break;
	}
	case NierState::NierState_CheckLogin:
	{
		updateDelay = 2000;
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* mePlayer = ObjectAccessor::FindPlayer(playerGuid))
		{
			me = mePlayer;
			if (me->IsInWorld())
			{
				me->isNier = true;
				NierScript_Base* nierScript = nullptr;
				uint32 playerClass = me->getClass();
				switch (playerClass)
				{
				case 1:
				{
					nierScript = new NierScript_Warrior(me);
					break;
				}
				case 2:
				{
					nierScript = new NierScript_Paladin(me);
					break;
				}
				case 3:
				{
					nierScript = new NierScript_Hunter(me);
					break;
				}
				case 4:
				{
					nierScript = new NierScript_Rogue(me);
					break;
				}
				case 5:
				{
					nierScript = new NierScript_Priest(me);
					break;
				}
				case 7:
				{
					nierScript = new NierScript_Shaman(me);
					break;
				}
				case 8:
				{
					nierScript = new NierScript_Mage(me);
					break;
				}
				case 9:
				{
					nierScript = new NierScript_Warlock(me);
					break;
				}
				case 11:
				{
					nierScript = new NierScript_Druid(me);
					break;
				}
				default:
				{
					nierScript = new NierScript_Base(me);
					break;
				}
				}
				nierScript->masterId = master_id;
				nierScript->specialty = target_specialty;
				me->_nierScript = nierScript;
				//sCharacterCache->AddCharacterCacheEntry(me->GetGUID(), account_id, me->GetName(), me->getGender(), me->getRace(), me->getClass(), me->getLevel());
				std::ostringstream replyStream;
				replyStream << "nier character logged in : account - " << account_id << " character - " << character_id;
				std::string replyString = replyStream.str();
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
				sLog.outBasic(replyString.c_str());
				entityState = NierState::NierState_Initialize;
				break;
			}
		}
		break;
	}
	case NierState::NierState_Initialize:
	{
		updateDelay = 2000;
		if (me->IsInWorld())
		{
			uint32 rawLevel = me->GetLevel();
			target_level = 20;
			ObjectGuid masterGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, master_id);
			if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
			{
				target_level = master->GetLevel();
			}
			if (target_level < 10)
			{
				target_level = 10;
			}
			me->_nierScript->InitializeCharacter();
			std::ostringstream replyStream;
			replyStream << "nier initialized : account - " << account_id << " character - " << character_id << " " << me->GetName();
			std::string replyString = replyStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
			sLog.outBasic(replyString.c_str());
			if (rawLevel > 1)
			{
				entityState = NierState::NierState_Equip;
			}
			else
			{
				entityState = NierState::NierState_Upgrade;
			}
			break;
		}
		entityState = NierState::NierState_OffLine;
		break;
	}
	case NierState::NierState_Equip:
	{
		updateDelay = 2000;
		if (me->IsInWorld())
		{
			me->_nierScript->InitializeEquipments();
			me->_nierScript->Relocate();
			entityState = NierState::NierState_Online;
			std::ostringstream msgStream;
			msgStream << me->GetName() << " Equiped all slots";
			std::string msgStr = msgStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStr.c_str());
			sLog.outBasic(msgStr.c_str());
			break;
		}
		break;
	}
	case NierState::NierState_LevelUp:
	{
		updateDelay = 1000;
		if (me->IsInWorld())
		{
			ObjectGuid masterGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, master_id);
			if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
			{
				target_level = master->GetLevel();
			}
			if (target_level < 10)
			{
				target_level = 10;
			}
			me->_nierScript->InitializeCharacter();
			std::ostringstream replyStream;
			replyStream << "nier level up : account - " << account_id << " character - " << character_id << " " << me->GetName();
			std::string replyString = replyStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
			sLog.outBasic(replyString.c_str());
			entityState = NierState::NierState_Upgrade;
			break;
		}
		entityState = NierState::NierState_OffLine;
		break;
	}
	case NierState::NierState_Upgrade:
	{
		updateDelay = 1000;
		if (me->IsInWorld())
		{
			me->_nierScript->InitializeEquipments(true);
			entityState = NierState::NierState_Online;
			std::ostringstream msgStream;
			msgStream << me->GetName() << " upgrade";
			std::string msgStr = msgStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStr.c_str());
			sLog.outBasic(msgStr.c_str());
			break;
		}
		break;
	}
	case NierState::NierState_Online:
	{
		if (me->IsInWorld())
		{
			if (me->_nierScript)
			{
				me->_nierScript->Update(diff);
			}
			ObjectGuid masterGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, master_id);
			if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
			{
				if (master->GetLevel() != me->GetLevel())
				{
					updateDelay = urand(1000, 10000);
					entityState = NierState::NierState_LevelUp;
				}
			}
		}
		break;
	}
	case NierState::NierState_Exit:
	{
		if (me->IsInWorld())
		{
			if (me->GetGroup())
			{
				entityState = NierState::NierState_Online;
				break;
			}
		}
		sLog.outBasic("Nier entry %d is ready to go offline.", entry);
		entityState = NierState::NierState_DoLogoff;
		break;
	}
	case NierState::NierState_DoLogoff:
	{
		updateDelay = 5000;
		if (me->IsInWorld())
		{
			if (WorldSession* ws = me->GetSession())
			{
				me->RemoveFromGroup();
				ws->LogoutPlayer();
				entityState = NierState::NierState_CheckLogoff;
				break;
			}
		}
		entityState = NierState::NierState_OffLine;
		break;
	}
	case NierState::NierState_CheckLogoff:
	{
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
		{
			sLog.outError("Log out nier %s failed", me->GetName());
			entityState = NierState::NierState_None;
			break;
		}
		entityState = NierState::NierState_OffLine;
		break;
	}
	case NierState::NierState_RedoLogin:
	{
		entityState = NierState::NierState_OffLine;
		break;
	}
	case NierState::NierState_CheckRedoLogin:
	{
		entityState = NierState::NierState_OffLine;
		break;
	}
	default:
	{
		updateDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
		break;
	}
	}
}