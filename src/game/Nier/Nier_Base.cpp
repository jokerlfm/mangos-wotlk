#include "Nier_Base.h"

#include "NierConfig.h"
#include "NierManager.h"
#include "Accounts/AccountMgr.h"
#include "Entities/ObjectGuid.h"
#include "World/World.h"
#include "MotionGenerators/PointMovementGenerator.h"
#include "Movement/MoveSpline.h"
#include "Movement/MoveSplineInit.h"

Nier_Base::Nier_Base()
{
	me = nullptr;
	entry = 0;
	master_id = 0;
	account_id = 0;
	account_name = "";
	character_id = 0;
	target_level = 0;
	target_specialty = 0;
	entityState = NierState::NierState_OffLine;
	orderType = OrderType::OrderType_None;
	actionDelay = 0;
	updateDelay = 5 * IN_MILLISECONDS;
	prepareDelay = 5 * IN_MILLISECONDS;
	assembleDelay = 0;
	reviveDelay = 0;
	wanderDelay = 0;
	restDelay = 0;
	drinkDelay = 0;
	interruptDelay = 0;
	helpDelay = 0;
	orderDelay = 0;

	pvpDelay = 0;
	pvp = true;

	combatDuration = 0;

	dpsDistance = CONTACT_DISTANCE;
	followDistance = INTERACTION_DISTANCE;
	chasing = true;
	freezing = false;
}

bool Nier_Base::Prepare()
{
	if (me)
	{
		me->SetPvP(true);
		me->DurabilityRepairAll(false, 0, false);
		if (!me->GetGroup())
		{
			if (me->GetMap()->Instanceable())
			{
				uint32 hbId = 0;
				float hbX = 0.0f;
				float hbY = 0.0f;
				float hbZ = 0.0f;
				me->GetHomebindLocation(hbX, hbY, hbZ, hbId);
				me->TeleportTo(hbId, hbX, hbY, hbZ, 0.0f);
			}
		}
	}

	return true;
}

void Nier_Base::Update(uint32 pDiff)
{
	if (updateDelay >= 0)
	{
		updateDelay -= pDiff;
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
					//loginSession->SetNoAnticheat();
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
		//createSession->SetNoAnticheat();
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
				me->_nier = this;
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
			me->masterId = master_id;
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
			InitializeCharacter();
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
			if (InitializeEquipments())
			{
				entityState = NierState::NierState_Online;
				std::ostringstream msgStream;
				msgStream << me->GetName() << " Equiped all slots";
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
				break;
			}
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
			InitializeCharacter();
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
			if (InitializeEquipments(true))
			{
				entityState = NierState::NierState_Online;
				std::ostringstream msgStream;
				msgStream << me->GetName() << " upgrade";
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
				break;
			}
		}
		break;
	}
	case NierState::NierState_Online:
	{
		Update_Online(pDiff);
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

void Nier_Base::Update_Online(uint32 pDiff)
{
	if (actionDelay > 0)
	{
		actionDelay -= pDiff;
	}
	else
	{
		actionDelay = 200;
	}
	if (wanderDelay > 0)
	{
		wanderDelay -= pDiff;
	}
	if (helpDelay > 0)
	{
		helpDelay -= pDiff;
	}
	if (pvpDelay > 0)
	{
		pvpDelay -= pDiff;
	}
	else
	{
		pvp = !pvp;
		pvpDelay = urand(300000, 2400000);
	}
	if (restDelay > 0)
	{
		restDelay -= pDiff;
		if (drinkDelay > 0)
		{
			drinkDelay -= pDiff;
			if (drinkDelay <= 0)
			{
				Drink();
			}
		}
	}
	if (me->IsInCombat())
	{
		combatDuration += pDiff;
		restDelay = 0;
		drinkDelay = 0;
	}
	else
	{
		if (combatDuration > 0)
		{
			combatDuration = 0;
			ClearTarget();
			//me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_SIT);
			//standDelay = 500;
		}
	}
	if (orderDelay > 0)
	{
		orderDelay -= pDiff;
		switch (orderType)
		{
		case OrderType::OrderType_None:
		{
			orderDelay = 0;
			break;
		}
		case OrderType::OrderType_Tank:
		{
			bool ordering = false;
			if (Group* myGroup = me->GetGroup())
			{
				Unit* skull = ObjectAccessor::GetUnit(*me, myGroup->GetGuidByTargetIcon(7));
				if (Tank(skull))
				{
					ordering = true;
				}
			}
			if (!ordering)
			{
				orderType = OrderType::OrderType_None;
				orderDelay = 0;
			}
			break;
		}
		case OrderType::OrderType_DPS:
		{
			break;
		}
		case OrderType::OrderType_Revive:
		{
			break;
		}
		case OrderType::OrderType_Move:
		{
			break;
		}
		default:
		{
			break;
		}
		}
		return;
	}
	if (interruptDelay > 0)
	{
		interruptDelay -= pDiff;
	}
	if (prepareDelay > 0)
	{
		prepareDelay -= pDiff;
		if (prepareDelay <= 0)
		{
			Prepare();
			return;
			prepareDelay = urand(60000, 12000);
		}
	}
	if (assembleDelay > 0)
	{
		assembleDelay -= pDiff;
		if (assembleDelay <= 0)
		{
			assembleDelay = 0;
			ObjectGuid masterGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, master_id);
			if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
			{
				if (!me->IsAlive())
				{
					reviveDelay = 5000;
					me->Whisper("Arrived. Revive in 5 seconds.", Language::LANG_UNIVERSAL, master->GetObjectGuid());
				}
				else
				{
					me->Whisper("Arrived.", Language::LANG_UNIVERSAL, master->GetObjectGuid());
				}
				me->ClearInCombat();
				me->TeleportTo(master->GetMapId(), master->GetPositionX(), master->GetPositionY(), master->GetPositionZ(), 0);
			}
			return;
		}
	}
	if (reviveDelay > 0)
	{
		reviveDelay -= pDiff;
		if (reviveDelay <= 0)
		{
			reviveDelay = 0;
			if (!me->IsAlive())
			{
				me->ResurrectPlayer(0.2f);
			}
			return;
		}
	}
}

