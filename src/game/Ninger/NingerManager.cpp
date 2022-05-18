#include "NingerManager.h"
#include "NingerStrategies/NingerStrategy_Base.h"
#include "NingerConfig.h"
#include "NingerActions/NingerAction_Base.h"
#include "NingerActions/NingerAction_Druid.h"
#include "NingerActions/NingerAction_Hunter.h"
#include "NingerActions/NingerAction_Mage.h"
#include "NingerActions/NingerAction_Paladin.h"
#include "NingerActions/NingerAction_Priest.h"
#include "NingerActions/NingerAction_Rogue.h"
#include "NingerActions/NingerAction_Shaman.h"
#include "NingerActions/NingerAction_Warlock.h"
#include "NingerActions/NingerAction_Warrior.h"

#include "MingerManager.h"

#include "World/World.h"
#include "Accounts/AccountMgr.h"
#include "Grids/GridNotifiers.h"

// lfm ninger 
#include <boost/chrono/duration.hpp>

NingerManager::NingerManager()
{
	checkDelay = 0;
	allianceRaces.clear();
	hordeRaces.clear();
	nameIndex = 0;
	ningerNameMap.clear();
	ningerEntitySet.clear();
	characterTalentTabNameMap.clear();
}

void NingerManager::InitializeManager()
{
	sLog.outBasic("Initialize ninger");

	checkDelay = sNingerConfig.ManagerCheckDelay;

	allianceRaces.clear();
	allianceRaces[CLASS_WARRIOR][allianceRaces[CLASS_WARRIOR].size()] = RACE_HUMAN;
	allianceRaces[CLASS_WARRIOR][allianceRaces[CLASS_WARRIOR].size()] = RACE_NIGHTELF;
	allianceRaces[CLASS_WARRIOR][allianceRaces[CLASS_WARRIOR].size()] = RACE_GNOME;
	allianceRaces[CLASS_WARRIOR][allianceRaces[CLASS_WARRIOR].size()] = RACE_DWARF;
	hordeRaces[CLASS_WARRIOR][hordeRaces[CLASS_WARRIOR].size()] = RACE_ORC;
	hordeRaces[CLASS_WARRIOR][hordeRaces[CLASS_WARRIOR].size()] = Races::RACE_UNDEAD;
	hordeRaces[CLASS_WARRIOR][hordeRaces[CLASS_WARRIOR].size()] = RACE_TAUREN;
	hordeRaces[CLASS_WARRIOR][hordeRaces[CLASS_WARRIOR].size()] = RACE_TROLL;
	hordeRaces[CLASS_WARRIOR][hordeRaces[CLASS_WARRIOR].size()] = Races::RACE_DRAENEI;

	allianceRaces[CLASS_PALADIN][allianceRaces[CLASS_PALADIN].size()] = RACE_HUMAN;
	allianceRaces[CLASS_PALADIN][allianceRaces[CLASS_PALADIN].size()] = RACE_DWARF;
	hordeRaces[CLASS_PALADIN][hordeRaces[CLASS_PALADIN].size()] = RACE_BLOODELF;
	hordeRaces[Classes::CLASS_PALADIN][hordeRaces[Classes::CLASS_PALADIN].size()] = Races::RACE_DRAENEI;

	allianceRaces[CLASS_ROGUE][allianceRaces[CLASS_ROGUE].size()] = RACE_HUMAN;
	allianceRaces[CLASS_ROGUE][allianceRaces[CLASS_ROGUE].size()] = RACE_DWARF;
	allianceRaces[CLASS_ROGUE][allianceRaces[CLASS_ROGUE].size()] = RACE_NIGHTELF;
	allianceRaces[CLASS_ROGUE][allianceRaces[CLASS_ROGUE].size()] = RACE_GNOME;
	hordeRaces[CLASS_ROGUE][hordeRaces[CLASS_ROGUE].size()] = RACE_ORC;
	hordeRaces[CLASS_ROGUE][hordeRaces[CLASS_ROGUE].size()] = RACE_TROLL;
	hordeRaces[CLASS_ROGUE][hordeRaces[CLASS_ROGUE].size()] = Races::RACE_UNDEAD;
	hordeRaces[Classes::CLASS_ROGUE][hordeRaces[Classes::CLASS_ROGUE].size()] = Races::RACE_BLOODELF;

	allianceRaces[CLASS_PRIEST][allianceRaces[CLASS_PRIEST].size()] = RACE_HUMAN;
	allianceRaces[CLASS_PRIEST][allianceRaces[CLASS_PRIEST].size()] = RACE_DWARF;
	allianceRaces[CLASS_PRIEST][allianceRaces[CLASS_PRIEST].size()] = RACE_NIGHTELF;
	hordeRaces[CLASS_PRIEST][hordeRaces[CLASS_PRIEST].size()] = RACE_TROLL;
	hordeRaces[CLASS_PRIEST][hordeRaces[CLASS_PRIEST].size()] = Races::RACE_UNDEAD;
	hordeRaces[Classes::CLASS_PRIEST][hordeRaces[Classes::CLASS_PRIEST].size()] = Races::RACE_DRAENEI;
	hordeRaces[Classes::CLASS_PRIEST][hordeRaces[Classes::CLASS_PRIEST].size()] = Races::RACE_BLOODELF;

	allianceRaces[CLASS_MAGE][allianceRaces[CLASS_MAGE].size()] = RACE_HUMAN;
	allianceRaces[CLASS_MAGE][allianceRaces[CLASS_MAGE].size()] = RACE_GNOME;
	hordeRaces[CLASS_MAGE][hordeRaces[CLASS_MAGE].size()] = Races::RACE_UNDEAD;
	hordeRaces[CLASS_MAGE][hordeRaces[CLASS_MAGE].size()] = RACE_TROLL;
	hordeRaces[Classes::CLASS_MAGE][hordeRaces[Classes::CLASS_MAGE].size()] = Races::RACE_DRAENEI;
	hordeRaces[Classes::CLASS_MAGE][hordeRaces[Classes::CLASS_MAGE].size()] = Races::RACE_BLOODELF;

	allianceRaces[CLASS_WARLOCK][allianceRaces[CLASS_WARLOCK].size()] = RACE_HUMAN;
	allianceRaces[CLASS_WARLOCK][allianceRaces[CLASS_WARLOCK].size()] = RACE_GNOME;
	hordeRaces[CLASS_WARLOCK][hordeRaces[CLASS_WARLOCK].size()] = Races::RACE_UNDEAD;
	hordeRaces[CLASS_WARLOCK][hordeRaces[CLASS_WARLOCK].size()] = RACE_ORC;
	hordeRaces[Classes::CLASS_WARLOCK][hordeRaces[Classes::CLASS_WARLOCK].size()] = Races::RACE_BLOODELF;

	allianceRaces[CLASS_SHAMAN][allianceRaces[CLASS_SHAMAN].size()] = RACE_DRAENEI;
	hordeRaces[CLASS_SHAMAN][hordeRaces[CLASS_SHAMAN].size()] = RACE_ORC;
	hordeRaces[CLASS_SHAMAN][hordeRaces[CLASS_SHAMAN].size()] = RACE_TAUREN;
	hordeRaces[CLASS_SHAMAN][hordeRaces[CLASS_SHAMAN].size()] = RACE_TROLL;
	hordeRaces[Classes::CLASS_SHAMAN][hordeRaces[Classes::CLASS_SHAMAN].size()] = Races::RACE_DRAENEI;

	allianceRaces[CLASS_HUNTER][allianceRaces[CLASS_HUNTER].size()] = RACE_DWARF;
	allianceRaces[CLASS_HUNTER][allianceRaces[CLASS_HUNTER].size()] = RACE_NIGHTELF;
	hordeRaces[CLASS_HUNTER][hordeRaces[CLASS_HUNTER].size()] = RACE_ORC;
	hordeRaces[CLASS_HUNTER][hordeRaces[CLASS_HUNTER].size()] = RACE_TAUREN;
	hordeRaces[CLASS_HUNTER][hordeRaces[CLASS_HUNTER].size()] = RACE_TROLL;
	hordeRaces[Classes::CLASS_HUNTER][hordeRaces[Classes::CLASS_HUNTER].size()] = Races::RACE_DRAENEI;
	hordeRaces[Classes::CLASS_HUNTER][hordeRaces[Classes::CLASS_HUNTER].size()] = Races::RACE_BLOODELF;

	allianceRaces[CLASS_DRUID][allianceRaces[CLASS_DRUID].size()] = RACE_NIGHTELF;
	hordeRaces[CLASS_DRUID][hordeRaces[CLASS_DRUID].size()] = RACE_TAUREN;

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

	characterTalentTabNameMap[Classes::CLASS_PRIEST][0] = "Discipline";
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

	if (sNingerConfig.Reset)
	{
		DeleteNingers();
	}

	QueryResult* ningerQR = CharacterDatabase.Query("SELECT ninger_id, account_name, character_id, target_level, target_race, target_class, target_specialty FROM ninger");
	if (ningerQR)
	{
		do
		{
			Field* fields = ningerQR->Fetch();
			NingerEntity* re = new NingerEntity();
			re->ninger_id = fields[0].GetUInt32();
			re->account_name = fields[1].GetString();
			re->character_id = fields[2].GetUInt32();
			re->target_level = fields[3].GetUInt32();
			re->target_race = fields[4].GetUInt32();
			re->target_class = fields[5].GetUInt32();
			re->target_specialty = fields[6].GetUInt32();
			ningerEntitySet.insert(re);
		} while (ningerQR->NextRow());
	}
	delete ningerQR;
	nameIndex = 0;
	QueryResult* ningerNameQR = WorldDatabase.Query("SELECT name FROM ninger_names order by rand()");
	if (ningerNameQR)
	{
		do
		{
			Field* fields = ningerNameQR->Fetch();
			std::string eachName = fields[0].GetString();
			ningerNameMap[ningerNameMap.size()] = eachName;
		} while (ningerNameQR->NextRow());
	}
	delete ningerNameQR;

	sLog.outBasic("ninger initialized");
}

