#include "NierManager.h"
#include "NierStrategies/NierStrategy_Base.h"
#include "NierConfig.h"
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

#include "World/World.h"
#include "Accounts/AccountMgr.h"
#include "Grids/GridNotifiers.h"

// lfm nier 
#include <boost/chrono/duration.hpp>

NierManager::NierManager()
{
	allianceRaces.clear();
	hordeRaces.clear();
	nierNameMap.clear();
	characterTalentTabNameMap.clear();
}

void NierManager::InitializeManager()
{
	if (sNierConfig.Enable == 0)
	{
		return;
	}

	sLog.outBasic("Initialize nier");


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

	if (sNierConfig.Reset)
	{
		DeleteNiers();
	}

	auto nierNameQR = WorldDatabase.Query("SELECT name FROM nier_names order by rand()");
	if (nierNameQR)
	{
		do
		{
			Field* fields = nierNameQR->Fetch();
			std::string eachName = fields[0].GetString();
			nierNameMap[nierNameMap.size()] = eachName;
		} while (nierNameQR->NextRow());
	}

	sLog.outBasic("nier initialized");
}

NierManager* NierManager::instance()
{
	static NierManager instance;
	return &instance;
}

void NierManager::DeleteNiers()
{
	auto nierQR = CharacterDatabase.Query("SELECT account_id FROM nier");
	if (nierQR)
	{
		do
		{
			Field* fields = nierQR->Fetch();
			uint32 eachAccountId = fields[0].GetUInt32();
			if (eachAccountId > 0)
			{				
				sAccountMgr.DeleteAccount(eachAccountId);
			}
		} while (nierQR->NextRow());
	}
	CharacterDatabase.DirectExecute("delete from nier");	

	std::ostringstream accountQueryStream;
	accountQueryStream << "SELECT id FROM account where username like '" << NIER_MARK << "%'";
	std::string accountQuerySQL = accountQueryStream.str();
	auto nierAccountQR = LoginDatabase.Query(accountQuerySQL.c_str());
	if (nierAccountQR)
	{
		do
		{
			Field* fields = nierAccountQR->Fetch();
			uint32 eachAccountId = fields[0].GetUInt32();
			if (eachAccountId > 0)
			{
				sAccountMgr.DeleteAccount(eachAccountId);
			}
		} while (nierAccountQR->NextRow());
	}
}

void NierManager::HandleChatCommand(Player* pmCommander, std::string pmContent, Player* pmTargetPlayer, Group* pmTargetGroup)
{
	if (!pmCommander)
	{
		return;
	}
	std::vector<std::string> commandVector = SplitString(pmContent, " ", true);
	std::string commandName = commandVector.at(0);
	if (commandName == "tank")
	{
		if (pmTargetGroup)
		{

		}
		else
		{

		}
	}
	else if (commandName == "debug")
	{

	}
}

void NierManager::HandleNierPacket(const WorldSession* pmSession, WorldPacket pmPacket)
{
	switch (pmPacket.GetOpcode())
	{
	case SMSG_CHAR_ENUM:
	{
		//std::unordered_set<uint32> myCharacterIdSet;
		//QueryResult* characterQR = CharacterDatabase.PQuery("SELECT guid FROM characters where account = %d", pmSession->GetAccountId());
		//if (characterQR)
		//{
		//	Field* characterFields = characterQR->Fetch();
		//	uint32 character_id = characterFields[0].GetUInt32();
		//	if (character_id > 0)
		//	{
		//		myCharacterIdSet.insert(character_id);
		//	}
		//}
		//for (std::unordered_set<NierEntity*>::iterator reIT = sNierManager->nierEntitySet.begin(); reIT != sNierManager->nierEntitySet.end(); reIT++)
		//{
		//	if (NierEntity* re = *reIT)
		//	{
		//		if (myCharacterIdSet.find(re->character_id) != myCharacterIdSet.end())
		//		{
		//			re->entityState = NierEntityState::NierEntityState_DoLogin;
		//		}
		//	}
		//}
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
		//if (Player* receiver = pmSession->GetPlayer())
		//{
		//	if (!pmSession->GetPlayer())
		//	{
		//		break;
		//	}
		//	else if (!receiver->IsInWorld())
		//	{
		//		break;
		//	}
		//	else if (!receiver->nierAction)
		//	{
		//		break;
		//	}
		//	if (Group* myGroup = receiver->GetGroup())
		//	{
		//		receiver->RemoveFromGroup();
		//		receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
		//		receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, true);
		//	}
		//	if (Group* grp = receiver->GetGroupInvite())
		//	{
		//		receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
		//		receiver->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, true);
		//		WorldPacket wpAccept(CMSG_GROUP_ACCEPT, 4);
		//		wpAccept << uint32(0);
		//		receiver->GetSession()->HandleGroupAcceptOpcode(wpAccept);
		//		std::ostringstream replyStream_Talent;
		//		receiver->nierAction->Reset();
		//		replyStream_Talent << sNierManager->characterTalentTabNameMap[receiver->getClass()][receiver->nierAction->specialty];
		//		if (Player* inviter = ObjectAccessor::FindPlayer(grp->GetLeaderGuid()))
		//		{
		//			receiver->Whisper(replyStream_Talent.str(), Language::LANG_UNIVERSAL, inviter->GetObjectGuid());
		//		}
		//		if (receiver->getClass() == Classes::CLASS_ROGUE)
		//		{
		//			if (NierAction_Rogue* nar = (NierAction_Rogue*)receiver->nierAction)
		//			{
		//				nar->CancelAura(nar->spell_Stealth);
		//			}
		//		}
		//	}
		//}
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
		//        if (!newLeader->isnier)
		//        {
		//            master = newLeader;
		//        }
		//    }
		//}
		break;
	}
	case SMSG_RESURRECT_REQUEST:
	{
		//if (Player* receiver = pmSession->GetPlayer())
		//{
		//	if (!receiver)
		//	{
		//		break;
		//	}
		//	else if (!receiver->IsInWorld())
		//	{
		//		break;
		//	}
		//	else if (!receiver->nierAction)
		//	{
		//		break;
		//	}
		//	if (receiver->isRessurectRequested())
		//	{
		//		receiver->ResurrectUsingRequestDataInit();
		//		receiver->ClearInCombat();
		//		receiver->nierAction->ClearTarget();
		//		receiver->nierAction->nm->ResetMovement();
		//	}
		//}
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

std::vector<std::string> NierManager::SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored)
{
	std::vector<std::string> resultStringVector;
	std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}, delimStr.at(0));
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

std::string NierManager::TrimString(std::string srcStr)
{
	std::string result = srcStr;
	if (!result.empty())
	{
		result.erase(0, result.find_first_not_of(" "));
		result.erase(result.find_last_not_of(" ") + 1);
	}

	return result;
}