void Nier_Base::InitializeCharacter()
{
	if (!me)
	{
		return;
	}
	if (me->GetLevel() != target_level)
	{
		me->GiveLevel(target_level);
	}
	if (me->GetFreeTalentPoints() == 0)
	{
		return;
	}

	// talents
	me->resetTalents(true);
	me->SendTalentsInfoData(false);
	if (target_class == Classes::CLASS_PRIEST)
	{
		// talent tab : 201 - discipline, 202 - holy, 203 - shadow
		switch (target_specialty)
		{
		case 0:
		{
			// discipline
			LearnTalent(1898);

			LearnTalent(344);
			LearnTalent(352);

			LearnTalent(348);
			LearnTalent(343);
			LearnTalent(347);

			LearnTalent(341);

			LearnTalent(351);
			LearnTalent(1201);

			LearnTalent(1771);
			LearnTalent(1772, 2);

			LearnTalent(322);
			LearnTalent(1772);
			LearnTalent(1773);

			LearnTalent(2235);
			LearnTalent(1896);

			LearnTalent(1774);
			LearnTalent(1894);
			LearnTalent(1901);

			LearnTalent(1202);

			LearnTalent(1897);
			LearnTalent(1895);

			LearnTalent(1858);

			LearnTalent(406);
			LearnTalent(401);

			LearnTalent(1181);

			LearnTalent(442);

			break;
		}
		case 1:
		{
			// holy
			LearnTalent(406);
			LearnTalent(401);
			LearnTalent(1181);
			LearnTalent(361);
			LearnTalent(408);
			LearnTalent(442);
			LearnTalent(1561);
			LearnTalent(402);
			LearnTalent(1766);
			LearnTalent(404);
			LearnTalent(1768);
			LearnTalent(2279);
			LearnTalent(1767);
			LearnTalent(1902);
			LearnTalent(1815);
			LearnTalent(1903);
			LearnTalent(1911);
			break;
		}
		case 2:
		{
			// shadow
			LearnTalent(462);
			LearnTalent(466);
			LearnTalent(482);
			LearnTalent(463);
			LearnTalent(501);
			LearnTalent(542);
			LearnTalent(461);
			LearnTalent(881);
			LearnTalent(541);
			LearnTalent(484);
			LearnTalent(1777);
			LearnTalent(1781);
			LearnTalent(521);
			LearnTalent(2267);
			LearnTalent(1778);
			LearnTalent(1816);
			LearnTalent(1779);
			LearnTalent(1908);
			LearnTalent(1909);
			LearnTalent(1907);
			LearnTalent(1910);

			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (target_class == Classes::CLASS_ROGUE)
	{
		// talent tab : 201 - discipline, 202 - holy, 203 - shadow
		switch (target_specialty)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			// talent tab : 181 - Combat, 182 - Assassination
			LearnTalent(221);
			LearnTalent(181);
			LearnTalent(182);
			LearnTalent(1122);
			LearnTalent(223);
			LearnTalent(201);
			LearnTalent(1827);
			LearnTalent(1703);
			LearnTalent(186);
			LearnTalent(205);
			LearnTalent(1706);
			LearnTalent(206, 1);
			LearnTalent(1705);
			LearnTalent(1825);
			LearnTalent(1709);
			LearnTalent(2074);
			LearnTalent(2075);
			LearnTalent(2076);

			LearnTalent(276);
			LearnTalent(270);
			LearnTalent(273);
			LearnTalent(269);
			LearnTalent(682);
			break;
		}
		case 2:
		{
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (target_class == Classes::CLASS_MAGE)
	{
		switch (target_specialty)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			LearnTalent(37);
			LearnTalent(1649);
			LearnTalent(73, 2);
			LearnTalent(69);
			LearnTalent(73);
			LearnTalent(63);
			LearnTalent(66);
			LearnTalent(38);
			LearnTalent(72);
			LearnTalent(61);
			LearnTalent(1737);
			LearnTalent(68);
			LearnTalent(1738);
			LearnTalent(1740);
			LearnTalent(1853);
			LearnTalent(67, 1);
			LearnTalent(1741);
			LearnTalent(1855);
			LearnTalent(67, 2);
			LearnTalent(1856);
			LearnTalent(1857);
			LearnTalent(1854);
			LearnTalent(67);
			LearnTalent(65);
			LearnTalent(62);
			LearnTalent(741);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (target_class == Classes::CLASS_WARLOCK)
	{
		switch (target_specialty)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			// destruction 
			LearnTalent(944);
			LearnTalent(943);
			LearnTalent(982);
			LearnTalent(941);
			LearnTalent(983);
			LearnTalent(963);
			LearnTalent(967);
			LearnTalent(964);
			LearnTalent(961);
			LearnTalent(981);
			LearnTalent(966);
			LearnTalent(968);
			LearnTalent(986);
			LearnTalent(1677);
			LearnTalent(1888);
			LearnTalent(1676);
			LearnTalent(2045);
			LearnTalent(1890);
			LearnTalent(1891);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (target_class == Classes::CLASS_WARRIOR)
	{
		switch (target_specialty)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			// 163 protection 
			LearnTalent(1601);
			LearnTalent(138);
			LearnTalent(147);
			LearnTalent(153);
			LearnTalent(1654);
			LearnTalent(146);
			LearnTalent(140);
			LearnTalent(152);
			LearnTalent(151);
			LearnTalent(702);
			LearnTalent(1652);
			LearnTalent(148);
			LearnTalent(1660);
			LearnTalent(1653);
			LearnTalent(1893);
			LearnTalent(1666);
			LearnTalent(2236);
			LearnTalent(1871);
			LearnTalent(2246);
			LearnTalent(1872);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (target_class == Classes::CLASS_HUNTER)
	{
		switch (target_specialty)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			// 363 Marksmanship
			LearnTalent(1344);
			LearnTalent(1349);
			LearnTalent(1818);
			LearnTalent(1346);
			LearnTalent(1342);
			LearnTalent(1353);
			LearnTalent(1819);
			LearnTalent(1341);
			LearnTalent(1804);
			LearnTalent(1362);
			LearnTalent(1361);
			LearnTalent(1348);
			LearnTalent(1345);
			LearnTalent(1807);
			LearnTalent(1808);
			LearnTalent(1343);
			LearnTalent(1806);
			LearnTalent(2132);
			LearnTalent(2135);
			LearnTalent(2134);
			LearnTalent(2197);

			LearnTalent(1382);

			LearnTalent(2130);
			LearnTalent(2131);

			LearnTalent(1624);
			break;
		}
		case 2:
		{
			break;
		}
		default:
		{
			break;
		}
		}
	}

	me->SendTalentsInfoData(false);

	// spells 
	me->LearnDefaultSkills();
	me->learnDefaultSpells();
	me->learnQuestRewardedSpells();
	me->UpdateSkillsForLevel(true);

	if (TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerSpells(sNierManager->trainerMap[target_class]))
	{
		bool hasNew = false;
		while (true)
		{
			hasNew = false;
			for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
			{
				TrainerSpell const* tSpell = &itr->second;
				if (me->HasSpell(tSpell->spell))
				{
					continue;
				}
				TrainerSpellState state = me->GetTrainerSpellState(tSpell, tSpell->reqLevel);
				if (state == TrainerSpellState::TRAINER_SPELL_GREEN)
				{
					me->learnSpell(tSpell->spell, true);
					hasNew = true;
				}
			}
			if (!hasNew)
			{
				break;
			}
		}
	}
	if (const CreatureInfo* ci = sObjectMgr.GetCreatureTemplate(sNierManager->trainerMap[target_class]))
	{
		uint32 trainerTemplateId = ci->TrainerTemplateId;

		if (TrainerSpellData const* trainer_template_spells = sObjectMgr.GetNpcTrainerTemplateSpells(trainerTemplateId))
		{
			bool hasNew = false;
			while (true)
			{
				hasNew = false;
				for (TrainerSpellMap::const_iterator itr = trainer_template_spells->spellList.begin(); itr != trainer_template_spells->spellList.end(); ++itr)
				{
					TrainerSpell const* tSpell = &itr->second;
					if (me->HasSpell(tSpell->spell))
					{
						continue;
					}
					TrainerSpellState state = me->GetTrainerSpellState(tSpell, tSpell->reqLevel);
					if (state == TrainerSpellState::TRAINER_SPELL_GREEN)
					{
						me->learnSpell(tSpell->spell, true);
						hasNew = true;
					}
				}
				if (!hasNew)
				{
					break;
				}
			}
		}
	}
}

bool Nier_Base::InitializeEquipments(bool pReset)
{
	if (!me)
	{
		return false;
	}
	for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
	{
		if (Item* inventoryItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
		{
			me->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
		}
	}
	if (pReset)
	{
		for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
		{
			if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
			{
				me->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
			}
		}
	}

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = 0;

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_BACK;
	inventoryType = InventoryType::INVTYPE_CLOAK;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	if (target_class == Classes::CLASS_DEATH_KNIGHT || target_class == Classes::CLASS_PALADIN || target_class == Classes::CLASS_WARRIOR)
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
		if (requiredLevel >= 40)
		{
			itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE;
		}
	}
	else if (target_class == Classes::CLASS_HUNTER || target_class == Classes::CLASS_SHAMAN)
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
		if (requiredLevel >= 40)
		{
			itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
		}
	}
	else if (target_class == Classes::CLASS_DRUID || target_class == Classes::CLASS_ROGUE)
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
	}
	else
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
	}
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_CHEST;
	inventoryType = InventoryType::INVTYPE_CHEST;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_FEET;
	inventoryType = InventoryType::INVTYPE_FEET;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_HANDS;
	inventoryType = InventoryType::INVTYPE_HANDS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD;
	inventoryType = InventoryType::INVTYPE_HEAD;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_LEGS;
	inventoryType = InventoryType::INVTYPE_LEGS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS;
	inventoryType = InventoryType::INVTYPE_SHOULDERS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_WAIST;
	inventoryType = InventoryType::INVTYPE_WAIST;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_WRISTS;
	inventoryType = InventoryType::INVTYPE_WRISTS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_MAINHAND;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	if (target_class == Classes::CLASS_MAGE || target_class == Classes::CLASS_PRIEST || target_class == Classes::CLASS_WARLOCK || target_class == Classes::CLASS_DRUID)
	{
		inventoryType = InventoryType::INVTYPE_2HWEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_HUNTER)
	{
		inventoryType = InventoryType::INVTYPE_2HWEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_PALADIN)
	{
		inventoryType = InventoryType::INVTYPE_2HWEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_ROGUE)
	{
		inventoryType = InventoryType::INVTYPE_WEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_SHAMAN)
	{
		inventoryType = InventoryType::INVTYPE_WEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_WARRIOR)
	{
		inventoryType = InventoryType::INVTYPE_WEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_DEATH_KNIGHT)
	{
		inventoryType = InventoryType::INVTYPE_2HWEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_OFFHAND;
	if (target_class == Classes::CLASS_ROGUE)
	{
		itemClass = ItemClass::ITEM_CLASS_WEAPON;
		inventoryType = InventoryType::INVTYPE_WEAPON;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_SHAMAN)
	{
		itemClass = ItemClass::ITEM_CLASS_ARMOR;
		inventoryType = InventoryType::INVTYPE_HOLDABLE;
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_WARRIOR)
	{
		itemClass = ItemClass::ITEM_CLASS_ARMOR;
		inventoryType = InventoryType::INVTYPE_SHIELD;
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	if (target_class == Classes::CLASS_HUNTER)
	{
		itemClass = ItemClass::ITEM_CLASS_WEAPON;
		inventoryType = InventoryType::INVTYPE_RANGED;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_ROGUE)
	{
		itemClass = ItemClass::ITEM_CLASS_WEAPON;
		inventoryType = InventoryType::INVTYPE_THROWN;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_WARRIOR)
	{
		itemClass = ItemClass::ITEM_CLASS_WEAPON;
		inventoryType = InventoryType::INVTYPE_THROWN;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}
	else if (target_class == Classes::CLASS_MAGE || target_class == Classes::CLASS_PRIEST || target_class == Classes::CLASS_WARLOCK)
	{
		itemClass = ItemClass::ITEM_CLASS_WEAPON;
		inventoryType = InventoryType::INVTYPE_RANGEDRIGHT;
		itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND;
		EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	}

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_NECK;
	inventoryType = InventoryType::INVTYPE_NECK;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_FINGER1;
	inventoryType = InventoryType::INVTYPE_FINGER;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_FINGER2;
	inventoryType = InventoryType::INVTYPE_HEAD;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	return true;
}

bool Nier_Base::EuipRandom(uint32 pmEquipSlot, uint32 pmInventoryType, uint32 pmItemClass, uint32 pmItemSubClass, uint32 pmMaxLevel)
{
	if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, pmEquipSlot))
	{
		return false;
	}
	int minLevel = pmMaxLevel - 5;
	if (minLevel < 1)
	{
		minLevel = 1;
	}
	std::ostringstream itemQueryStream;
	itemQueryStream << "SELECT entry FROM item_template where class = " << pmItemClass << " and subclass = " << pmItemSubClass << " and InventoryType = " << pmInventoryType << " and RequiredSkill = 0 and RequiredReputationFaction = 0 and RequiredLevel >= " << minLevel << " and RequiredLevel <= " << pmMaxLevel << " order by rand() ";
	auto itemQR = WorldDatabase.Query(itemQueryStream.str().c_str());
	if (itemQR)
	{
		do
		{
			Field* fields = itemQR->Fetch();
			uint32 itemEntry = fields[0].GetUInt32();

			if (Item* pItem = Item::CreateItem(itemEntry, 1))
			{
				uint16 dest = 0;
				me->CombatStopWithPets(true, true);
				if (me->CanEquipItem(pmEquipSlot, dest, pItem, false) == InventoryResult::EQUIP_ERR_OK)
				{
					me->EquipItem(dest, pItem, true);
					std::ostringstream msgStream;
					msgStream << me->GetName() << " Equiped " << pItem->GetProto()->Name1;
					//sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
					sLog.outBasic(msgStream.str().c_str());
					return true;
				}
			}
		} while (itemQR->NextRow());
	}
	return false;
}

bool Nier_Base::Threating(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}
	else if (!me->CanAttack(pTarget))
	{
		return false;
	}
	if (!me->CanReachWithMeleeAttack(pTarget))
	{
		return false;
	}

	return true;
}

bool Nier_Base::Tank(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}
	else if (!me->CanAttack(pTarget))
	{
		return false;
	}
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool Nier_Base::Heal(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}

	return true;
}