NingerManager* NingerManager::instance()
{
	static NingerManager instance;
	return &instance;
}

void NingerManager::UpdateNingerManager(uint32 pmDiff)
{
	if (sNingerConfig.Enable == 0)
	{
		return;
	}

	if (checkDelay >= 0)
	{
		checkDelay -= pmDiff;
	}
	if (checkDelay < 0)
	{
		checkDelay = sNingerConfig.ManagerCheckDelay;
		std::unordered_set<uint32> onlinePlayerLevelSet;
		onlinePlayerLevelSet.clear();
		std::unordered_map<uint32, WorldSession*> allSessions = sWorld.GetAllSessions();
		for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
		{
			if (WorldSession* eachWS = wsIT->second)
			{
				if (!eachWS->isNinger)
				{
					if (Player* eachPlayer = eachWS->GetPlayer())
					{
						uint32 eachLevel = eachPlayer->GetLevel();
						if (onlinePlayerLevelSet.find(eachLevel) == onlinePlayerLevelSet.end())
						{
							onlinePlayerLevelSet.insert(eachLevel);
						}
					}
				}
			}
		}
		for (std::unordered_set<uint32>::iterator levelIT = onlinePlayerLevelSet.begin(); levelIT != onlinePlayerLevelSet.end(); levelIT++)
		{
			uint32 eachLevel = *levelIT;
			LoginNinger(eachLevel, sNingerConfig.NingerCountEachLevel);
		}
	}
}

void NingerManager::UpdateNingerEntities(uint32 pmDiff)
{
	for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
	{
		(*reIT)->Update(pmDiff);
	}
}

void NingerManager::LogoutNingers(bool pmInstant)
{
	for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
	{
		if (NingerEntity* eachNinger = *reIT)
		{
			if (pmInstant)
			{
				ObjectGuid playerGuid = ObjectGuid(HighGuid::HIGHGUID_PLAYER, eachNinger->character_id);
				if (Player* eachPlayer = ObjectAccessor::FindPlayer(playerGuid))
				{
					std::ostringstream logStream;
					logStream << "Logout ninger : " << eachPlayer->GetNameStr();
					sLog.outBasic(logStream.str().c_str());
					eachPlayer->GetSession()->LogoutPlayer();
				}
			}
			else
			{
				eachNinger->entityState = NingerEntityState::NingerEntityState_DoLogoff;
				eachNinger->checkDelay = 10;
			}
		}
	}
}

void NingerManager::DeleteNingers()
{
	QueryResult* ningerQR = CharacterDatabase.Query("SELECT account_id FROM ninger");
	if (ningerQR)
	{
		do
		{
			Field* fields = ningerQR->Fetch();
			uint32 eachAccountId = fields[0].GetUInt32();
			if (eachAccountId > 0)
			{
				sAccountMgr.DeleteAccount(eachAccountId);
			}
		} while (ningerQR->NextRow());
	}
	CharacterDatabase.DirectExecute("delete from ninger");
	delete ningerQR;

	std::ostringstream accountQueryStream;
	accountQueryStream << "SELECT id FROM account where username like '" << NINGER_MARK << "%'";
	std::string accountQuerySQL = accountQueryStream.str();
	QueryResult* ningerAccountQR = LoginDatabase.Query(accountQuerySQL.c_str());
	if (ningerAccountQR)
	{
		do
		{
			Field* fields = ningerAccountQR->Fetch();
			uint32 eachAccountId = fields[0].GetUInt32();
			if (eachAccountId > 0)
			{
				sAccountMgr.DeleteAccount(eachAccountId);
			}
		} while (ningerAccountQR->NextRow());
	}
	delete ningerAccountQR;
}

