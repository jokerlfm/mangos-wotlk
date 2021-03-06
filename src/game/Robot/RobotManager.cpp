#include "RobotManager.h"

#include "Log.h"
#include "Accounts/AccountMgr.h"
#include "Entities/Pet.h"
#include "Server/WorldSession.h"
#include "Globals/ObjectMgr.h"
#include "MotionGenerators/MotionMaster.h"
#include "Maps/MapManager.h"
#include "Groups/Group.h"
#include "Entities/Item.h"
#include "World/World.h"
#include "Spells/SpellMgr.h"
#include "RobotConfig.h"
#include "RobotAI.h"
#include "Strategies/Strategy_Solo_Normal.h"

RobotManager::RobotManager()
{
	updateRobotGroupIndex = 0;

	nameIndex = 0;
	robotMap.clear();
	robotAICache.clear();
	playerGroupRoleCache.clear();
	deleteRobotAccountSet.clear();
	meleeWeaponMap.clear();
	rangeWeaponMap.clear();
	armorMap.clear();
	miscMap.clear();
	teleportCacheMap.clear();
	tamableBeastEntryMap.clear();
	spellNameEntryMap.clear();
}

void RobotManager::InitializeManager()
{
	if (sRobotConfig.Enable == 0)
	{
		return;
	}

	if (sRobotConfig.ResetRobots == 1)
	{
		sLog.outBasic("Reset robots");
		if (DeleteRobots())
		{
			sWorld.ShutdownServ(10, SHUTDOWN_MASK_RESTART | SHUTDOWN_MASK_IDLE, RESTART_EXIT_CODE);
			return;
		}
	}

	sLog.outBasic("Initialize robot manager");

	QueryResult* robotNamesQR = WorldDatabase.Query("SELECT name FROM robot_names order by rand()");
	if (!robotNamesQR)
	{
		sLog.outError("Found zero robot names");
		sRobotConfig.Enable = false;
		delete robotNamesQR;
		return;
	}
	do
	{
		Field* fields = robotNamesQR->Fetch();
		std::string eachName = fields[0].GetString();
		robotNameMap[robotNameMap.size()] = eachName;
	} while (robotNamesQR->NextRow());

	availableRaces[CLASS_WARRIOR].push_back(RACE_HUMAN);
	availableRaces[CLASS_WARRIOR].push_back(RACE_NIGHTELF);
	availableRaces[CLASS_WARRIOR].push_back(RACE_GNOME);
	availableRaces[CLASS_WARRIOR].push_back(RACE_DWARF);
	availableRaces[CLASS_WARRIOR].push_back(RACE_ORC);
	availableRaces[CLASS_WARRIOR].push_back(RACE_UNDEAD);
	availableRaces[CLASS_WARRIOR].push_back(RACE_TAUREN);
	availableRaces[CLASS_WARRIOR].push_back(RACE_TROLL);
	availableRaces[CLASS_WARRIOR].push_back(Races::RACE_DRAENEI);

	availableRaces[CLASS_PALADIN].push_back(RACE_HUMAN);
	availableRaces[CLASS_PALADIN].push_back(RACE_DWARF);
	availableRaces[CLASS_PALADIN].push_back(Races::RACE_DRAENEI);
	availableRaces[CLASS_PALADIN].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_ROGUE].push_back(RACE_HUMAN);
	availableRaces[CLASS_ROGUE].push_back(RACE_DWARF);
	availableRaces[CLASS_ROGUE].push_back(RACE_NIGHTELF);
	availableRaces[CLASS_ROGUE].push_back(RACE_GNOME);
	availableRaces[CLASS_ROGUE].push_back(RACE_ORC);
	availableRaces[CLASS_ROGUE].push_back(RACE_TROLL);
	availableRaces[CLASS_ROGUE].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_PRIEST].push_back(RACE_HUMAN);
	availableRaces[CLASS_PRIEST].push_back(RACE_DWARF);
	availableRaces[CLASS_PRIEST].push_back(RACE_NIGHTELF);
	availableRaces[CLASS_PRIEST].push_back(RACE_TROLL);
	availableRaces[CLASS_PRIEST].push_back(RACE_UNDEAD);
	availableRaces[CLASS_PRIEST].push_back(Races::RACE_DRAENEI);
	availableRaces[CLASS_PRIEST].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_MAGE].push_back(RACE_HUMAN);
	availableRaces[CLASS_MAGE].push_back(RACE_GNOME);
	availableRaces[CLASS_MAGE].push_back(RACE_UNDEAD);
	availableRaces[CLASS_MAGE].push_back(RACE_TROLL);
	availableRaces[CLASS_MAGE].push_back(Races::RACE_DRAENEI);
	availableRaces[CLASS_MAGE].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_WARLOCK].push_back(RACE_HUMAN);
	availableRaces[CLASS_WARLOCK].push_back(RACE_GNOME);
	availableRaces[CLASS_WARLOCK].push_back(RACE_UNDEAD);
	availableRaces[CLASS_WARLOCK].push_back(RACE_ORC);
	availableRaces[CLASS_WARLOCK].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_SHAMAN].push_back(RACE_ORC);
	availableRaces[CLASS_SHAMAN].push_back(RACE_TAUREN);
	availableRaces[CLASS_SHAMAN].push_back(RACE_TROLL);
	availableRaces[CLASS_SHAMAN].push_back(Races::RACE_DRAENEI);

	availableRaces[CLASS_HUNTER].push_back(RACE_DWARF);
	availableRaces[CLASS_HUNTER].push_back(RACE_NIGHTELF);
	availableRaces[CLASS_HUNTER].push_back(RACE_ORC);
	availableRaces[CLASS_HUNTER].push_back(RACE_TAUREN);
	availableRaces[CLASS_HUNTER].push_back(RACE_TROLL);
	availableRaces[CLASS_HUNTER].push_back(Races::RACE_DRAENEI);
	availableRaces[CLASS_HUNTER].push_back(Races::RACE_BLOODELF);

	availableRaces[CLASS_DRUID].push_back(RACE_NIGHTELF);
	availableRaces[CLASS_DRUID].push_back(RACE_TAUREN);


	armorInventorySet.insert(InventoryType::INVTYPE_CHEST);
	armorInventorySet.insert(InventoryType::INVTYPE_FEET);
	armorInventorySet.insert(InventoryType::INVTYPE_HANDS);
	armorInventorySet.insert(InventoryType::INVTYPE_HEAD);
	armorInventorySet.insert(InventoryType::INVTYPE_LEGS);
	armorInventorySet.insert(InventoryType::INVTYPE_SHOULDERS);
	armorInventorySet.insert(InventoryType::INVTYPE_WAIST);
	armorInventorySet.insert(InventoryType::INVTYPE_WRISTS);

	miscInventoryMap[0] = InventoryType::INVTYPE_CLOAK;
	miscInventoryMap[1] = InventoryType::INVTYPE_FINGER;
	miscInventoryMap[2] = InventoryType::INVTYPE_FINGER;
	miscInventoryMap[3] = InventoryType::INVTYPE_NECK;

	characterTalentTabNameMap.clear();
	characterTalentTabNameMap[Classes::CLASS_WARRIOR][0] = "Arms";
	characterTalentTabNameMap[Classes::CLASS_WARRIOR][1] = "Fury";
	characterTalentTabNameMap[Classes::CLASS_WARRIOR][2] = "Protection";

	characterTalentTabNameMap[Classes::CLASS_HUNTER][0] = "Beast Mastery";
	characterTalentTabNameMap[Classes::CLASS_HUNTER][1] = "Marksmanship";
	characterTalentTabNameMap[Classes::CLASS_HUNTER][2] = "Survival";

	characterTalentTabNameMap[Classes::CLASS_SHAMAN][0] = "Elemental";
	characterTalentTabNameMap[Classes::CLASS_SHAMAN][1] = "Enhancement";
	characterTalentTabNameMap[Classes::CLASS_SHAMAN][2] = "Restoration";

	characterTalentTabNameMap[Classes::CLASS_PALADIN][0] = "Holy";
	characterTalentTabNameMap[Classes::CLASS_PALADIN][1] = "Protection";
	characterTalentTabNameMap[Classes::CLASS_PALADIN][2] = "Retribution";

	characterTalentTabNameMap[Classes::CLASS_WARLOCK][0] = "Affliction";
	characterTalentTabNameMap[Classes::CLASS_WARLOCK][1] = "Demonology";
	characterTalentTabNameMap[Classes::CLASS_WARLOCK][2] = "Destruction";

	characterTalentTabNameMap[Classes::CLASS_PRIEST][0] = "Descipline";
	characterTalentTabNameMap[Classes::CLASS_PRIEST][1] = "Holy";
	characterTalentTabNameMap[Classes::CLASS_PRIEST][2] = "Shadow";

	characterTalentTabNameMap[Classes::CLASS_ROGUE][0] = "Assassination";
	characterTalentTabNameMap[Classes::CLASS_ROGUE][1] = "Combat";
	characterTalentTabNameMap[Classes::CLASS_ROGUE][2] = "subtlety";

	characterTalentTabNameMap[Classes::CLASS_MAGE][0] = "Arcane";
	characterTalentTabNameMap[Classes::CLASS_MAGE][1] = "Fire";
	characterTalentTabNameMap[Classes::CLASS_MAGE][2] = "Frost";

	characterTalentTabNameMap[Classes::CLASS_DRUID][0] = "Balance";
	characterTalentTabNameMap[Classes::CLASS_DRUID][1] = "Feral";
	characterTalentTabNameMap[Classes::CLASS_DRUID][2] = "Restoration";

	// all equip are from loot
	std::unordered_set<uint32> rlSet;
	rlSet.clear();
	QueryResult* rlQR = WorldDatabase.Query("SELECT distinct Item FROM reference_loot_template");
	if (rlQR)
	{
		do
		{
			Field* rlField = rlQR->Fetch();
			uint32 eachLootItemEntry = rlField[0].GetUInt32();
			rlSet.insert(eachLootItemEntry);
		} while (rlQR->NextRow());
		delete rlQR;
	}

	uint8 levelRange = 0;
	for (uint32 itemID = 0; itemID < sItemStorage.GetMaxEntry(); ++itemID)
	{
		ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemID);
		if (!proto)
		{
			continue;
		}
		if (proto->Quality < 2)
		{
			continue;
		}
		//if (proto->RandomSuffix > 0)
		//{
		//	continue;
		//}
		levelRange = proto->RequiredLevel / 10;
		if (proto->InventoryType == InventoryType::INVTYPE_CLOAK || proto->InventoryType == InventoryType::INVTYPE_FINGER || proto->InventoryType == InventoryType::INVTYPE_NECK)
		{
			miscMap[proto->InventoryType][levelRange][miscMap[proto->InventoryType][levelRange].size()] = proto->ItemId;
			continue;
		}
		if (proto->Class == ItemClass::ITEM_CLASS_WEAPON)
		{
			switch (proto->SubClass)
			{
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF:
			{
				meleeWeaponMap[0][levelRange][meleeWeaponMap[0][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD:
			{
				meleeWeaponMap[1][levelRange][meleeWeaponMap[1][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD2:
			{
				meleeWeaponMap[2][levelRange][meleeWeaponMap[2][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER:
			{
				meleeWeaponMap[3][levelRange][meleeWeaponMap[3][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2:
			{
				meleeWeaponMap[5][levelRange][meleeWeaponMap[5][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_POLEARM:
			{
				meleeWeaponMap[6][levelRange][meleeWeaponMap[6][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW:
			{
				rangeWeaponMap[0][levelRange][rangeWeaponMap[0][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW:
			{
				rangeWeaponMap[0][levelRange][rangeWeaponMap[0][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN:
			{
				rangeWeaponMap[0][levelRange][rangeWeaponMap[0][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND:
			{
				rangeWeaponMap[1][levelRange][rangeWeaponMap[1][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN:
			{
				rangeWeaponMap[2][levelRange][rangeWeaponMap[2][levelRange].size()] = proto->ItemId;
				continue;
			}
			default:
			{
				break;
			}
			}
		}
		else if (proto->Class == ItemClass::ITEM_CLASS_ARMOR)
		{
			switch (proto->SubClass)
			{
			case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH:
			{
				armorMap[0][proto->InventoryType][levelRange][armorMap[0][proto->InventoryType][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER:
			{
				armorMap[1][proto->InventoryType][levelRange][armorMap[1][proto->InventoryType][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL:
			{
				armorMap[2][proto->InventoryType][levelRange][armorMap[2][proto->InventoryType][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE:
			{
				armorMap[3][proto->InventoryType][levelRange][armorMap[3][proto->InventoryType][levelRange].size()] = proto->ItemId;
				continue;
			}
			case ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD:
			{
				meleeWeaponMap[4][levelRange][meleeWeaponMap[4][levelRange].size()] = proto->ItemId;
				continue;
			}
			default:
			{
				break;
			}
			}
		}
	}

	spellRewardClassQuestIDSet.clear();
	std::unordered_map<uint32, Quest*> allQuestMap = sObjectMgr.GetQuestTemplates();
	for (std::unordered_map<uint32, Quest*>::iterator it = allQuestMap.begin(); it != allQuestMap.end(); it++)
	{
		if (it->second->GetRequiredClasses() > 0)
		{
			if (it->second->GetRewSpellCast() > 0)
			{
				spellRewardClassQuestIDSet.insert(it->first);
			}
		}
	}

	QueryResult* normalCreatureQR = WorldDatabase.Query("SELECT CT.maxlevel, C.map, C.position_x, C.position_y, C.position_z FROM creature_template CT join creature C on CT.entry = C.id where CT.rank = 0 and (C.map <> 0 or C.map <> 1 or C.map <> 530 or C.map <> 571)");
	if (normalCreatureQR)
	{
		do
		{
			Field* creatureField = normalCreatureQR->Fetch();
			uint8 level = creatureField[0].GetUInt8();
			level = level / 10;
			int mapID = creatureField[1].GetInt32();
			float x = creatureField[2].GetFloat();
			float y = creatureField[3].GetFloat();
			float z = creatureField[4].GetFloat();
			WorldLocation eachWL = WorldLocation(mapID, x, y, z, 0);
			teleportCacheMap[level][teleportCacheMap[level].size()] = eachWL;
		} while (normalCreatureQR->NextRow());
		delete normalCreatureQR;
	}

	for (uint32 i = 1; i < sCreatureStorage.GetMaxEntry(); ++i)
	{
		CreatureInfo const* cInfo = sCreatureStorage.LookupEntry<CreatureInfo>(i);
		if (!cInfo)
		{
			continue;
		}
		if (cInfo->isTameable(false))
		{
			tamableBeastEntryMap[tamableBeastEntryMap.size()] = cInfo->Entry;
		}
	}
	for (int checkEntry = 0; checkEntry < sSpellTemplate.GetMaxEntry(); checkEntry++)
	{
		SpellEntry const* eachSI = sSpellTemplate.LookupEntry<SpellEntry>(checkEntry);
		if (eachSI)
		{
			spellNameEntryMap[eachSI->SpellName[0]].insert(eachSI->Id);
		}
	}

	uint32 checkLevel = 20;
	uint32 maxLevel = 80;
	uint32 checkGroupIndex = 0;
	while (checkLevel <= maxLevel)
	{
		for (std::map<uint32, std::vector<uint32>>::iterator classIT = availableRaces.begin(); classIT != availableRaces.end(); classIT++)
		{
			// EJ debug hunter and priest and paladin will be ignored for now
			if (classIT->first == Classes::CLASS_HUNTER || classIT->first == Classes::CLASS_PRIEST || classIT->first == Classes::CLASS_PALADIN)
			{
				continue;
			}
			std::vector<uint32> raceVector = classIT->second;
			for (std::vector<uint32>::iterator raceIT = raceVector.begin(); raceIT != raceVector.end(); raceIT++)
			{
				RobotAI* eachRAI = new RobotAI(checkLevel, classIT->first, *raceIT);
				robotMap[checkGroupIndex].insert(eachRAI);
				checkGroupIndex++;
				if (checkGroupIndex >= ROBOT_GROUP_COUNT)
				{
					checkGroupIndex = 0;
				}
			}
		}
		checkLevel++;
	}

	sLog.outBasic("Robot system ready");
}

RobotManager* RobotManager::instance()
{
	static RobotManager instance;
	return &instance;
}

void RobotManager::UpdateManager()
{
	if (sRobotConfig.Enable == 0)
	{
		return;
	}

	if (updateRobotGroupIndex >= ROBOT_GROUP_COUNT)
	{
		updateRobotGroupIndex = 0;
	}
	std::unordered_set<RobotAI*> eachRobotGroup = robotMap[updateRobotGroupIndex];
	for (std::unordered_set<RobotAI*>::iterator rit = eachRobotGroup.begin(); rit != eachRobotGroup.end(); rit++)
	{
		(*rit)->Update();
	}
	updateRobotGroupIndex++;
}

bool RobotManager::DeleteRobots()
{
	QueryResult* accountQR = LoginDatabase.PQuery("SELECT id, username FROM account where username like '%s%%'", sRobotConfig.RobotAccountNamePrefix.c_str());

	if (accountQR)
	{
		do
		{
			Field* fields = accountQR->Fetch();
			uint32 id = fields[0].GetUInt32();
			std::string userName = fields[1].GetString();
			deleteRobotAccountSet.insert(id);
			sAccountMgr.DeleteAccount(id);
			sLog.outBasic("Delete robot account %d - %s", id, userName.c_str());
		} while (accountQR->NextRow());
		delete accountQR;
		return true;
	}

	return false;
}

bool RobotManager::RobotsDeleted()
{
	for (std::set<uint32>::iterator it = deleteRobotAccountSet.begin(); it != deleteRobotAccountSet.end(); it++)
	{
		QueryResult* accountQR = LoginDatabase.PQuery("SELECT id FROM account where id = '%d'", (*it));
		if (accountQR)
		{
			sLog.outBasic("Account %d is under deleting", (*it));
			delete accountQR;
			return false;
		}
		QueryResult* characterQR = CharacterDatabase.PQuery("SELECT count(*) FROM characters where account = '%d'", (*it));
		if (characterQR)
		{
			Field* fields = characterQR->Fetch();
			uint32 count = fields[0].GetUInt32();
			delete characterQR;
			if (count > 0)
			{
				sLog.outBasic("Characters for account %d are under deleting", (*it));
				return false;
			}
		}
	}

	sLog.outBasic("Robot accounts are deleted");
	return true;
}

uint32 RobotManager::CheckRobotAccount(std::string pmAccountName)
{
	uint32 accountID = 0;

	QueryResult* accountQR = LoginDatabase.PQuery("SELECT id FROM account where username = '%s'", pmAccountName.c_str());
	if (accountQR)
	{
		Field* idFields = accountQR->Fetch();
		accountID = idFields[0].GetUInt32();
		delete accountQR;
	}
	return accountID;
}

bool RobotManager::CreateRobotAccount(std::string pmAccountName)
{
	AccountOpResult aor = sAccountMgr.CreateAccount(pmAccountName, "robot");
	if (aor == AccountOpResult::AOR_OK)
	{
		sLog.outBasic("Create robot account %s", pmAccountName.c_str());
		return true;
	}
	return false;
}

ObjectGuid RobotManager::CheckAccountCharacter(uint32 pmAccountID)
{
	uint32 characterID = 0;

	QueryResult* characterQR = CharacterDatabase.PQuery("SELECT guid FROM characters where account = '%d'", pmAccountID);
	if (characterQR)
	{
		Field* characterFields = characterQR->Fetch();
		characterID = characterFields[0].GetUInt32();		
		delete characterQR;		
		return ObjectGuid(HighGuid::HIGHGUID_PLAYER, characterID);
	}
	return ObjectGuid();
}

bool RobotManager::CreateRobotCharacter(uint32 pmAccountID, uint32 pmCharacterClass, uint32 pmCharacterRace)
{
	std::string currentName = "";
	bool nameValid = false;
	while (nameIndex < robotNameMap.size())
	{
		currentName = robotNameMap[nameIndex];
		QueryResult* checkNameQR = CharacterDatabase.PQuery("SELECT count(*) FROM characters where name = '%s'", currentName.c_str());

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
			delete checkNameQR;
		}
		nameIndex++;
		if (nameValid)
		{
			break;
		}
	}
	if (!nameValid)
	{
		sLog.outError("No available names");
		return false;
	}

	uint8 gender = 0, skin = 0, face = 0, hairStyle = 0, hairColor = 0, facialHair = 0;
	while (true)
	{
		gender = urand(0, 1);
		face = urand(0, 5);
		hairStyle = urand(0, 5);
		hairColor = urand(0, 5);
		facialHair = urand(0, 5);

		WorldSession* eachSession = new WorldSession(pmAccountID, NULL, SEC_PLAYER, 2, 0, LOCALE_enUS);
		Player* newPlayer = new Player(eachSession);
		if (!newPlayer->Create(sObjectMgr.GeneratePlayerLowGuid(), currentName, pmCharacterRace, pmCharacterClass, gender, skin, face, hairStyle, hairColor, facialHair, 0))
		{
			newPlayer->CleanupsBeforeDelete();
			delete eachSession;
			delete newPlayer;
			sLog.outError("Character create failed, %s %d %d", currentName.c_str(), pmCharacterRace, pmCharacterClass);
			sLog.outBasic("Try again");
			continue;
		}
		newPlayer->GetMotionMaster()->Initialize();
		newPlayer->setCinematic(2);
		newPlayer->SetAtLoginFlag(AT_LOGIN_NONE);
		newPlayer->SetPvP(true);
		newPlayer->SaveToDB();
		sWorld.AddSession(eachSession);
		sLog.outBasic("Create character %d - %s for account %d", newPlayer->GetGUIDLow(), currentName.c_str(), pmAccountID);
		break;
	}

	return true;
}

Player* RobotManager::CheckLogin(uint32 pmAccountID, ObjectGuid pmGUID)
{
	Player* currentPlayer = ObjectAccessor::FindPlayer(pmGUID, true);
	if (currentPlayer)
	{
		return currentPlayer;
	}
	return NULL;
}

bool RobotManager::LoginRobot(uint32 pmAccountID, ObjectGuid pmGUID)
{
	Player* currentPlayer = ObjectAccessor::FindPlayer(pmGUID);
	if (currentPlayer)
	{
		if (currentPlayer->IsInWorld())
		{
			sLog.outBasic("Robot %d %s is already in world", pmGUID, currentPlayer->GetName());
			return false;
		}
	}
	QueryResult* characterQR = CharacterDatabase.PQuery("SELECT name, level FROM characters where guid = '%d'", pmGUID);
	if (!characterQR)
	{
		sLog.outError("Found zero robot characters for account %d while processing logging in", pmAccountID);
		return false;
	}
	Field* characterFields = characterQR->Fetch();
	std::string characterName = characterFields[0].GetString();
	uint8 characterLevel = characterFields[1].GetUInt8();
	delete characterQR;
	WorldSession* loginSession = sWorld.FindSession(pmAccountID);
	if (!loginSession)
	{
		loginSession = new WorldSession(pmAccountID, NULL, SEC_PLAYER, 2, 0, LOCALE_enUS);
		sWorld.AddSession(loginSession);
	}
	WorldPacket loginPKT;
	loginPKT << pmGUID;
	loginSession->HandlePlayerLoginOpcode(loginPKT);
	sLog.outBasic("Log in character %d %s (level %d)", pmGUID, characterName.c_str(), characterLevel);

	return true;
}

void RobotManager::LogoutRobots()
{
	for (std::unordered_map<uint32, RobotAI*>::iterator rit = sRobotManager->robotAICache.begin(); rit != sRobotManager->robotAICache.end(); rit++)
	{
		Player* checkP = ObjectAccessor::FindPlayer(rit->second->characterGUID, true);
		if (checkP)
		{
			sLog.outBasic("Log out robot %s", checkP->GetName());
			std::ostringstream msgStream;
			msgStream << checkP->GetName() << " logged out";
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
			WorldSession* checkWS = checkP->GetSession();
			if (checkWS)
			{
				checkWS->LogoutPlayer(true);
			}
		}
	}
}

void RobotManager::HandlePlayerSay(Player* pmPlayer, std::string pmContent)
{
	std::vector<std::string> commandVector = sRobotManager->SplitString(pmContent, " ", true);
	std::string commandName = commandVector.at(0);
	if (commandName == "role")
	{
		std::ostringstream replyStream;
		if (commandVector.size() > 1)
		{
			std::string newRole = commandVector.at(1);
			if (newRole == "dps")
			{
				playerGroupRoleCache[pmPlayer->GetGUIDLow()] = 0;
			}
			else if (newRole == "tank")
			{
				playerGroupRoleCache[pmPlayer->GetGUIDLow()] = 1;
			}
			else if (newRole == "healer")
			{
				playerGroupRoleCache[pmPlayer->GetGUIDLow()] = 2;
			}
		}

		replyStream << "Your group role is ";
		switch (playerGroupRoleCache[pmPlayer->GetGUIDLow()])
		{
		case 0:
		{
			replyStream << "dps";
			break;
		}
		case 1:
		{
			replyStream << "tank";
			break;
		}
		case 2:
		{
			replyStream << "healer";
			break;
		}
		default:
		{
			replyStream << "dps";
			break;
		}
		}

		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "join")
	{
		std::ostringstream replyStream;
		Group* myGroup = pmPlayer->GetGroup();
		if (myGroup)
		{
			ObjectGuid targetGUID = pmPlayer->GetSelectionGuid();
			if (!targetGUID.IsEmpty())
			{
				bool validTarget = false;
				for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
				{
					Player* member = groupRef->getSource();
					if (member)
					{
						if (member->GetObjectGuid() != pmPlayer->GetObjectGuid())
						{
							if (member->GetObjectGuid() == targetGUID)
							{
								validTarget = true;
								replyStream << "Joining " << member->GetName();
								pmPlayer->TeleportTo(member->GetMapId(), member->GetPositionX(), member->GetPositionY(), member->GetPositionZ(), member->GetOrientation());
							}
						}
					}
				}
				if (!validTarget)
				{
					replyStream << "Target is no group member";
				}
			}
			else
			{
				replyStream << "You have no target";
			}
		}
		else
		{
			replyStream << "You are not in a group";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "front1")
	{
		std::ostringstream replyStream;
		Unit* targetUnit = pmPlayer->GetTarget();
		if (targetUnit)
		{
			if (pmPlayer->isInFront(targetUnit, ATTACK_DISTANCE))
			{
				replyStream << "Yes";
			}
			else
			{
				replyStream << "No";
			}
		}
		else
		{
			replyStream << "No target";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "front2")
	{
		std::ostringstream replyStream;
		Unit* targetUnit = pmPlayer->GetTarget();
		if (targetUnit)
		{
			if (targetUnit->isInFront(pmPlayer, ATTACK_DISTANCE))
			{
				replyStream << "Yes";
			}
			else
			{
				replyStream << "No";
			}
		}
		else
		{
			replyStream << "No target";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "front3")
	{
		std::ostringstream replyStream;
		Unit* targetUnit = pmPlayer->GetTarget();
		if (targetUnit)
		{
			pmPlayer->GetMotionMaster()->MoveFollow(targetUnit, 5.0f, M_PI / 2);
			replyStream << "Moved";
		}
		else
		{
			replyStream << "No target";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "front4")
	{
		std::ostringstream replyStream;
		Unit* targetUnit = pmPlayer->GetTarget();
		if (targetUnit)
		{
			pmPlayer->SetFacingToObject(targetUnit);
			replyStream << "Set";
		}
		else
		{
			replyStream << "No target";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmPlayer);
	}
	else if (commandName == "front5")
	{

	}
}

bool RobotManager::StringEndWith(const std::string& str, const std::string& tail)
{
	return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool RobotManager::StringStartWith(const std::string& str, const std::string& head)
{
	return str.compare(0, head.size(), head) == 0;
}

Player* RobotManager::GetMaster(uint32 pmSessionID)
{
	if (sRobotManager->robotAICache.find(pmSessionID) != sRobotManager->robotAICache.end())
	{		
		Player* masterPlayer = ObjectAccessor::FindPlayer(sRobotManager->robotAICache[pmSessionID]->masterGUID);
		return masterPlayer;
	}

	return NULL;
}

RobotAI* RobotManager::GetRobotAI(uint32 pmSessionID)
{
	if (sRobotManager->robotAICache.find(pmSessionID) != sRobotManager->robotAICache.end())
	{
		return sRobotManager->robotAICache[pmSessionID];
	}

	return NULL;
}

bool RobotManager::IsRobot(uint32 pmSessionID)
{
	if (sRobotManager->robotAICache.find(pmSessionID) != sRobotManager->robotAICache.end())
	{
		return true;
	}
	return false;
}

std::vector<std::string> RobotManager::SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored)
{
	std::vector<std::string> resultStringVector;
	std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}/*pred*/, delimStr.at(0));
	size_t pos = srcStr.find(delimStr.at(0));
	std::string addedString = "";
	while (pos != std::string::npos) {
		addedString = srcStr.substr(0, pos);
		if (!addedString.empty() || !repeatedCharIgnored) {
			resultStringVector.push_back(addedString);
		}
		srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
		pos = srcStr.find(delimStr.at(0));
	}
	addedString = srcStr;
	if (!addedString.empty() || !repeatedCharIgnored) {
		resultStringVector.push_back(addedString);
	}
	return resultStringVector;
}

std::string RobotManager::TrimString(std::string srcStr)
{
	std::string result = srcStr;
	if (!result.empty())
	{
		result.erase(0, result.find_first_not_of(" "));
		result.erase(result.find_last_not_of(" ") + 1);
	}

	return result;
}