bool Nier_Base::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}
	else if (!me->CanAttack(pTarget))
	{
		return false;
	}
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}
	else
	{
		// ranged 
		if (targetDistance < dpsDistance)
		{
			if (pTarget->IsWithinLOS(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
			{
				if (!me->isInFront(pTarget, dpsDistance, M_PI / 4))
				{
					me->GetMotionMaster()->Clear(true);
					me->SetFacingToObject(pTarget);
					return true;
				}
				else if (me->IsMoving())
				{
					me->UpdateSplinePosition();
					me->movespline->_Finalize();
					me->GetMotionMaster()->Clear(true);
				}
				return true;
			}
		}

		bool adjust = false;
		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance > dpsDistance)
		{
			adjust = true;
		}
		else if (!pTarget->IsWithinLOS(destination.x, destination.y, destination.z))
		{
			adjust = true;
		}

		if (adjust)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, pTarget->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool Nier_Base::PVP(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}
	else if (!me->CanAttack(pTarget))
	{
		return false;
	}
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		ClearTarget();
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}
	else
	{
		// ranged 
		if (targetDistance < dpsDistance)
		{
			if (pTarget->IsWithinLOS(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
			{
				if (!me->isInFront(pTarget, dpsDistance, M_PI / 4))
				{
					me->GetMotionMaster()->Clear(true);
					me->SetFacingToObject(pTarget);
					return true;
				}
				else if (me->IsMoving())
				{
					me->UpdateSplinePosition();
					me->movespline->_Finalize();
					me->GetMotionMaster()->Clear(true);
				}
				return true;
			}
		}

		bool adjust = false;
		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance > dpsDistance)
		{
			adjust = true;
		}
		else if (!pTarget->IsWithinLOS(destination.x, destination.y, destination.z))
		{
			adjust = true;
		}

		if (adjust)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, pTarget->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool Nier_Base::Interrupt(Unit* pTarget)
{
	return false;
}

bool Nier_Base::Buff()
{
	return false;
}

bool Nier_Base::Cure()
{
	return false;
}

bool Nier_Base::Revive()
{
	return false;
}

void Nier_Base::MoveTo(Position pDestination, Unit* pTarget, uint32 pMoveType)
{
	if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
	{
		me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
	}
	me->m_movementInfo.RemoveMovementFlag(MovementFlags::MOVEFLAG_SPLINE_ELEVATION);
	float runSpeed = me->GetSpeed((UnitMoveType)pMoveType);

	Movement::MoveSplineInit init(*me);
	init.SetWalk(false);
	init.SetVelocity(runSpeed);
	if (pTarget)
	{
		init.SetFacing(pTarget);
	}
	init.MoveTo(pDestination.x, pDestination.y, pDestination.z, true);
	init.Launch();

	//me->GetMotionMaster()->MovePoint(0, pDestination, ForcedMovement::FORCED_MOVEMENT_RUN, runSpeed);
}

bool Nier_Base::Follow(Unit* pTarget)
{
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	//ChooseTarget(pTarget);
	if (targetDistance < followDistance)
	{
		if (me->IsMoving())
		{
			me->UpdateSplinePosition();
			me->movespline->_Finalize();
			me->GetMotionMaster()->Clear(true);
			return true;
		}
		return false;
	}

	// destination check 
	float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
	if (destDistance > followDistance)
	{
		float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
		angle = frand(angle - 0.1f, angle + 0.1f);
		pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle);
		MoveTo(destination, pTarget);
		return true;
	}

	if (!me->IsMoving())
	{
		MoveTo(destination, pTarget);
	}

	return true;
}

bool Nier_Base::Wander()
{
	if (wanderDelay > 0)
	{
		return false;
	}
	else
	{
		float distance = frand(5.0f, 30.0f);
		float angle = frand(0.0f, M_PI * 2.0f);
		me->GetNearPoint(me, destination.x, destination.y, destination.z, 0.0f, distance, angle);
		MoveTo(destination, nullptr, UnitMoveType::MOVE_WALK);
		wanderDelay = urand(10000, 30000);
	}
	return true;
}

bool Nier_Base::UseItem(Item* pmItem, Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}

	if (sNierManager->SpellCasting(me))
	{
		return false;
	}

	if (const ItemPrototype* proto = pmItem->GetProto())
	{
		//ChooseTarget(pmTarget);
		SpellCastTargets targets;
		targets.Update(pmTarget);
		uint32 spellId = 0;
		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			if (proto->Spells[i].SpellId > 0)
			{
				spellId = proto->Spells[i].SpellId;
				break;
			}
		}
		me->CastItemUseSpell(pmItem, targets, 1, 0, spellId);
		return true;
	}

	return false;
}