bool NingerManager::LoginNinger(uint32 pmLevel, uint32 pmCount)
{
	if (pmLevel >= 20)
	{
		uint32 currentCount = 0;
		QueryResult* levelNingerQR = CharacterDatabase.PQuery("SELECT count(*) FROM ninger where target_level = %d", pmLevel);
		if (levelNingerQR)
		{
			Field* fields = levelNingerQR->Fetch();
			currentCount = fields[0].GetUInt32();
		}
		delete levelNingerQR;
		if (currentCount < pmCount)
		{
			uint32 addCount = pmCount - currentCount;
			uint32 allianceCount = addCount / 2;
			uint32 hordeCount = addCount - allianceCount;
			int checkCount = allianceCount;
			int tankA = 1;
			int tankH = 1;
			int healerA = 1;
			int healerH = 1;
			while (checkCount > 0)
			{
				uint32 groupRole = GroupRole::GroupRole_DPS;
				if (tankA > 0)
				{
					groupRole = GroupRole::GroupRole_Tank;
					tankA--;
				}
				else if (healerA > 0)
				{
					groupRole = GroupRole::GroupRole_Healer;
					healerA--;
				}
				CreateNinger(pmLevel, true, groupRole);
				checkCount--;
			}
			checkCount = hordeCount;
			while (checkCount > 0)
			{
				uint32 groupRole = GroupRole::GroupRole_DPS;
				if (tankH > 0)
				{
					groupRole = GroupRole::GroupRole_Tank;
					tankH--;
				}
				else if (healerH > 0)
				{
					groupRole = GroupRole::GroupRole_Healer;
					healerH--;
				}
				CreateNinger(pmLevel, false, groupRole);
				checkCount--;
			}
		}
		uint32 onlineCount = 0;
		for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
		{
			if (NingerEntity* eachRE = *reIT)
			{
				if (eachRE->target_level == pmLevel)
				{
					if (eachRE->entityState != NingerEntityState::NingerEntityState_OffLine && eachRE->entityState != NingerEntityState::NingerEntityState_None)
					{
						onlineCount++;
					}
				}
			}
		}
		int toOnline = 0;
		if (pmCount > onlineCount)
		{
			toOnline = pmCount - onlineCount;
		}
		if (toOnline > 0)
		{
			for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
			{
				if (NingerEntity* eachRE = *reIT)
				{
					if (eachRE->target_level == pmLevel)
					{
						if (eachRE->entityState == NingerEntityState::NingerEntityState_OffLine)
						{
							eachRE->checkDelay = urand(5 * IN_MILLISECONDS, 5 * MINUTE * IN_MILLISECONDS);
							eachRE->entityState = NingerEntityState::NingerEntityState_Enter;
							toOnline--;
							if (toOnline <= 0)
							{
								break;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

void NingerManager::CreateNinger(uint32 pmLevel, bool pmAlliance, uint32 pmGroupRole)
{
	uint32 currentNingerCount = 0;
	QueryResult* ningerQR = CharacterDatabase.Query("SELECT count(*) FROM ninger");
	if (ningerQR)
	{
		Field* fields = ningerQR->Fetch();
		currentNingerCount = fields[0].GetUInt32();
	}
	delete ningerQR;
	std::ostringstream accountNameStream;
	accountNameStream << NINGER_MARK << currentNingerCount;
	std::string checkAccountName = accountNameStream.str();
	std::ostringstream querySQLStream;
	querySQLStream << "SELECT * FROM account where username ='" << checkAccountName << "'";
	std::string querySQL = querySQLStream.str();
	QueryResult* accountNameQR = LoginDatabase.Query(querySQL.c_str());
	if (accountNameQR)
	{
		sLog.outBasic("Account already %s exists.", checkAccountName);
	}
	else
	{
		uint32 target_class = Classes::CLASS_HUNTER;
		uint32 target_specialty = 1;
		uint32 classRand = urand(1, 100);
		if (pmGroupRole == GroupRole::GroupRole_Tank)
		{
			target_class = Classes::CLASS_WARRIOR;
			target_specialty = 2;
		}
		else if (pmGroupRole == GroupRole::GroupRole_Healer)
		{
			target_class = Classes::CLASS_PRIEST;
			target_specialty = 0;
		}
		else
		{
			if (classRand < 30)
			{
				target_class = Classes::CLASS_ROGUE;
				target_specialty = 1;
			}
			else if (classRand < 60)
			{
				target_class = Classes::CLASS_DRUID;
				target_specialty = 0;
			}
			//else if (classRand < 60)
			//{
			//	target_class = Classes::CLASS_HUNTER;
			//	target_specialty = 1;
			//}
			else if (classRand < 200)
			{
				target_class = Classes::CLASS_MAGE;
				target_specialty = 1;
			}
		}

		// lfm debug
		//target_class = Classes::CLASS_ROGUE;
		//target_specialty = 1;

		uint32 target_race = 0;
		if (pmAlliance)
		{
			uint32 raceIndex = urand(0, allianceRaces[target_class].size() - 1);
			target_race = allianceRaces[target_class][raceIndex];
		}
		else
		{
			uint32 raceIndex = urand(0, hordeRaces[target_class].size() - 1);
			target_race = hordeRaces[target_class][raceIndex];
		}
		std::ostringstream sqlStream;
		sqlStream << "INSERT INTO ninger (ninger_id,account_id, account_name, character_id, target_level, target_race, target_class, target_specialty) VALUES (" << currentNingerCount << ", 0, '" << checkAccountName << "', 0, " << pmLevel << ", " << target_race << ", " << target_class << ", " << target_specialty << ")";
		std::string sql = sqlStream.str();
		CharacterDatabase.DirectExecute(sql.c_str());
		NingerEntity* re = new NingerEntity();
		re->ninger_id = currentNingerCount;
		re->account_id = 0;
		re->account_name = checkAccountName;
		re->character_id = 0;
		re->target_level = pmLevel;
		re->target_race = target_race;
		re->target_class = target_class;
		re->target_specialty = target_specialty;
		ningerEntitySet.insert(re);
		std::ostringstream replyStream;
		replyStream << "ninger " << checkAccountName << " created";
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str());
	}
	delete accountNameQR;
}

bool NingerManager::IsPolymorphed(Unit* pmTarget)
{
	if (pmTarget)
	{
		if (pmTarget->HasAura(118) || pmTarget->HasAura(12824) || pmTarget->HasAura(12825) || pmTarget->HasAura(12826))
		{
			return true;
		}
		if (pmTarget->HasAura(2637) || pmTarget->HasAura(18657) || pmTarget->HasAura(18658))
		{
			return true;
		}
		if (pmTarget->HasAura(339) || pmTarget->HasAura(1062) || pmTarget->HasAura(5195) || pmTarget->HasAura(5196) || pmTarget->HasAura(9852) || pmTarget->HasAura(9853) || pmTarget->HasAura(26989) || pmTarget->HasAura(53308))
		{
			return true;
		}
	}

	return false;
}

void NingerManager::HandleChatCommand(Player* pmCommander, std::string pmContent, Player* pmTargetPlayer, Group* pmTargetGroup)
{
	if (!pmCommander)
	{
		return;
	}
	std::vector<std::string> commandVector = sMingerManager->SplitString(pmContent, " ", true);
	std::string commandName = commandVector.at(0);
	if (commandName == "role")
	{
		if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
		else
		{
			Player* targetPlayer = pmTargetPlayer;
			if (!targetPlayer)
			{
				targetPlayer = pmCommander;
			}
			std::ostringstream replyStream;
			if (commandVector.size() > 1)
			{
				std::string newRole = commandVector.at(1);
				if (newRole == "tank")
				{
					targetPlayer->groupRole = GroupRole::GroupRole_Tank;
				}
				else if (newRole == "healer")
				{
					targetPlayer->groupRole = GroupRole::GroupRole_Healer;
				}
				else if (newRole == "dps")
				{
					targetPlayer->groupRole = GroupRole::GroupRole_DPS;
				}
			}
			if (targetPlayer->groupRole == GroupRole::GroupRole_Tank)
			{
				replyStream << "Role is tank";
			}
			else if (targetPlayer->groupRole == GroupRole::GroupRole_Healer)
			{
				replyStream << "Role is healer";
			}
			else if (targetPlayer->groupRole == GroupRole::GroupRole_DPS)
			{
				replyStream << "Role is dps";
			}
			if (pmTargetPlayer)
			{
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
			else
			{
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
			}
		}
	}
	else if (commandName == "who")
	{
		if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
		else if (pmTargetPlayer)
		{
			std::ostringstream replyStream;
			replyStream << characterTalentTabNameMap[pmTargetPlayer->getClass()][pmTargetPlayer->ningerAction->specialty];
			pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
		}
	}
	else if (commandName == "arrangement")
	{
		std::ostringstream replyStream;
		if (pmTargetGroup)
		{
			if (pmTargetGroup->GetLeaderGuid() == pmCommander->GetObjectGuid())
			{
				ObjectGuid ogTank = ObjectGuid();
				ObjectGuid ogHealer = ObjectGuid();
				int rti = 0;
				for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
				{
					if (Player* member = groupRef->getSource())
					{
						if (member->GetObjectGuid() == pmCommander->GetObjectGuid())
						{
							continue;
						}
						uint32 mapId = member->GetMapId();
						if (mapId == StrategyIndex::StrategyIndex_The_Underbog)
						{
							member->activeStrategyIndex = StrategyIndex::StrategyIndex_The_Underbog;
						}
						else if (mapId == 269)
						{
							member->activeStrategyIndex = StrategyIndex::StrategyIndex_The_Black_Morass;
						}
						if (NingerStrategy_Base* ns = member->strategyMap[member->activeStrategyIndex])
						{
							ns->Reset();
							member->groupRole = GroupRole::GroupRole_DPS;
							switch (member->getClass())
							{
							case Classes::CLASS_WARRIOR:
							{
								member->groupRole = GroupRole::GroupRole_Tank;
								ogTank = member->GetObjectGuid();
								break;
							}
							case Classes::CLASS_HUNTER:
							{
								break;
							}
							case Classes::CLASS_MAGE:
							{
								ns->rti = rti;
								rti++;
								break;
							}
							case Classes::CLASS_DRUID:
							{
								ns->rti = rti;
								rti++;
								break;
							}
							case Classes::CLASS_ROGUE:
							{
								ns->forceBack = true;
								break;
							}
							case Classes::CLASS_PRIEST:
							{
								member->groupRole = GroupRole::GroupRole_Healer;
								ogHealer = member->GetObjectGuid();
								break;
							}
							default:
							{
								break;
							}
							}
						}
						member->ningerAction->Prepare();
					}
				}
				for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
				{
					if (Player* member = groupRef->getSource())
					{
						if (member->GetObjectGuid() == pmCommander->GetObjectGuid())
						{
							continue;
						}
						if (NingerStrategy_Base* ns = member->strategyMap[member->activeStrategyIndex])
						{
							ns->ogTank = ogTank;
							ns->ogHealer = ogHealer;
							ns->Report();							
						}
					}
				}
				replyStream << "Arranged";
			}
			else
			{
				replyStream << "You are not leader";
			}
		}
		else
		{
			replyStream << "Not in a group";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
	}
	else if (commandName == "instance")
	{
		std::ostringstream replyStream;
		if (pmTargetPlayer)
		{
			if (pmTargetPlayer->GetSession()->isNinger)
			{
				pmTargetPlayer->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
				pmTargetPlayer->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, true);
				replyStream << "Instance reset";
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
					}
				}
			}
		}
	}
	else if (commandName == "updateentry")
	{
		if (Unit* target = pmCommander->GetTarget())
		{
			if (Creature* targetC = (Creature*)target)
			{
				if (commandVector.size() > 1)
				{
					std::string checkEntry = commandVector.at(1);
					uint32 entry = atoi(checkEntry.c_str());
					targetC->UpdateEntry(entry);
				}
			}
		}
	}
	else if (commandName == "debug")
	{
		const SpellEntry* pS1 = sSpellTemplate.LookupEntry<SpellEntry>(27089);
		const SpellEntry* pS2 = sSpellTemplate.LookupEntry<SpellEntry>(43182);
		bool end = true;
	}
	else if (commandName == "vimgol")
	{
		if (pmTargetGroup)
		{
			CreatureList visualList;
			MaNGOS::AllCreaturesOfEntryInRangeCheck checker(pmCommander, 23040, VISIBILITY_DISTANCE_NORMAL);
			MaNGOS::CreatureListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(visualList, checker);
			Cell::VisitGridObjects(pmCommander, searcher, VISIBILITY_DISTANCE_NORMAL);

			std::unordered_set<ObjectGuid> ogGoingSet;
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				if (Player* member = groupRef->getSource())
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						for (auto& eachVisual : visualList)
						{
							if (ogGoingSet.find(eachVisual->GetObjectGuid()) == ogGoingSet.end())
							{
								if (NingerStrategy_Base* ns = member->strategyMap[member->activeStrategyIndex])
								{
									ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Hold;
									ns->restLimit = 0;
									ns->actionType = ActionType::ActionType_Move;
									ns->actionLimit = 30000;
									member->ningerAction->nm->Point(eachVisual->GetPosition(), ns->actionLimit);
									//std::ostringstream replyStream;
									//replyStream << "Goting to visual : " << eachVisual->GetObjectGuid().GetCounter();
									//member->Say(replyStream.str(), Language::LANG_UNIVERSAL);
									ogGoingSet.insert(eachVisual->GetObjectGuid());
								}
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			HandleChatCommand(pmCommander, pmContent, pmCommander, pmTargetGroup);
		}
	}
	else if (commandName == "distance")
	{
		if (pmTargetPlayer)
		{
			if (Unit* distanceTarget = pmCommander->GetTarget())
			{
				float distance = pmTargetPlayer->GetDistance(distanceTarget, true, DistanceCalculation::DIST_CALC_BOUNDING_RADIUS);
				std::ostringstream replyStream0;
				replyStream0 << "bounding : " << distance;
				pmTargetPlayer->Say(replyStream0.str(), Language::LANG_UNIVERSAL);
				std::ostringstream replyStream1;
				distance = pmTargetPlayer->GetDistance(distanceTarget, true, DistanceCalculation::DIST_CALC_COMBAT_REACH);
				replyStream1 << "combat : " << distance;
				pmTargetPlayer->Say(replyStream1.str(), Language::LANG_UNIVERSAL);
				std::ostringstream replyStream2;
				distance = pmTargetPlayer->GetDistance(distanceTarget, true, DistanceCalculation::DIST_CALC_COMBAT_REACH_WITH_MELEE);
				replyStream2 << "melee : " << distance;
				pmTargetPlayer->Say(replyStream2.str(), Language::LANG_UNIVERSAL);
				std::ostringstream replyStream3;
				distance = pmTargetPlayer->GetDistance(distanceTarget, true, DistanceCalculation::DIST_CALC_NONE);
				replyStream3 << "none : " << distance;
				pmTargetPlayer->Say(replyStream3.str(), Language::LANG_UNIVERSAL);
			}
			else
			{
				std::ostringstream replyStream;
				replyStream << "No target";
				pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
					}
				}
			}
		}
		else
		{
			HandleChatCommand(pmCommander, pmContent, pmCommander, pmTargetGroup);
		}
	}
	else if (commandName == "apexis")
	{
		std::ostringstream replyStream;
		if (pmTargetPlayer)
		{
			if (commandVector.size() > 1)
			{
				std::string color = commandVector.at(1);
				uint32 entry = 0;
				if (color == "red")
				{
					entry = 185794;
				}
				else if (color == "yellow")
				{
					entry = 185792;
				}
				else if (color == "blue")
				{
					entry = 185795;
				}
				else if (color == "green")
				{
					entry = 185793;
				}
				if (entry > 0)
				{
					if (GameObject* pGo = GetClosestGameObjectWithEntry(pmTargetPlayer, entry, INTERACTION_DISTANCE))
					{
						pGo->Use(pmTargetPlayer);
						replyStream << "Use GO : " << pGo->GetObjectGuid().GetCounter();
					}
				}
				else
				{
					replyStream << "No color";
				}
			}
			else
			{
				replyStream << "No color";
			}
			pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
					}
				}
			}
		}
		else
		{
			HandleChatCommand(pmCommander, pmContent, pmCommander);
		}
	}
	else if (commandName == "nearpoint")
	{
		std::ostringstream replyStream;
		if (pmTargetPlayer)
		{
			if (Unit* distanceTarget = pmCommander->GetTarget())
			{
				float distance = 10.0f;
				if (commandVector.size() > 1)
				{
					std::string checkStr = commandVector.at(1);
					distance = atof(checkStr.c_str());
				}
				pmTargetPlayer->ningerAction->nm->Chase(distanceTarget, distance, 0.0f, false, false);
				replyStream << "chasing : " << distanceTarget->GetName();
				pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
			}
			else
			{
				replyStream << "No target";
			}
			pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
					}
				}
			}
		}
		else
		{
			HandleChatCommand(pmCommander, pmContent, pmCommander);
		}
	}
	else if (commandName == "closepoint")
	{
		std::ostringstream replyStream;
		if (pmTargetPlayer)
		{
			if (Unit* distanceTarget = pmCommander->GetTarget())
			{
				float distance = 10.0f;
				if (commandVector.size() > 1)
				{
					std::string checkStr = commandVector.at(1);
					distance = atof(checkStr.c_str());
				}
				float commanderAngle = distanceTarget->GetAngle(pmCommander);
				float destX = 0.0f, destY = 0.0f, destZ = 0.0f;
				distanceTarget->GetClosePoint(destX, destY, destZ, distanceTarget->GetObjectBoundingRadius(), distance, commanderAngle, pmTargetPlayer);
				pmTargetPlayer->GetMotionMaster()->MovePoint(0, destX, destY, destZ, ForcedMovement::FORCED_MOVEMENT_RUN, true);
			}
			else
			{
				replyStream << "No target";
			}
			pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
					}
				}
			}
		}
		else
		{
			HandleChatCommand(pmCommander, pmContent, pmCommander, pmTargetGroup);
		}
	}
	else if (commandName == "assemble")
	{
		std::ostringstream replyStream;
		if (Player* teleportTarget = ObjectAccessor::FindPlayer(pmCommander->GetSelectionGuid()))
		{
			if (pmTargetPlayer)
			{
				if (teleportTarget->GetObjectGuid() != pmTargetPlayer->GetObjectGuid())
				{
					pmTargetPlayer->teleportTargetGuid = teleportTarget->GetObjectGuid().GetCounter();
					if (!pmTargetPlayer->IsAlive())
					{
						pmTargetPlayer->teleportDelay = sNingerConfig.CorpseRunDelay;
						pmTargetPlayer->reviveDelay = sNingerConfig.CorpseRunDelay + 5000;
						replyStream << "I am dead, Prepare to do corpserun to " << teleportTarget->GetName() << " in " << pmTargetPlayer->teleportDelay / 1000 << " seconds";
					}
					else
					{
						pmTargetPlayer->teleportDelay = sNingerConfig.TeleportDelay;
						replyStream << "Prepare to teleport to " << teleportTarget->GetName() << " in " << pmTargetPlayer->teleportDelay / 1000 << " seconds";
					}
					if (pmTargetPlayer->GetObjectGuid() == pmCommander->GetObjectGuid())
					{
						sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
					}
					else
					{
						pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
			}
			else if (pmTargetGroup)
			{
				for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
				{
					Player* member = groupRef->getSource();
					if (member)
					{
						if (member->GetObjectGuid() != teleportTarget->GetObjectGuid())
						{
							HandleChatCommand(pmCommander, pmContent, member, pmTargetGroup);
						}
					}
				}
			}
			else
			{
				HandleChatCommand(pmCommander, pmContent, pmCommander);
			}
		}
		else
		{
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, "No target", pmCommander);
		}
	}
	else if (commandName == "leader")
	{
		if (pmTargetGroup)
		{
			if (pmTargetGroup->GetLeaderGuid() != pmCommander->GetObjectGuid())
			{
				bool change = true;
				if (Player* leader = ObjectAccessor::FindPlayer(pmTargetGroup->GetLeaderGuid()))
				{
					if (WorldSession* leaderSession = leader->GetSession())
					{
						if (!leaderSession->isNinger)
						{
							change = false;
						}
					}
				}
				if (change)
				{
					pmTargetGroup->ChangeLeader(pmCommander->GetObjectGuid());
				}
				else
				{
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, "Leader is valid", pmCommander);
				}
			}
			else
			{
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, "You are the leader", pmCommander);
			}
		}
		else
		{
			sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, "You are not in a group", pmCommander);
		}
	}
	else if (commandName == "ninger")
	{
		if (commandVector.size() > 1)
		{
			std::string ningerAction = commandVector.at(1);
			if (ningerAction == "reset")
			{
				std::ostringstream replyStream;
				bool allOffline = true;
				for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
				{
					if (NingerEntity* eachRE = *reIT)
					{
						if (eachRE->entityState != NingerEntityState::NingerEntityState_None && eachRE->entityState != NingerEntityState::NingerEntityState_OffLine)
						{
							allOffline = false;
							replyStream << "Not all ningers are offline. Going offline first";
							LogoutNingers();
							break;
						}
					}
				}
				if (allOffline)
				{
					replyStream << "All ningers are offline. Ready to delete";
					DeleteNingers();
				}
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
			}
			else if (ningerAction == "offline")
			{
				std::ostringstream replyStream;
				replyStream << "All ningers are going offline";
				LogoutNingers();
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
			}
			else if (ningerAction == "online")
			{
				uint32 playerLevel = pmCommander->GetLevel();
				if (playerLevel < 10)
				{
					std::ostringstream replyStream;
					replyStream << "You level is too low";
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
				}
				else
				{
					uint32 ningerCount = sNingerConfig.NingerCountEachLevel;
					if (commandVector.size() > 2)
					{
						ningerCount = atoi(commandVector.at(2).c_str());
					}
					if (ningerCount > 0)
					{
						std::ostringstream replyTitleStream;
						replyTitleStream << "ninger count to go online : " << ningerCount;
						sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyTitleStream.str().c_str(), pmCommander);
						LoginNinger(playerLevel, ningerCount);
					}
				}
			}
			else if (ningerAction == "relocate")
			{
				std::unordered_map<uint32, WorldSession*> allSessions = sWorld.GetAllSessions();
				for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
				{
					if (WorldSession* eachWS = wsIT->second)
					{
						if (eachWS->isNinger)
						{
							if (Player* eachNinger = eachWS->GetPlayer())
							{
								if (eachNinger->IsInWorld())
								{
									eachNinger->TeleportToHomebind();

									std::ostringstream replyTitleStream;
									replyTitleStream << "Teleport ninger to homebind : " << eachNinger->GetName();
									sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyTitleStream.str().c_str(), pmCommander);
								}
							}
						}
					}
				}
			}
		}
	}
	else if (commandName == "engage")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze || ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Glue)
				{
					ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
				}
				if (Unit* target = pmCommander->GetTarget())
				{
					if (ns->Engage(target))
					{
						int engageLimit = DEFAULT_ACTION_LIMIT_DELAY;
						if (commandVector.size() > 1)
						{
							std::string checkStr = commandVector.at(1);
							engageLimit = atoi(checkStr.c_str());
						}
						ns->actionLimit = engageLimit;
						ns->ogActionTarget = target->GetObjectGuid();
						ns->actionType = ActionType::ActionType_Engage;
						std::ostringstream replyStream;
						replyStream << "Try to engage " << target->GetName();
						pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "tank")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze || ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Glue)
				{
					ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
				}
				if (Unit* target = pmCommander->GetTarget())
				{
					if (ns->Tank(target))
					{
						int engageLimit = DEFAULT_ACTION_LIMIT_DELAY;
						if (commandVector.size() > 1)
						{
							std::string checkStr = commandVector.at(1);
							engageLimit = atoi(checkStr.c_str());
						}
						ns->actionLimit = engageLimit;
						ns->ogActionTarget = target->GetObjectGuid();
						ns->actionType = ActionType::ActionType_Engage;
						std::ostringstream replyStream;
						replyStream << "Try to engage " << target->GetName();
						pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "hold")
	{
		if (pmTargetPlayer)
		{
			bool doAction = true;
			if (commandVector.size() > 1)
			{
				std::string role = commandVector.at(1);
				if (role == "tank")
				{
					if (pmTargetPlayer->groupRole != GroupRole::GroupRole_Tank)
					{
						doAction = false;
					}
				}
				else if (role == "healer")
				{
					if (pmTargetPlayer->groupRole != GroupRole::GroupRole_Healer)
					{
						doAction = false;
					}
				}
				else if (role == "dps")
				{
					if (pmTargetPlayer->groupRole != GroupRole::GroupRole_DPS)
					{
						doAction = false;
					}
				}
				else if (role == "melee")
				{
					uint32 myClass = pmTargetPlayer->getClass();
					if (myClass != Classes::CLASS_WARRIOR && myClass != Classes::CLASS_ROGUE && myClass != Classes::CLASS_PALADIN)
					{
						doAction = false;
					}
				}
			}
			if (doAction)
			{
				if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
				{
					ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Hold;
					pmTargetPlayer->StopMoving(true);
					pmTargetPlayer->GetMotionMaster()->Clear();
					pmTargetPlayer->GetMotionMaster()->MovementExpired(true);
					std::ostringstream replyStream;
					replyStream << "Holding";
					pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "dev0")
	{
		if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						member->SetSelectionGuid(ObjectGuid());
						member->SetTarget(nullptr);
						member->CombatStop(true);
						member->StopMoving(true);
						member->GetMotionMaster()->Clear();
						member->AttackStop();
						member->InterruptNonMeleeSpells(true);
						member->ResurrectPlayer(1.0f);
						member->TeleportTo(pmCommander->GetMapId(), pmCommander->GetPositionX(), pmCommander->GetPositionY(), pmCommander->GetPositionZ(), pmCommander->GetOrientation());
					}
				}
			}
		}
	}
	else if (commandName == "caution")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (commandVector.size() > 1)
				{
					std::string cautionTypeStr = commandVector.at(1);
					if (commandVector.size() > 2)
					{
						std::ostringstream nameStream;
						nameStream << commandVector.at(2);
						for (int nameIndex = 3; nameIndex < commandVector.size(); nameIndex++)
						{
							nameStream << " " << commandVector.at(nameIndex);
						}
						std::string spellNameStr = nameStream.str();
						if (cautionTypeStr == "add")
						{
							std::ostringstream queryStream;
							queryStream << "SELECT Id FROM spell_template where SpellName = '" << spellNameStr << "'";
							QueryResult* spellQR = WorldDatabase.Query(queryStream.str().c_str());
							if (spellQR)
							{
								do
								{
									Field* fields = spellQR->Fetch();
									uint32 eachId = fields[0].GetUInt32();
									if (ns->cautionSpellMap.find(spellNameStr) == ns->cautionSpellMap.end())
									{
										ns->cautionSpellMap[spellNameStr].insert(eachId);
									}
									else
									{
										std::unordered_set<uint32> eachIDSet = ns->cautionSpellMap.find(spellNameStr)->second;
										if (eachIDSet.find(eachId) == eachIDSet.end())
										{
											ns->cautionSpellMap[spellNameStr].insert(eachId);
										}
									}
								} while (spellQR->NextRow());
							}
							delete spellQR;
						}
						else if (cautionTypeStr == "remove")
						{
							ns->cautionSpellMap.erase(spellNameStr);
						}
					}
					else if (cautionTypeStr == "clear")
					{
						ns->cautionSpellMap.clear();
					}
				}
				for (std::unordered_map<std::string, std::unordered_set<uint32>>::iterator nameIT = ns->cautionSpellMap.begin(); nameIT != ns->cautionSpellMap.end(); nameIT++)
				{
					for (std::unordered_set<uint32>::iterator idIT = nameIT->second.begin(); idIT != nameIT->second.end(); idIT++)
					{
						std::ostringstream detailsStream;
						detailsStream << "Caution : " << nameIT->first << " - " << *idIT;
						pmTargetPlayer->Whisper(detailsStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "freeze")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Freeze;
				pmTargetPlayer->ningerAction->nm->ResetMovement();
				pmTargetPlayer->InterruptNonMeleeSpells(true);
				pmTargetPlayer->AttackStop();
				pmTargetPlayer->ningerAction->PetStop();
				std::ostringstream replyStream;
				replyStream << "Freezed";
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "prepare")
	{
		if (pmTargetPlayer)
		{
			pmTargetPlayer->ningerAction->Prepare();
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "rest")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				ns->Rest(true);
				pmTargetPlayer->Whisper("Resting", Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "revive")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (Unit* target = pmCommander->GetTarget())
				{
					if (ns->Revive(target))
					{
						ns->actionLimit = DEFAULT_ACTION_LIMIT_DELAY;
						ns->ogActionTarget = target->GetObjectGuid();
						ns->actionType = ActionType::ActionType_Revive;
						std::ostringstream replyStream;
						replyStream << "Try to do reviving";
						pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
				else if (ns->Revive())
				{
					std::ostringstream replyStream;
					replyStream << "Try to do reviving";
					pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "follow")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string cmdDistanceStr = commandVector.at(1);
					float cmdDistance = atof(cmdDistanceStr.c_str());
					if (cmdDistance >= FOLLOW_MIN_DISTANCE && cmdDistance <= FOLLOW_MAX_DISTANCE)
					{
						ns->followDistance = cmdDistance;
						replyStream << "Follow distance - " << ns->followDistance;
					}
					else
					{
						replyStream << "Distance is not valid";
					}
				}
				else
				{
					ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
					ns->restLimit = 0;
					ns->actionLimit = 0;
					replyStream << "Following - " << ns->followDistance;
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "chase")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string cmdDistanceStr = commandVector.at(1);
					float cmdDistance = atof(cmdDistanceStr.c_str());
					if (cmdDistance >= MELEE_MIN_DISTANCE && cmdDistance <= RANGE_MAX_DISTANCE)
					{
						ns->dpsDistance = cmdDistance;
					}
					else
					{
						replyStream << "Distance is not valid";
					}
				}
				replyStream << "Chase distance - " << ns->dpsDistance;
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "gather")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (pmTargetPlayer->IsAlive())
				{
					if (pmCommander->GetDistance(pmTargetPlayer) < FOLLOW_MAX_DISTANCE)
					{
						ns->restLimit = 0;
						ns->actionType = ActionType::ActionType_Move;
						ns->actionLimit = 3 * IN_MILLISECONDS;
						pmTargetPlayer->ningerAction->nm->Point(pmCommander->GetPosition(), ns->actionLimit);
						replyStream << "I will move to you";
					}
					else
					{
						replyStream << "I am too far away";
					}
				}
				else
				{
					replyStream << "I am dead";
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "move")
	{
		if (pmTargetPlayer)
		{
			std::ostringstream replyStream;
			if (pmTargetPlayer->IsAlive())
			{
				if (commandVector.size() > 1)
				{
					bool doAction = true;
					std::string direction = commandVector.at(1);
					uint32 directionType = NingerMovementDirection::NingerMovementDirection_Forward;
					if (direction == "forward")
					{
						directionType = NingerMovementDirection::NingerMovementDirection_Forward;
					}
					else if (direction == "back")
					{
						directionType = NingerMovementDirection::NingerMovementDirection_Back;
					}
					else if (direction == "left")
					{
						directionType = NingerMovementDirection::NingerMovementDirection_Left;
					}
					else if (direction == "right")
					{
						directionType = NingerMovementDirection::NingerMovementDirection_Right;
					}
					else
					{
						doAction = false;
						replyStream << "Direction invalid";
					}
					if (doAction)
					{
						if (commandVector.size() > 2)
						{
							std::string role = commandVector.at(2);
							if (role == "all")
							{
								doAction = true;
							}
							else if (role == "tank")
							{
								if (pmTargetPlayer->groupRole != GroupRole::GroupRole_Tank)
								{
									doAction = false;
								}
							}
							else if (role == "healer")
							{
								if (pmTargetPlayer->groupRole != GroupRole::GroupRole_Healer)
								{
									doAction = false;
								}
							}
							else if (role == "dps")
							{
								if (pmTargetPlayer->groupRole != GroupRole::GroupRole_DPS)
								{
									doAction = false;
								}
							}
							else if (role == "melee")
							{
								uint32 myClass = pmTargetPlayer->getClass();
								if (myClass != Classes::CLASS_WARRIOR && myClass != Classes::CLASS_ROGUE && myClass != Classes::CLASS_PALADIN)
								{
									doAction = false;
								}
							}
							else
							{
								doAction = false;
								replyStream << "Role invalid";
							}
						}
						else
						{
							doAction = false;
							replyStream << "No role";
						}
						if (doAction)
						{
							if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
							{
								uint32 distance = 5;
								if (commandVector.size() > 3)
								{
									std::string distanceStr = commandVector.at(3);
									distance = std::atoi(distanceStr.c_str());
								}
								uint32 actionLimit = distance / 5;
								actionLimit = actionLimit * IN_MILLISECONDS;
								if (pmTargetPlayer->ningerAction->nm->Direction(pmCommander, directionType, actionLimit, distance))
								{
									ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Hold;
									ns->restLimit = 0;
									ns->actionType = ActionType::ActionType_Move;
									ns->actionLimit = actionLimit;
									replyStream << direction;
								}
								pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
							}
						}
					}
				}
				else
				{
					replyStream << "No direction";
				}
			}
			else
			{
				replyStream << "I am dead";
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "cast")
	{
		if (pmTargetPlayer)
		{
			std::ostringstream replyStream;
			if (Unit* targetU = pmCommander->GetTarget())
			{
				if (commandVector.size() > 1)
				{
					uint32 spellId = std::stoi(commandVector.at(1));
					if (pmTargetPlayer->HasActiveSpell(spellId))
					{
						pmTargetPlayer->InterruptNonMeleeSpells(true);
						SpellCastResult scr = pmTargetPlayer->CastSpell(targetU, spellId, TriggerCastFlags::TRIGGERED_NONE);
						replyStream << "Cast spell : " << scr;
					}
				}
				else
				{
					replyStream << "Spell name missing";
				}
			}
			else
			{
				replyStream << "No target";
			}
			pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "unaura")
	{
		if (Unit* targetU = pmCommander->GetTarget())
		{
			if (commandVector.size() > 1)
			{
				uint32 spellId = std::stoi(commandVector.at(1));
				targetU->RemoveAurasDueToSpell(spellId);
			}
		}
	}
	else if (commandName == "aura")
	{
		std::ostringstream replyStream;
		if (Unit* targetU = pmCommander->GetTarget())
		{
			if (commandVector.size() > 1)
			{
				uint32 spellId = std::stoi(commandVector.at(1));
				if (targetU->HasAura(spellId))
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
				replyStream << "No id";
			}
		}
		else
		{
			replyStream << "No target";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
	}
	else if (commandName == "tcast")
	{
		std::ostringstream replyStream;
		if (Unit* targetU = pmCommander->GetTarget())
		{
			if (commandVector.size() > 1)
			{
				uint32 spellId = std::stoi(commandVector.at(1));
				SpellCastResult scr = targetU->CastSpell(targetU, spellId, TriggerCastFlags::TRIGGERED_NONE);
				replyStream << "Cast result : " << scr;
			}
			else
			{
				replyStream << "No spell";
			}
		}
		else
		{
			replyStream << "No t";
		}
		sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pmCommander);
	}
	else if (commandName == "delay")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					int delayMS = std::stoi(commandVector.at(1));
					ns->dpsDelay = delayMS;
					replyStream << "DPS delay set to : " << delayMS;
				}
				else
				{
					replyStream << "DPS delay is : " << ns->dpsDelay;
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "cure")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string cureCMD = commandVector.at(1);
					if (cureCMD == "on")
					{
						ns->cure = true;
					}
					else if (cureCMD == "off")
					{
						ns->cure = false;
					}
					else
					{
						replyStream << "Unknown state";
					}
				}
				if (ns->cure)
				{
					replyStream << "auto cure is on";
				}
				else
				{
					replyStream << "auto cure is off";
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "petting")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string switchCMD = commandVector.at(1);
					if (switchCMD == "on")
					{
						ns->petting = true;
					}
					else if (switchCMD == "off")
					{
						ns->petting = false;
					}
					else if (switchCMD == "reset")
					{
						if (pmTargetPlayer->getClass() == Classes::CLASS_HUNTER)
						{
							if (NingerAction_Hunter* naHunter = (NingerAction_Hunter*)pmTargetPlayer->ningerAction)
							{
								if (naHunter->Petting(ns->petting, true))
								{
									replyStream << "Pet abandoned. ";
								}
								else
								{
									replyStream << "reset failed";
								}
							}
						}
					}
					else
					{
						replyStream << "Unknown state";
					}
				}
				if (ns->petting)
				{
					replyStream << "Petting is on";
				}
				else
				{
					replyStream << "Petting is off";
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "aoe")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string roleCMD = commandVector.at(1);
					bool onoff = false;
					if (commandVector.size() > 2)
					{
						std::string switchCMD = commandVector.at(2);
						if (switchCMD == "on")
						{
							onoff = true;
						}
						if (roleCMD == "tank")
						{
							if (pmTargetPlayer->groupRole == GroupRole::GroupRole_Tank)
							{
								ns->aoe = onoff;
							}
						}
						else if (roleCMD == "dps")
						{
							if (pmTargetPlayer->groupRole == GroupRole::GroupRole_DPS)
							{
								ns->aoe = onoff;
							}
						}
					}
				}
				if (ns->aoe)
				{
					replyStream << "AOE is on";
				}
				else
				{
					replyStream << "AOE is off";
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "rti")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				int targetIcon = -1;
				if (commandVector.size() > 1)
				{
					std::string iconIndex = commandVector.at(1);
					targetIcon = atoi(iconIndex.c_str());
				}
				if (targetIcon >= 0 && targetIcon < TARGET_ICON_COUNT)
				{
					ns->rti = targetIcon;
				}
				replyStream << "RTI is " << ns->rti;
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "portal")
	{
		if (pmTargetPlayer)
		{
			if (pmTargetPlayer->getClass() == Classes::CLASS_MAGE)
			{
				if (pmTargetPlayer->GetSession()->isNinger)
				{
					if (commandVector.size() > 1)
					{
						std::string city = commandVector.at(1);
						if (NingerAction_Mage* nam = (NingerAction_Mage*)pmTargetPlayer->ningerAction)
						{
							uint32 portalSpell = 0;
							if (city == "Dalaran")
							{
								portalSpell = nam->spell_Portal_Dalaran;
							}
							else
							{
								uint32 playerRace = pmTargetPlayer->getRace();
								if (playerRace == Races::RACE_DRAENEI || playerRace == Races::RACE_DWARF || playerRace == Races::RACE_GNOME || playerRace == Races::RACE_GOBLIN || playerRace == Races::RACE_HUMAN || playerRace == Races::RACE_NIGHTELF)
								{
									if (city == "exodar")
									{
										portalSpell = nam->spell_Portal_Exodar;
									}
									else if (city == "stormwind")
									{
										portalSpell = nam->spell_Portal_Stormwind;
									}
									else if (city == "darnassus")
									{
										portalSpell = nam->spell_Portal_Darnassus;
									}
									else if (city == "ironforge")
									{
										portalSpell = nam->spell_Portal_Ironforge;
									}
									else if (city == "shattrath")
									{
										portalSpell = nam->spell_Portal_Shattrath_A;
									}
								}
								else
								{
									if (city == "orgrimmar")
									{
										portalSpell = nam->spell_Portal_Orgrimmar;
									}
									else if (city == "thunderbluff")
									{
										portalSpell = nam->spell_Portal_ThunderBluff;
									}
									else if (city == "silvermoon")
									{
										portalSpell = nam->spell_Portal_Silvermoon;
									}
									else if (city == "undercity")
									{
										portalSpell = nam->spell_Portal_Shattrath_H;
									}
								}
							}
							if (portalSpell > 0)
							{
								std::ostringstream replyStream;
								replyStream << "Portal to " << city;
								pmTargetPlayer->Say(replyStream.str(), Language::LANG_UNIVERSAL);
								nam->CastSpell(pmTargetPlayer, portalSpell);
							}
						}
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "home")
	{
		if (pmTargetPlayer)
		{
			if (pmTargetPlayer->GetSession()->isNinger)
			{
				pmTargetPlayer->Whisper("Going home", Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
				pmTargetPlayer->TeleportToHomebind();
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "instant")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				if (commandVector.size() > 1)
				{
					std::string instantType = commandVector.at(1);
					if (instantType == "on")
					{
						ns->instantOnly = true;
					}
					else
					{
						ns->instantOnly = false;
					}
				}
				replyStream << "Instant type is : ";
				if (ns->instantOnly)
				{
					replyStream << "on";
				}
				else
				{
					replyStream << "off";
				}
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "glue")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				std::ostringstream replyStream;
				ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Glue;
				pmTargetPlayer->ningerAction->nm->ResetMovement();
				replyStream << "bst : " << ns->basicStrategyType;
				pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "Innervate")
	{
		if (pmTargetPlayer)
		{
			if (pmTargetPlayer->getClass() == Classes::CLASS_DRUID)
			{
				if (pmTargetPlayer->GetSession()->isNinger)
				{
					if (NingerAction_Druid* nad = (NingerAction_Druid*)pmTargetPlayer->ningerAction)
					{
						if (nad->spell_Innervate > 0)
						{
							if (Group* targetGroup = pmTargetPlayer->GetGroup())
							{
								for (GroupReference* groupRef = targetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
								{
									if (Player* member = groupRef->getSource())
									{
										if (member->groupRole == GroupRole::GroupRole_Healer)
										{
											if (nad->spell_MoonkinForm > 0)
											{
												nad->CancelAura(nad->spell_MoonkinForm);
											}
											if (nad->CastSpell(member, nad->spell_Innervate, true, false, true))
											{
												std::ostringstream replyStream;
												replyStream << "Innervate - " << member->GetName();
												pmTargetPlayer->Yell(replyStream.str(), Language::LANG_UNIVERSAL);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "assist")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (ns->rti >= 0)
				{
					if (Group* targetGroup = pmTargetPlayer->GetGroup())
					{
						ObjectGuid ogRTI = targetGroup->GetGuidByTargetIcon(ns->rti);
						Unit* rtiTarget = nullptr;
						if (!ogRTI.IsEmpty())
						{
							rtiTarget = ObjectAccessor::GetUnit(*pmTargetPlayer, ogRTI);
						}
						else
						{
							if (pmTargetPlayer->IsInCombat())
							{
								bool marked = false;
								std::set<Unit*> checkAttackers = pmTargetPlayer->getAttackers();
								for (std::set<Unit*>::iterator ait = checkAttackers.begin(); ait != checkAttackers.end(); ++ait)
								{
									if (Unit* eachAttacker = *ait)
									{
										if (pmTargetPlayer->CanAttack(eachAttacker))
										{
											if (targetGroup->GetTargetIconByGuid(eachAttacker->GetObjectGuid()) < 0)
											{
												if (pmTargetPlayer->ningerAction->Mark(eachAttacker, ns->rti))
												{
													marked = true;
													break;
												}
											}
										}
									}
								}
								if (!marked)
								{
									std::list<Creature*> creatureList;
									MaNGOS::AnyUnitInObjectRangeCheck u_check(pmTargetPlayer, VISIBILITY_DISTANCE_NORMAL);
									MaNGOS::CreatureListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(creatureList, u_check);
									Cell::VisitAllObjects(pmTargetPlayer, searcher, RANGE_MAX_DISTANCE);
									if (!creatureList.empty())
									{
										for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
										{
											if (Creature* hostileCreature = *itr)
											{
												if (pmTargetPlayer->CanAttack(hostileCreature))
												{
													if (targetGroup->GetTargetIconByGuid(hostileCreature->GetObjectGuid()) < 0)
													{
														if (hostileCreature->IsInCombat())
														{
															if (pmTargetPlayer->ningerAction->Mark(hostileCreature, ns->rti))
															{
																marked = true;
																break;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
						if (rtiTarget)
						{
							pmTargetPlayer->ningerAction->Assist(ns->rti);
						}
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "equip")
	{
		if (pmTargetPlayer)
		{
			std::ostringstream replyStream;
			if (commandVector.size() > 1)
			{
				uint32 entry = 0;
				std::string equipTypeName = commandVector.at(1);
				if (equipTypeName == "name")
				{
					if (commandVector.size() > 2)
					{
						std::string itemName = commandVector.at(2);
						std::ostringstream queryStream;
						queryStream << "SELECT entry FROM item_template where name = '" << itemName << "'";
						QueryResult* itemQR = WorldDatabase.Query(queryStream.str().c_str());
						if (itemQR)
						{
							do
							{
								Field* fields = itemQR->Fetch();
								entry = fields[0].GetUInt32();
								break;
							} while (itemQR->NextRow());
						}
						delete itemQR;
					}
					else
					{
						replyStream << "Missing name";
					}
				}
				else if (equipTypeName == "entry")
				{
					if (commandVector.size() > 2)
					{
						std::string entryStr = commandVector.at(2);
						entry = std::atoi(entryStr.c_str());
					}
					else
					{
						replyStream << "Missing entry";
					}
				}
				if (entry > 0)
				{
					std::ostringstream queryStream;
					queryStream << "SELECT entry FROM item_template where entry = " << entry << "";
					QueryResult* itemQR = WorldDatabase.Query(queryStream.str().c_str());
					if (itemQR)
					{
						do
						{
							Field* fields = itemQR->Fetch();
							uint32 entry = fields[0].GetUInt32();
							if (const ItemPrototype* proto = sObjectMgr.GetItemPrototype(entry))
							{
								if (Item* pItem = Item::CreateItem(entry, 1))
								{
									uint32 equipSlot = 0;
									switch (proto->InventoryType)
									{
									case INVTYPE_HEAD:
									{
										equipSlot = EQUIPMENT_SLOT_HEAD;
										break;
									}
									case INVTYPE_NECK:
									{
										equipSlot = EQUIPMENT_SLOT_NECK;
										break;
									}
									case INVTYPE_SHOULDERS:
									{
										equipSlot = EQUIPMENT_SLOT_SHOULDERS;
										break;
									}
									case INVTYPE_BODY:
									{
										equipSlot = EQUIPMENT_SLOT_BODY;
										break;
									}
									case INVTYPE_CHEST:
									{
										equipSlot = EQUIPMENT_SLOT_CHEST;
										break;
									}
									case INVTYPE_ROBE:
									{
										equipSlot = EQUIPMENT_SLOT_CHEST;
										break;
									}
									case INVTYPE_WAIST:
									{
										equipSlot = EQUIPMENT_SLOT_WAIST;
										break;
									}
									case INVTYPE_LEGS:
									{
										equipSlot = EQUIPMENT_SLOT_LEGS;
										break;
									}
									case INVTYPE_FEET:
									{
										equipSlot = EQUIPMENT_SLOT_FEET;
										break;
									}
									case INVTYPE_WRISTS:
									{
										equipSlot = EQUIPMENT_SLOT_WRISTS;
										break;
									}
									case INVTYPE_HANDS:
									{
										equipSlot = EQUIPMENT_SLOT_HANDS;
										break;
									}
									case INVTYPE_FINGER:
									{
										equipSlot = EQUIPMENT_SLOT_FINGER1;
										//viable_slots[1] = EQUIPMENT_SLOT_FINGER2;
										break;
									}
									case INVTYPE_TRINKET:
									{
										equipSlot = EQUIPMENT_SLOT_TRINKET1;
										//viable_slots[1] = EQUIPMENT_SLOT_TRINKET2;
										break;
									}
									case INVTYPE_CLOAK:
									{
										equipSlot = EQUIPMENT_SLOT_BACK;
										break;
									}
									case INVTYPE_WEAPON:
									{
										equipSlot = EQUIPMENT_SLOT_MAINHAND;
										//if (CanDualWield())
										//{										
										//	viable_slots[1] = EQUIPMENT_SLOT_OFFHAND;
										//}
										break;
									}
									case INVTYPE_SHIELD:
									{
										equipSlot = EQUIPMENT_SLOT_OFFHAND;
										break;
									}
									case INVTYPE_RANGED:
									{
										equipSlot = EQUIPMENT_SLOT_RANGED;
										break;
									}
									case INVTYPE_2HWEAPON:
									{
										equipSlot = EQUIPMENT_SLOT_MAINHAND;
										//if (CanDualWield() && CanTitanGrip())
										//{										
										//	viable_slots[1] = EQUIPMENT_SLOT_OFFHAND;
										//}
										break;
									}
									case INVTYPE_TABARD:
									{
										equipSlot = EQUIPMENT_SLOT_TABARD;
										break;
									}
									case INVTYPE_WEAPONMAINHAND:
									{
										equipSlot = EQUIPMENT_SLOT_MAINHAND;
										break;
									}
									case INVTYPE_WEAPONOFFHAND:
									{
										equipSlot = EQUIPMENT_SLOT_OFFHAND;
										break;
									}
									case INVTYPE_HOLDABLE:
									{
										equipSlot = EQUIPMENT_SLOT_OFFHAND;
										break;
									}
									case INVTYPE_THROWN:
									{
										equipSlot = EQUIPMENT_SLOT_RANGED;
										break;
									}
									case INVTYPE_RANGEDRIGHT:
									{
										equipSlot = EQUIPMENT_SLOT_RANGED;
										break;
									}
									case INVTYPE_BAG:
									{
										equipSlot = INVENTORY_SLOT_BAG_START + 0;
										//viable_slots[1] = INVENTORY_SLOT_BAG_START + 1;
										//viable_slots[2] = INVENTORY_SLOT_BAG_START + 2;
										//viable_slots[3] = INVENTORY_SLOT_BAG_START + 3;
										break;
									}
									case INVTYPE_RELIC:
									{
										//pClass = getClass();
										//if (pClass)
										//{
										//	switch (proto->SubClass)
										//	{
										//	case ITEM_SUBCLASS_ARMOR_LIBRAM:											
										//		if (pClass == CLASS_PALADIN)
										//		{
										//			equipSlot = EQUIPMENT_SLOT_RANGED;
										//		}
										//		break;
										//	case ITEM_SUBCLASS_ARMOR_IDOL:											
										//		if (pClass == CLASS_DRUID)
										//		{
										//			equipSlot = EQUIPMENT_SLOT_RANGED;
										//		}
										//		break;
										//	case ITEM_SUBCLASS_ARMOR_TOTEM:											
										//		if (pClass == CLASS_SHAMAN)
										//		{
										//			equipSlot = EQUIPMENT_SLOT_RANGED;
										//		}
										//		break;
										//	case ITEM_SUBCLASS_ARMOR_MISC:											
										//		if (pClass == CLASS_WARLOCK)
										//		{
										//			equipSlot = EQUIPMENT_SLOT_RANGED;
										//		}
										//		break;
										//	case ITEM_SUBCLASS_ARMOR_SIGIL:											
										//		if (pClass == CLASS_DEATH_KNIGHT)
										//		{
										//			equipSlot = EQUIPMENT_SLOT_RANGED;
										//		}
										//		break;
										//	default:											
										//		break;
										//	}
										//}
										break;
									}
									default:
									{
										break;
									}
									}
									if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, equipSlot))
									{
										if (const ItemPrototype* checkIT = currentEquip->GetProto())
										{
											pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, equipSlot, true);
										}
									}
									uint16 dest = 0;
									if (pmTargetPlayer->CanEquipItem(equipSlot, dest, pItem, false) == InventoryResult::EQUIP_ERR_OK)
									{
										pmTargetPlayer->EquipItem(dest, pItem, true);
										replyStream << "Equiped " << pItem->GetProto()->Name1;
									}
									else
									{
										replyStream << "Can not equip " << pItem->GetProto()->Name1;
									}
								}
							}
							break;
						} while (itemQR->NextRow());
					}
					else
					{
						replyStream << "Item not found : " << entry;
					}
					delete itemQR;
				}
				else
				{
					replyStream << "Item entry not found : " << entry;
				}
			}
			else
			{
				for (std::unordered_set<NingerEntity*>::iterator reIT = ningerEntitySet.begin(); reIT != ningerEntitySet.end(); reIT++)
				{
					if (NingerEntity* eachNinger = *reIT)
					{
						if (eachNinger->character_id == pmTargetPlayer->GetObjectGuid().GetCounter())
						{
							pmTargetPlayer->ningerAction->InitializeEquipments(true);
							eachNinger->entityState = NingerEntityState::NingerEntityState_Equip;
							eachNinger->checkDelay = 100;
							replyStream << "Equipments reset";
							break;
						}
					}
				}
			}
			pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
		else
		{
			//HandleChatCommand(pmCommander, pmContent, pmCommander);
		}
	}
	else if (commandName == "rush")
	{
		if (pmTargetPlayer)
		{
			if (NingerStrategy_Base* ns = pmTargetPlayer->strategyMap[pmTargetPlayer->activeStrategyIndex])
			{
				if (ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze || ns->basicStrategyType == BasicStrategyType::BasicStrategyType_Glue)
				{
					ns->basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
				}
				if (Unit* target = pmCommander->GetTarget())
				{
					if (ns->Engage(target))
					{
						int engageLimit = DEFAULT_ACTION_LIMIT_DELAY;
						if (commandVector.size() > 1)
						{
							std::string checkStr = commandVector.at(1);
							engageLimit = atoi(checkStr.c_str());
						}
						ns->rushing = true;
						ns->actionLimit = engageLimit;
						ns->ogActionTarget = target->GetObjectGuid();
						ns->actionType = ActionType::ActionType_Engage;
						std::ostringstream replyStream;
						replyStream << "Rush " << target->GetName();
						pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
					}
				}
			}
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
	else if (commandName == "talent")
	{
		if (pmTargetPlayer)
		{
			std::ostringstream replyStream;
			pmTargetPlayer->ningerAction->ResetTalent();
			replyStream << "Talents reset";
			pmTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pmCommander->GetObjectGuid());
		}
		else if (pmTargetGroup)
		{
			for (GroupReference* groupRef = pmTargetGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (member)
				{
					if (member->GetObjectGuid() != pmCommander->GetObjectGuid())
					{
						HandleChatCommand(pmCommander, pmContent, member);
					}
				}
			}
		}
	}
}

void NingerManager::HandleNingerPacket(const WorldSession* pmSession, WorldPacket pmPacket)
{
	switch (pmPacket.GetOpcode())
	{
	case SMSG_CHAR_ENUM:
	{
		std::unordered_set<uint32> myCharacterIdSet;
		QueryResult* characterQR = CharacterDatabase.PQuery("SELECT guid FROM characters where account = %d", pmSession->GetAccountId());
		if (characterQR)
		{
			Field* characterFields = characterQR->Fetch();
			uint32 character_id = characterFields[0].GetUInt32();
			if (character_id > 0)
			{
				myCharacterIdSet.insert(character_id);
			}
		}
		for (std::unordered_set<NingerEntity*>::iterator reIT = sNingerManager->ningerEntitySet.begin(); reIT != sNingerManager->ningerEntitySet.end(); reIT++)
		{
			if (NingerEntity* re = *reIT)
			{
				if (myCharacterIdSet.find(re->character_id) != myCharacterIdSet.end())
				{
					re->entityState = NingerEntityState::NingerEntityState_DoLogin;
				}
			}
		}
		break;
	}
	case SMSG_SPELL_FAILURE:
	{
		break;
	}
	case SMSG_SPELL_DELAYED:
	{
		break;
	}
	case SMSG_GROUP_INVITE:
	{
		if (Player* receiver = pmSession->GetPlayer())
		{
			if (!pmSession->GetPlayer())
			{
				break;
			}
			else if (!receiver->IsInWorld())
			{
				break;
			}
			else if (!receiver->ningerAction)
			{
				break;
			}
			if (Group* myGroup = receiver->GetGroup())
			{
				receiver->RemoveFromGroup();
				receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
				receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, true);
			}
			if (Group* grp = receiver->GetGroupInvite())
			{
				receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
				receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, true);
				WorldPacket wpAccept(CMSG_GROUP_ACCEPT, 4);
				wpAccept << uint32(0);
				receiver->GetSession()->HandleGroupAcceptOpcode(wpAccept);
				std::ostringstream replyStream_Talent;
				receiver->ningerAction->Reset();
				replyStream_Talent << sNingerManager->characterTalentTabNameMap[receiver->getClass()][receiver->ningerAction->specialty];
				if (Player* inviter = ObjectAccessor::FindPlayer(grp->GetLeaderGuid()))
				{
					receiver->Whisper(replyStream_Talent.str(), Language::LANG_UNIVERSAL, inviter->GetObjectGuid());
				}
				if (receiver->getClass() == Classes::CLASS_ROGUE)
				{
					if (NingerAction_Rogue* nar = (NingerAction_Rogue*)receiver->ningerAction)
					{
						nar->CancelAura(nar->spell_Stealth);
					}
				}
			}
		}
		break;
	}
	case BUY_ERR_NOT_ENOUGHT_MONEY:
	{
		break;
	}
	case BUY_ERR_REPUTATION_REQUIRE:
	{
		break;
	}
	case MSG_RAID_READY_CHECK:
	{
		break;
	}
	case SMSG_GROUP_SET_LEADER:
	{
		//std::string leaderName = "";
		//pmPacket >> leaderName;
		//Player* newLeader = ObjectAccessor::FindPlayerByName(leaderName);
		//if (newLeader)
		//{
		//    if (newLeader->GetGUID() == me->GetGUID())
		//    {
		//        WorldPacket data(CMSG_GROUP_SET_LEADER, 8);
		//        data << master->GetGUID().WriteAsPacked();
		//        me->GetSession()->HandleGroupSetLeaderOpcode(data);
		//    }
		//    else
		//    {
		//        if (!newLeader->isninger)
		//        {
		//            master = newLeader;
		//        }
		//    }
		//}
		break;
	}
	case SMSG_RESURRECT_REQUEST:
	{
		if (Player* receiver = pmSession->GetPlayer())
		{
			if (!receiver)
			{
				break;
			}
			else if (!receiver->IsInWorld())
			{
				break;
			}
			else if (!receiver->ningerAction)
			{
				break;
			}
			if (receiver->isRessurectRequested())
			{
				receiver->ResurrectUsingRequestDataInit();
				receiver->ClearInCombat();
				receiver->ningerAction->ClearTarget();
				receiver->ningerAction->nm->ResetMovement();
			}
		}
		break;
	}
	case SMSG_INVENTORY_CHANGE_FAILURE:
	{
		break;
	}
	case SMSG_TRADE_STATUS:
	{
		break;
	}
	case SMSG_LOOT_RESPONSE:
	{
		break;
	}
	case SMSG_ITEM_PUSH_RESULT:
	{
		break;
	}
	case SMSG_PARTY_COMMAND_RESULT:
	{
		break;
	}
	case SMSG_DUEL_REQUESTED:
	{
		if (Player* receiver = pmSession->GetPlayer())
		{
			if (!receiver)
			{
				break;
			}
			if (!receiver->duel)
			{
				break;
			}
			receiver->DuelComplete(DuelCompleteType::DUEL_INTERRUPTED);
			receiver->Whisper("Not interested", Language::LANG_UNIVERSAL, receiver->duel->opponent->GetObjectGuid());
		}
		break;
	}
	default:
	{
		break;
	}
	}
}
