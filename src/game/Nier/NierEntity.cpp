#include "NierEntity.h"
#include "NierManager.h"
#include "NierConfig.h"
#include "NierStrategies/NierStrategy_Base.h"
#include "NierActions/NierAction_Base.h"
#include "NierActions/NierAction_Druid.h"
#include "NierActions/NierAction_Hunter.h"
#include "NierActions/NierAction_Mage.h"
#include "NierActions/NierAction_Paladin.h"
#include "NierActions/NierAction_Priest.h"
#include "NierActions/NierAction_Rogue.h"
#include "NierActions/NierAction_Shaman.h"
#include "NierActions/NierAction_Warlock.h"
#include "NierActions/NierAction_Warrior.h"

#include "Accounts/AccountMgr.h"
#include "Groups/Group.h"
#include "World/World.h"

NierEntity::NierEntity()
{
	nier_id = 0;
	master_id = 0;
	account_id = 0;	
	character_id = 0;
	target_level = 0;
	target_specialty = 0;
	group_role = 0;
	nier_type = 0;
	checkDelay = 5 * IN_MILLISECONDS;
	entityState = NierEntityState::NierEntityState_OffLine;	

	nierPlayer = nullptr;
	action = nullptr;
}

void NierEntity::Update(uint32 pmDiff)
{
	if (checkDelay >= 0)
	{
		checkDelay -= pmDiff;
		return;
	}
	switch (entityState)
	{
	case NierEntityState::NierEntityState_Online:
	{
		AITick(pmDiff);
		break;
	}
	case NierEntityState::NierEntityState_None:
	{
		checkDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
		break;
	}
	case NierEntityState::NierEntityState_OffLine:
	{
		break;
	}
	case NierEntityState::NierEntityState_Enter:
	{
		entityState = NierEntityState::NierEntityState_CheckAccount;
		sLog.outBasic("Nier %d is ready to go online.", nier_id);
		break;
	}
	case NierEntityState::NierEntityState_CheckAccount:
	{
		if (account_id > 0)
		{
			entityState = NierEntityState::NierEntityState_CheckCharacter;
			checkDelay = 2000;
			break;
		}
		else
		{
			std::ostringstream accountNameStream;
			accountNameStream << NIER_MARK << nier_id;
			std::string accountName = accountNameStream.str();
			AccountOpResult aor = sAccountMgr.CreateAccount(accountName, NIER_MARK);
			if (aor == AccountOpResult::AOR_OK)
			{
				account_id = sAccountMgr.GetId(accountName);
				std::ostringstream sqlStream;
				sqlStream << "update nier set account_id = " << account_id << " where entry = " << nier_id;
				std::string sql = sqlStream.str();
				CharacterDatabase.DirectExecute(sql.c_str());
				entityState = NierEntityState::NierEntityState_CheckCharacter;
				sLog.outError("Nier %d account created : %s", nier_id, accountName);
				break;
			}
			else
			{
				entityState = NierEntityState::NierEntityState_None;
				sLog.outError("Nier %d account create error : %s - %d", nier_id, accountName, aor);
				break;
			}
		}
		break;
	}
	case NierEntityState::NierEntityState_CheckCharacter:
	{
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
				entityState = NierEntityState::NierEntityState_DoEnum;
				checkDelay = 1000;
				break;
			}
		}
		sLog.outBasic("Nier account_id %d character_id is not ready.", account_id);
		entityState = NierEntityState::NierEntityState_CreateCharacter;
		break;
	}
	case NierEntityState::NierEntityState_CreateCharacter:
	{
		std::string currentName = "";
		bool nameValid = false;
		uint32 nameIndex = urand(0, sizeof(sNierManager->nierNameMap));
		currentName = sNierManager->nierNameMap[nameIndex];
		std::ostringstream queryStream;
		queryStream << "SELECT count(*) FROM characters where name = '" << currentName << "'";
		auto checkNameQR = CharacterDatabase.Query(queryStream.str().c_str());
		if (!checkNameQR)
		{
			sLog.outBasic("Name %s is available", currentName.c_str());
		}
		else
		{
			Field* nameCountFields = checkNameQR->Fetch();
			uint32 nameCount = nameCountFields[0].GetUInt32();
			if (nameCount == 0)
			{
				sLog.outBasic("Name %s is available", currentName.c_str());
			}
			else
			{
				sLog.outError("Name %s is unavailable, try again", currentName.c_str());
				checkDelay = 2000;
				break;
			}
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
		accountNameStream << NIER_MARK << nier_id;
		std::string accountName = accountNameStream.str();

		WorldSession* createSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, accountName, 0, 0, false);
		Player* newPlayer = new Player(createSession);
		if (!newPlayer->Create(sObjectMgr.GeneratePlayerLowGuid(), currentName, target_race, target_class, gender, skin, face, hairStyle, hairColor, facialHair, 0))
		{
			newPlayer->CleanupsBeforeDelete();
			delete createSession;
			delete newPlayer;
			sLog.outError("Character create failed, %s %d %d ", currentName.c_str(), target_race, target_class);
			sLog.outBasic("Try again");
			checkDelay = 2000;
			break;
		}
		newPlayer->GetMotionMaster()->Initialize();
		newPlayer->setCinematic(2);
		newPlayer->SetAtLoginFlag(AT_LOGIN_NONE);
		newPlayer->SaveToDB();
		character_id = newPlayer->GetObjectGuid().GetCounter();
		if (character_id > 0)
		{
			createSession->isNier = true;
			sWorld.AddSession(createSession);
			std::ostringstream replyStream;
			replyStream << "nier character created : account - " << account_id << " character - " << newPlayer->GetObjectGuid().GetCounter() << " " << currentName;
			std::string replyString = replyStream.str();
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
			sLog.outBasic(replyString.c_str());


			std::ostringstream sqlStream;
			sqlStream << "update nier set character_id = " << character_id << " where entry = " << nier_id;
			std::string sql = sqlStream.str();
			CharacterDatabase.DirectExecute(sql.c_str());
			entityState = NierEntityState::NierEntityState_CheckCharacter;
			break;
		}

		entityState = NierEntityState::NierEntityState_None;
		break;
	}
	case NierEntityState::NierEntityState_DoEnum:
	{
		WorldSession* loginSession = sWorld.FindSession(account_id);
		if (!loginSession)
		{
			std::ostringstream accountNameStream;
			accountNameStream << NIER_MARK << nier_id;
			std::string accountName = accountNameStream.str();

			loginSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, accountName, 0, 0, false);
			sWorld.AddSession(loginSession);
		}
		loginSession->isNier = true;
		WorldPacket wpEnum(CMSG_CHAR_ENUM, 4);
		loginSession->HandleCharEnumOpcode(wpEnum);
		sLog.outBasic("Enum character %d %d ", account_id, character_id);
		checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
		entityState = NierEntityState::NierEntityState_CheckEnum;
		checkDelay = 2000;
		break;
	}
	case NierEntityState::NierEntityState_CheckEnum:
	{
		entityState = NierEntityState::NierEntityState_DoLogin;
		checkDelay = urand(2 * IN_MILLISECONDS, 4 * IN_MILLISECONDS);
		break;
	}
	case NierEntityState::NierEntityState_DoLogin:
	{
		WorldSession* loginSession = sWorld.FindSession(account_id);
		if (!loginSession)
		{
			std::ostringstream accountNameStream;
			accountNameStream << NIER_MARK << nier_id;
			std::string accountName = accountNameStream.str();
			loginSession = new WorldSession(account_id, NULL, AccountTypes::SEC_PLAYER, 2, 0, LocaleConstant::LOCALE_enUS, accountName, 0, 0, false);
			sWorld.AddSession(loginSession);
		}
		loginSession->isNier = true;
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
		entityState = NierEntityState::NierEntityState_CheckLogin;
		checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
		break;
	}
	case NierEntityState::NierEntityState_CheckLogin:
	{
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
		{
			if (me->IsInWorld())
			{
				std::ostringstream replyStream;
				replyStream << "nier character logged in : account - " << account_id << " character - " << character_id;
				std::string replyString = replyStream.str();
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
				sLog.outBasic(replyString.c_str());
				entityState = NierEntityState::NierEntityState_Initialize;
				checkDelay = 2000;
				break;
			}
		}
		checkDelay = urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
		break;
	}
	case NierEntityState::NierEntityState_Initialize:
	{
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
		{
			if (me->IsInWorld())
			{
				me->isNier = true;
				switch (target_class)
				{
				case Classes::CLASS_DEATH_KNIGHT:
				{
					action = new NierAction_Base(me);
					break;
				}
				case Classes::CLASS_DRUID:
				{
					action = new NierAction_Druid(me);
					break;
				}
				case Classes::CLASS_HUNTER:
				{
					action = new NierAction_Hunter(me);
					break;
				}
				case Classes::CLASS_MAGE:
				{
					action = new NierAction_Mage(me);
					break;
				}
				case Classes::CLASS_PALADIN:
				{
					action = new NierAction_Paladin(me);
					break;
				}
				case Classes::CLASS_PRIEST:
				{
					action = new NierAction_Priest(me);
					break;
				}
				case Classes::CLASS_ROGUE:
				{
					action = new NierAction_Rogue(me);
					break;
				}
				case Classes::CLASS_SHAMAN:
				{
					action = new NierAction_Shaman(me);
					break;
				}
				case Classes::CLASS_WARLOCK:
				{
					action = new NierAction_Warlock(me);
					break;
				}
				case Classes::CLASS_WARRIOR:
				{
					action = new NierAction_Warrior(me);
					break;
				}
				default:
				{
					action = new NierAction_Base(me);
					break;
				}
				}
				action->InitializeCharacter(target_level, target_specialty);
				std::ostringstream replyStream;
				replyStream << "nier initialized : account - " << account_id << " character - " << character_id << " " << me->GetName();
				std::string replyString = replyStream.str();
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyString.c_str());
				sLog.outBasic(replyString.c_str());
				entityState = NierEntityState::NierEntityState_Online;
				checkDelay = 2000;
				break;
			}
		}
		entityState = NierEntityState::NierEntityState_OffLine;
		break;
	}
	case NierEntityState::NierEntityState_Exit:
	{
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
		{
			if (me->IsInWorld())
			{
				if (me->GetGroup())
				{
					entityState = NierEntityState::NierEntityState_Online;
					break;
				}
			}
		}
		sLog.outBasic("Nier %d is ready to go offline.", nier_id);
		entityState = NierEntityState::NierEntityState_DoLogoff;
		break;
	}
	case NierEntityState::NierEntityState_DoLogoff:
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
					entityState = NierEntityState::NierEntityState_CheckLogoff;
					break;
				}
			}
		}
		entityState = NierEntityState::NierEntityState_OffLine;
		break;
	}
	case NierEntityState::NierEntityState_CheckLogoff:
	{
		ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, character_id);
		if (Player* me = ObjectAccessor::FindPlayer(playerGuid))
		{
			sLog.outError("Log out nier %s failed", me->GetName());
			entityState = NierEntityState::NierEntityState_None;
			break;
		}
		entityState = NierEntityState::NierEntityState_OffLine;
		break;
	}
	default:
	{
		checkDelay = urand(5 * MINUTE * IN_MILLISECONDS, 10 * MINUTE * IN_MILLISECONDS);
		break;
	}
	}
}

void NierEntity::AITick(uint32 pmDiff)
{

}