bool Nier_Base::UseItem(Item* pmItem, Item* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}
	if (sNierManager->SpellCasting(me))
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}

	if (const ItemPrototype* proto = pmItem->GetProto())
	{
		SpellCastTargets targets;
		targets.setItemTarget(pmTarget);
		uint32 spellId = 0;
		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			if (proto->Spells[i].SpellId > 0)
			{
				spellId = proto->Spells[i].SpellId;
				break;
			}
		}
		me->CastItemUseSpell(pmItem, targets, 1, 0, spellId);
		return true;
	}

	return false;
}

bool Nier_Base::CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura, bool pmOnlyMyAura, uint32 pmMaxAuraStack, bool pmClearShapeShift)
{
	if (!SpellValid(pmSpellId))
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (sNierManager->SpellCasting(me))
	{
		return true;
	}
	if (pmClearShapeShift)
	{
		me->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);
	}
	if (const SpellEntry* pS = sSpellTemplate.LookupEntry<SpellEntry>(pmSpellId))
	{
		if (pmTarget)
		{
			if (!me->IsWithinLOSInMap(pmTarget))
			{
				return false;
			}
			if (pmTarget->IsImmuneToSpell(pS, false, 0, me))
			{
				return false;
			}
			bool valid = true;
			if (pmCheckAura)
			{
				for (uint8 effIndex = 0; effIndex < MAX_EFFECT_INDEX; ++effIndex)
				{
					Unit::AuraList const& auras = pmTarget->GetAurasByType(AuraType(pS->EffectApplyAuraName[effIndex]));
					for (Unit::AuraList::const_iterator it = auras.begin(); it != auras.end(); ++it)
					{
						if (Aura* eachAura = *it)
						{
							if (eachAura->GetSpellProto()->Id == pmSpellId)
							{
								if (pmOnlyMyAura)
								{
									if (eachAura->GetCasterGuid() != me->GetObjectGuid())
									{
										continue;
									}
								}
								if (eachAura->GetStackAmount() >= pmMaxAuraStack)
								{
									valid = false;
								}
								break;
							}
						}
					}
					if (!valid)
					{
						break;
					}
				}
				if (!valid)
				{
					return false;
				}
			}
			if (!me->isInFront(pmTarget, VISIBILITY_DISTANCE_NORMAL, M_PI / 2))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pmTarget);
				return true;
			}
			if (me->GetSelectionGuid() != pmTarget->GetObjectGuid())
			{
				ChooseTarget(pmTarget);
			}
		}
		for (size_t i = 0; i < MAX_SPELL_REAGENTS; i++)
		{
			if (pS->Reagent[i] > 0)
			{
				if (!me->HasItemCount(pS->Reagent[i], pS->ReagentCount[i]))
				{
					me->StoreNewItemInBestSlots(pS->Reagent[i], pS->ReagentCount[i] * 10);
				}
			}
		}
		if (SpellRangeEntry const* spellRange = sSpellRangeStore.LookupEntry(pS->rangeIndex))
		{
			float maxRange = GetSpellMaxRange(spellRange);
			float targetDistance = me->GetDistance(pmTarget);
			if (targetDistance < maxRange)
			{
				if (me->IsMoving())
				{
					me->StopMoving(true);
					me->GetMotionMaster()->Clear(true);
				}
			}
		}
		if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
		{
			me->SetStandState(UNIT_STAND_STATE_STAND);
		}

		SpellCastResult scr = me->CastSpell(pmTarget, pS->Id, TriggerCastFlags::TRIGGERED_NONE);
		if (scr == SpellCastResult::SPELL_CAST_OK)
		{
			return true;
		}
	}

	return false;
}

void Nier_Base::CancelAura(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return;
	}
	if (!me)
	{
		return;
	}
	me->RemoveAurasDueToSpell(pmSpellID);
}

void Nier_Base::LearnTalent(uint32 pmTalentId, uint32 pmMaxRank)
{
	if (!me)
	{
		return;
	}
	uint32 checkRank = 0;
	while (checkRank < pmMaxRank)
	{
		me->LearnTalent(pmTalentId, checkRank);
		checkRank++;
	}
}

bool Nier_Base::Rest(bool pForce)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	else if (me->IsInCombat())
	{
		return false;
	}
	float hpPercent = me->GetHealthPercent();
	float manaPercent = me->GetPower(Powers::POWER_MANA) * 100.0f / me->GetMaxPower(Powers::POWER_MANA);
	if (hpPercent < 60.0f || manaPercent < 60.0f || pForce)
	{
		uint32 foodEntry = 0;
		uint32 myLevel = me->GetLevel();
		if (myLevel >= 75)
		{
			foodEntry = 35950;
		}
		else if (myLevel >= 65)
		{
			foodEntry = 33449;
		}
		else if (myLevel >= 55)
		{
			foodEntry = 27855;
		}
		else if (myLevel >= 45)
		{
			foodEntry = 8950;
		}
		else if (myLevel >= 35)
		{
			foodEntry = 4601;
		}
		else if (myLevel >= 25)
		{
			foodEntry = 4544;
		}
		else if (myLevel >= 15)
		{
			foodEntry = 4542;
		}
		else if (myLevel >= 5)
		{
			foodEntry = 4541;
		}
		else
		{
			foodEntry = 4540;
		}
		if (!me->HasItemCount(foodEntry, 1))
		{
			me->StoreNewItemInBestSlots(foodEntry, 20);
		}

		Item* pFood = me->GetItemByEntry(foodEntry);
		if (pFood && !pFood->IsInTrade())
		{
			if (UseItem(pFood, me))
			{
				restDelay = 20000;
			}
		}
	}

	if (restDelay > 0)
	{
		drinkDelay = 1000;
		return true;
	}

	return false;
}

bool Nier_Base::Drink()
{
	uint32 drinkEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 75)
	{
		drinkEntry = 33445;
	}
	else if (myLevel >= 70)
	{
		drinkEntry = 33444;
	}
	else if (myLevel >= 65)
	{
		drinkEntry = 27860;
	}
	else if (myLevel >= 60)
	{
		drinkEntry = 28399;
	}
	else if (myLevel >= 55)
	{
		drinkEntry = 18300;
	}
	else if (myLevel >= 45)
	{
		drinkEntry = 8766;
	}
	else if (myLevel >= 35)
	{
		drinkEntry = 1645;
	}
	else if (myLevel >= 25)
	{
		drinkEntry = 1708;
	}
	else if (myLevel >= 15)
	{
		drinkEntry = 1205;
	}
	else if (myLevel >= 5)
	{
		drinkEntry = 1179;
	}
	else
	{
		drinkEntry = 159;
	}

	if (!me->HasItemCount(drinkEntry, 1))
	{
		me->StoreNewItemInBestSlots(drinkEntry, 20);
	}
	Item* pDrink = me->GetItemByEntry(drinkEntry);
	if (pDrink && !pDrink->IsInTrade())
	{
		if (UseItem(pDrink, me))
		{
			return true;
		}
	}

	return false;
}

void Nier_Base::ChooseTarget(Unit* pmTarget)
{
	if (pmTarget)
	{
		if (me)
		{
			me->SetSelectionGuid(pmTarget->GetObjectGuid());
			me->SetTarget(pmTarget);
		}
	}
}

void Nier_Base::ClearTarget()
{
	if (me)
	{
		me->AttackStop(true, true, true, true);
		//me->CastStop();
		InterruptSpells();
		me->SetSelectionGuid(ObjectGuid());
		me->SetTarget(nullptr);
		if (Pet* myPet = me->GetPet())
		{
			myPet->AttackStop(true, true, true, true);
			if (CharmInfo* pci = myPet->GetCharmInfo())
			{
				pci->SetCommandState(COMMAND_FOLLOW);
			}
		}
		me->GetMotionMaster()->Clear(true);
	}
}

void Nier_Base::InterruptSpells()
{
	if (me)
	{
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		me->SetStandState(MAX_UNIT_STAND_STATE);
	}
}

bool Nier_Base::SpellValid(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (!me->IsSpellReady(pmSpellID))
	{
		return false;
	}

	return true;
}
