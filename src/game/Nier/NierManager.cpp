#include "NierManager.h"

#include "NierConfig.h"
#include "NierScripts/NierScript_Base.h"

#include "Ming/MingManager.h"
#include "World/World.h"
#include "Accounts/AccountMgr.h"

NierManager::NierManager()
{
	allianceRaces.clear();
	hordeRaces.clear();
	nierNameMap.clear();
	characterTalentTabNameMap.clear();
	trainerMap.clear();

	nierMap.clear();
	updateIndex = 0;	
}

void NierManager::InitializeManager()
{
	sLog.outBasic("Initialize nier");

	if (!sNierConfig.Initialize())
	{
		return;
	}

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

	trainerMap.clear();
	trainerMap[Classes::CLASS_WARRIOR] = 914;
	trainerMap[Classes::CLASS_PALADIN] = 928;
	trainerMap[Classes::CLASS_HUNTER] = 5115;
	trainerMap[Classes::CLASS_ROGUE] = 4214;
	trainerMap[Classes::CLASS_PRIEST] = 3046;
	trainerMap[Classes::CLASS_SHAMAN] = 3032;
	trainerMap[Classes::CLASS_MAGE] = 3047;
	trainerMap[Classes::CLASS_WARLOCK] = 5173;
	trainerMap[Classes::CLASS_DRUID] = 4217;

	groupRoleNameMap.clear();
	groupRoleNameMap[GroupRole::GroupRole_Tank] = "tank";
	groupRoleNameMap[GroupRole::GroupRole_Healer] = "healer";
	groupRoleNameMap[GroupRole::GroupRole_DPS] = "dps";

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

	if (sNierConfig.Reset == 1)
	{
		DeleteNiers();
	}

	sLog.outBasic("nier system initialized");
}

NierManager* NierManager::instance()
{
	static NierManager instance;
	return &instance;
}

void NierManager::LoginNiers(uint32 pMasterId)
{
	if (pMasterId > 0)
	{
		std::ostringstream nierCountQueryStream;
		nierCountQueryStream << "SELECT count(*) FROM nier where master_id = " << pMasterId << " and nier_type = 0";
		auto nierQR = CharacterDatabase.Query(nierCountQueryStream.str().c_str());
		if (nierQR)
		{
			int createCount = sNierConfig.NierCount;
			do
			{
				Field* fields = nierQR->Fetch();
				uint32 nierCount = fields[0].GetUInt32();
				std::ostringstream replyStream;
				replyStream << "Nier count loaded : ";
				replyStream << nierCount;
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str());
				createCount = createCount - nierCount;
				break;
			} while (nierQR->NextRow());
			if (createCount > 0)
			{
				int tankCountA = 1;
				int tankCountH = 1;
				int healerCountA = 1;
				int healerCountH = 1;
				// lfm debug 
				tankCountA = 0;
				tankCountH = 0;
				healerCountA = 1;
				healerCountH = 0;
				uint32 career = Classes::CLASS_ROGUE;
				bool alliance = true;
				while (createCount > 0)
				{
					career = Classes::CLASS_ROGUE;
					if (alliance)
					{
						if (tankCountA > 0)
						{
							career = Classes::CLASS_PALADIN;
							tankCountA--;
						}
						else if (healerCountA > 0)
						{
							career = Classes::CLASS_PRIEST;
							healerCountA--;
						}
					}
					else
					{
						if (tankCountH > 0)
						{
							career = Classes::CLASS_PALADIN;
							tankCountH--;
						}
						else if (healerCountH > 0)
						{
							career = Classes::CLASS_PRIEST;
							healerCountH--;
						}
					}
					AddNier(pMasterId, career, alliance);
					std::ostringstream replyStream;
					replyStream << "Nier created : " << pMasterId << "-" << career << "-" << alliance;
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str());
					alliance = !alliance;
					createCount--;
				}
			}
		}
	}

	std::ostringstream nierQueryStream;
	if (pMasterId > 0)
	{
		nierQueryStream << "SELECT entry, master_id, account_id, character_id, account_name, race, career, specialty, role, nier_type FROM nier where master_id = " << pMasterId << " and nier_type = 0";
	}
	else
	{
		nierQueryStream << "SELECT entry, master_id, account_id, character_id, account_name, race, career, specialty, role, nier_type FROM nier where nier_type = 0";
	}
	auto nierQR = CharacterDatabase.Query(nierQueryStream.str().c_str());
	if (nierQR)
	{
		do
		{
			NierEntity* nier = nullptr;
			Field* fields = nierQR->Fetch();
			uint32 entry = fields[0].GetUInt32();
			for (std::unordered_map<uint32, NierEntity*>::iterator nierIT = nierMap.begin(); nierIT != nierMap.end(); nierIT++)
			{
				if (nierIT->second->entry == entry) {
					std::ostringstream replyStream;
					replyStream << nierIT->second->entry << " exists.";
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str());
					nier = nierIT->second;
					break;
				}
			}
			if (nier == nullptr)
			{
				nier = new NierEntity();
				nier->entry = entry;
				nier->master_id = fields[1].GetUInt32();
				nier->account_id = fields[2].GetUInt32();
				nier->character_id = fields[3].GetUInt32();
				nier->target_race = fields[5].GetUInt32();
				nier->target_class = fields[6].GetUInt32();
				nier->target_specialty = fields[7].GetUInt32();
				nierMap[nierMap.size()] = nier;
				std::ostringstream replyStream;
				replyStream << nier->entry << " added.";
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str());
			}
			if (nier)
			{
				if (nier->entityState == NierState::NierState_OffLine)
				{
					nier->updateDelay = urand(5000, 60000);
					nier->entityState = NierState::NierState_Enter;
				}
			}
		} while (nierQR->NextRow());
	}
}

void NierManager::LogoutNiers(bool pmInstant)
{
	sWorld.KickAll(true);
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

bool NierManager::IsPolymorphed(Unit* pmTarget)
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

bool NierManager::SpellCasting(Player* me)
{
	//if (Spell const* cs = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
	//{
	//	if (Unit* target = cs->m_targets.getUnitTarget())
	//	{
	//		if (!target->IsAlive())
	//		{
	//			me->FinishSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL);
	//		}
	//	}
	//	return true;
	//}
	if (Spell const* cs = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
	{
		if (Unit* target = cs->m_targets.getUnitTarget())
		{
			if (!target->IsAlive())
			{
				me->FinishSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
			}
		}
		return true;
	}
	if (Spell const* cs = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL))
	{
		if (Unit* target = cs->m_targets.getUnitTarget())
		{
			if (!target->IsAlive())
			{
				me->FinishSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
			}
		}
		return true;
	}

	return false;
}

float NierManager::GetExactDistance(Position p1, Position p2)
{
	float result = 10000.0f;

	result = p1.GetDistance(p2);
	result = sqrt(result);

	return result;
}

Position NierManager::PredictPosition(Unit* target)
{
	Position pos = target->GetPosition();

	float speed = target->GetSpeed(UnitMoveType::MOVE_RUN);
	float orientation = target->GetOrientation();

	if (target->m_movementInfo.HasMovementFlag(MovementFlags::MOVEFLAG_FORWARD))
	{
		pos.x += cos(orientation) * speed;
		pos.y += std::sin(orientation) * speed;
	}
	else if (target->m_movementInfo.HasMovementFlag(MovementFlags::MOVEFLAG_BACKWARD))
	{
		pos.x -= cos(orientation) * speed;
		pos.y -= std::sin(orientation) * speed;
	}

	if (target->m_movementInfo.HasMovementFlag(MovementFlags::MOVEFLAG_STRAFE_LEFT))
	{
		pos.x += cos(orientation + M_PI / 2.f) * speed;
		pos.y += std::sin(orientation + M_PI / 2.f) * speed;
	}
	else if (target->m_movementInfo.HasMovementFlag(MovementFlags::MOVEFLAG_STRAFE_RIGHT))
	{
		pos.x += cos(orientation - M_PI / 2.f) * speed;
		pos.y += std::sin(orientation - M_PI / 2.f) * speed;
	}

	return pos;
}

void NierManager::HandleChatCommand(Player* pCommander, std::string pContent, Player* pTargetPlayer, Group* pTargetGroup)
{
	if (!pCommander)
	{
		return;
	}
	if (pTargetGroup)
	{
		for (const auto& citr : pTargetGroup->GetMemberSlots())
		{
			if (Player* member = sObjectMgr.GetPlayer(citr.guid))
			{
				HandleChatCommand(pCommander, pContent, member);
			}
		}
	}
	std::vector<std::string> commandVector = sMingManager->SplitString(pContent, " ", true);
	std::string commandName = commandVector.at(0);

	if (commandName == "nier")
	{
		if (commandVector.size() > 1)
		{
			std::string nierAction = commandVector.at(1);
			if (nierAction == "login")
			{
				uint32 rawLevel = pCommander->GetLevel();
				if (rawLevel < 10)
				{
					sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, "Level lower than 10", pCommander);
					return;
				}
				LoginNiers(pCommander->GetObjectGuid().GetCounter());
			}
		}
	}
	else if (commandName == "role")
	{
		if (pTargetPlayer)
		{
			if (commandVector.size() > 1)
			{
				std::string role = commandVector.at(1);
				if (role == "tank")
				{
					pTargetPlayer->groupRole = GroupRole::GroupRole_Tank;
				}
				else if (role == "healer")
				{
					pTargetPlayer->groupRole = GroupRole::GroupRole_Healer;
				}
				else if (role == "dps")
				{
					pTargetPlayer->groupRole = GroupRole::GroupRole_DPS;
				}
			}
			std::ostringstream replyStream;
			replyStream << "Role is : " << groupRoleNameMap[pTargetPlayer->groupRole];
			if (pTargetPlayer->GetObjectGuid() == pCommander->GetObjectGuid())
			{
				sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, replyStream.str().c_str(), pCommander);
			}
			else
			{
				pTargetPlayer->Whisper(replyStream.str().c_str(), Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
			}
		}
		else
		{
			HandleChatCommand(pCommander, pContent, pCommander);
		}
	}
	else if (commandName == "equip")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (!pTargetPlayer->IsInCombat())
					{
						if (NierScript_Base* nb = pTargetPlayer->_nierScript)
						{
							nb->InitializeEquipments(true);
							pTargetPlayer->Whisper("Equiped", Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
							return;
						}
					}
				}
				pTargetPlayer->Whisper("Can not equiped", Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
			}
		}
	}
	else if (commandName == "assemble")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						nb->assembleDelay = 60000;
						if (!pTargetPlayer->IsAlive())
						{
							if (!pTargetPlayer->GetCorpse())
							{
								pTargetPlayer->SetBotDeathTimer();
								pTargetPlayer->BuildPlayerRepop();
								WorldLocation loc;
								Corpse* corpse = pTargetPlayer->GetCorpse();
								corpse->GetPosition(loc);
								pTargetPlayer->TeleportTo(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z, 0.0f);
							}
						}
						pTargetPlayer->Whisper("Assemble in 60 seconds", Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
					}
				}
			}
		}
	}
	else if (commandName == "join")
	{
		if (Player* joinTargetPlayer = ObjectAccessor::FindPlayer(pCommander->GetSelectionGuid()))
		{
			pCommander->ClearInCombat();
			pCommander->TeleportTo(joinTargetPlayer->GetMapId(), joinTargetPlayer->GetPositionX(), joinTargetPlayer->GetPositionY(), joinTargetPlayer->GetPositionZ(), joinTargetPlayer->GetOrientation());
		}
	}
	else if (commandName == "freeze")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						nb->freezing = true;
						nb->ClearTarget();
						if (Pet* myPet = pTargetPlayer->GetPet())
						{
							myPet->AttackStop(true, true, true, true);
							if (CharmInfo* pci = myPet->GetCharmInfo())
							{
								pci->SetCommandState(COMMAND_FOLLOW);
							}
						}
						pTargetPlayer->StopMoving();
						pTargetPlayer->GetMotionMaster()->Clear(true);
						pTargetPlayer->Whisper("Freezed", Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
					}
				}
			}
		}
	}
	else if (commandName == "follow")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						float followDistance = 0.0f;
						if (commandVector.size() > 1)
						{
							followDistance = atof(commandVector.at(1).c_str());
						}
						std::ostringstream replyStream;
						replyStream << "Following";
						if (followDistance > CONTACT_DISTANCE)
						{
							nb->followDistance = followDistance;
							replyStream << " : " << nb->followDistance;
						}
						nb->freezing = false;
						pTargetPlayer->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
					}
				}
			}
		}
	}
	else if (commandName == "formation")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						if (commandVector.size() > 1)
						{
							std::string formation = commandVector.at(1);
							if (formation == "point")
							{
								pTargetPlayer->GetMotionMaster()->Clear();
								nb->destination = pCommander->GetPosition();
								nb->orderDelay = 2000;
								pTargetPlayer->GetMotionMaster()->MovePoint(0, nb->destination.x, nb->destination.y, nb->destination.z, ForcedMovement::FORCED_MOVEMENT_RUN, true);
							}
						}
					}
				}
			}
		}
	}
	else if (commandName == "prepare")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						nb->Prepare();
						pTargetPlayer->Whisper("Prepared", Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
					}
				}
			}
		}
	}
	else if (commandName == "cast")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						if (commandVector.size() > 1)
						{
							uint32 spellId = atoi(commandVector.at(1).c_str());
							Unit* castTarget = pCommander->GetTarget();
							if (!castTarget)
							{
								castTarget = pTargetPlayer;
							}
							if (nb->CastSpell(castTarget, spellId))
							{
								nb->orderDelay = 5000;
							}
						}
					}
				}
			}
		}
	}
	else if (commandName == "finish")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						pTargetPlayer->FinishSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL);
						pTargetPlayer->FinishSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
						pTargetPlayer->FinishSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
					}
				}
			}
		}
	}
	else if (commandName == "interrupt")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						nb->InterruptSpells();
					}
				}
			}
		}
	}
	else if (commandName == "rest")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						nb->Rest(true);
					}
				}
			}
		}
	}
	else if (commandName == "emote")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						int emote = 0;
						if (commandVector.size() > 1)
						{
							emote = atoi(commandVector.at(1).c_str());
						}
						pTargetPlayer->HandleEmote(emote);
					}
				}
			}
		}
	}
	else if (commandName == "stand")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (NierScript_Base* nb = pTargetPlayer->_nierScript)
					{
						int stand = 0;
						if (commandVector.size() > 1)
						{
							stand = atoi(commandVector.at(1).c_str());
						}
						pTargetPlayer->SetStandState(stand, true);
					}
				}
			}
		}
	}
	else if (commandName == "tank")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					if (pTargetPlayer->groupRole == GroupRole::GroupRole_Tank)
					{
						if (NierScript_Base* nb = pTargetPlayer->_nierScript)
						{
							if (Unit* target = pCommander->GetTarget())
							{
								if (nb->Tank(target))
								{
									nb->orderDelay = 5000;
									nb->orderType = OrderType::OrderType_Tank;
									if (Group* tankGroup = pTargetPlayer->GetGroup())
									{
										tankGroup->SetTargetIcon(7, pTargetPlayer->GetObjectGuid(), target->GetObjectGuid());
									}
									//eachPlayer->Say("tanking", Language::LANG_UNIVERSAL);
								}
							}
						}
					}
				}
			}
		}
	}
	else if (commandName == "unbind")
	{
		if (pTargetPlayer)
		{
			if (pTargetPlayer->isNier)
			{
				if (pTargetPlayer->IsInWorld())
				{
					std::ostringstream unbondStream;
					uint32 unbond = 0;
					for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
					{
						Player::BoundInstancesMap& binds = pTargetPlayer->GetBoundInstances(Difficulty(i));
						for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
						{
							if (itr->first != pTargetPlayer->GetMapId())
							{
								DungeonPersistentState* save = itr->second.state;
								pTargetPlayer->UnbindInstance(itr, Difficulty(i));
								unbond++;
							}
							else
							{
								unbondStream << "currently in map " << itr->first << ", can not unbond. ";
								++itr;
							}
						}
					}
					unbondStream << "unbond " << unbond << " instances";
					pTargetPlayer->Whisper(unbondStream.str(), Language::LANG_UNIVERSAL, pCommander->GetObjectGuid());
				}
			}
		}
	}
}

void NierManager::HandlePacket(const WorldSession* pmSession, WorldPacket pmPacket)
{
	switch (pmPacket.GetOpcode())
	{
	case SMSG_CHAR_ENUM:
	{
		std::unordered_set<uint32> myCharacterIdSet;

		std::ostringstream sqlStream;
		sqlStream << "SELECT guid FROM characters where account = " << pmSession->GetAccountId();
		auto characterQR = CharacterDatabase.Query(sqlStream.str().c_str());
		if (characterQR)
		{
			Field* characterFields = characterQR->Fetch();
			uint32 character_id = characterFields[0].GetUInt32();
			if (character_id > 0)
			{
				myCharacterIdSet.insert(character_id);
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
			else if (!receiver->isNier)
			{
				break;
			}
			if (NierScript_Base* nb = receiver->_nierScript)
			{
				if (Group* myGroup = receiver->GetGroup())
				{
					receiver->RemoveFromGroup();
				}
				if (Group* groupInvite = receiver->GetGroupInvite())
				{
					for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
					{
						Player::BoundInstancesMap& binds = receiver->GetBoundInstances(Difficulty(i));
						for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
						{
							if (itr->first != receiver->GetMapId())
							{
								DungeonPersistentState* save = itr->second.state;
								receiver->UnbindInstance(itr, Difficulty(i));
							}
							else
							{
								++itr;
							}
						}
					}

					WorldPacket wpAccept(CMSG_GROUP_ACCEPT, 4);
					wpAccept << uint32(0);
					receiver->GetSession()->HandleGroupAcceptOpcode(wpAccept);

					ObjectGuid leaderGuid = groupInvite->GetLeaderGuid();
					std::ostringstream replyStream;
					replyStream << "Role is : " << groupRoleNameMap[receiver->groupRole];
					replyStream << " Specialty is : " << characterTalentTabNameMap[receiver->getClass()][nb->specialty];
					receiver->Whisper(replyStream.str(), Language::LANG_UNIVERSAL, leaderGuid);
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
		//    if (newLeader->GetObjectGuid() == me->GetObjectGuid())
		//    {
		//        WorldPacket data(CMSG_GROUP_SET_LEADER, 8);
		//        data << master->GetObjectGuid().WriteAsPacked();
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
			if (receiver->isRessurectRequested())
			{
				receiver->ResurrectUsingRequestDataInit();
				receiver->ClearInCombat();
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

void NierManager::RandomTeleport(Player* me, Player* target)
{
	if (!target)
	{
		return;
	}
	float nearX = 0.0f;
	float nearY = 0.0f;
	float nearZ = 0.0f;
	float nearDistance = frand(200.0f, 400.0f);
	float nearAngle = frand(0.0f, M_PI * 2);
	target->GetNearPoint(target, nearX, nearY, nearZ, 0.0f, nearDistance, nearAngle);
	me->ClearInCombat();
	me->TeleportTo(me->GetMapId(), nearX, nearY, nearZ, 0);
}

void NierManager::AddNier(uint32 pMasterId, uint32 pCareer, bool pAlliance, uint32 pNierType)
{
	uint32 race = 0;
	if (pAlliance)
	{
		race = sNierManager->allianceRaces[pCareer].size();
		race = urand(1, race);
		if (race >= sNierManager->allianceRaces[pCareer].size())
		{
			race = sNierManager->allianceRaces[pCareer].size() - 1;
		}
		race = sNierManager->allianceRaces[pCareer][race];
	}
	else
	{
		race = sNierManager->hordeRaces[pCareer].size();
		race = urand(1, race);
		if (race >= sNierManager->hordeRaces[pCareer].size())
		{
			race = sNierManager->hordeRaces[pCareer].size() - 1;
		}
		race = sNierManager->hordeRaces[pCareer][race];
	}

	std::ostringstream nierInsertStream;
	nierInsertStream << "INSERT INTO `nier` (`master_id`, `account_id`, `character_id`, `account_name`, `race`, `career`, `specialty`, `role`, `nier_type`) VALUES (" << pMasterId << ", 0, 0, '', " << race << ", " << pCareer << ", 1, 1, " << pNierType << ")";
	CharacterDatabase.DirectExecute(nierInsertStream.str().c_str());
	sLog.outBasic("nier added : %d, %d", race, pCareer);
}

void NierManager::UpdateNierManager(uint32 pDiff)
{
	if (sNierConfig.Enable == 0)
	{
		return;
	}

	if (nierMap.size() > 0)
	{
		for (std::unordered_map<uint32, NierEntity*>::iterator nierIT = nierMap.begin(); nierIT != nierMap.end(); nierIT++)
		{
			nierIT->second->Update(pDiff);
		}
		int updates = 0;
		while (true)
		{
			if (nierMap.find(updateIndex) == nierMap.end())
			{
				updateIndex = 0;
				break;
			}
			if (nierMap[updateIndex]->entityState == NierState::NierState_Online)
			{
				if (Player* nierPlayer = nierMap[updateIndex]->me)
				{
					if (nierPlayer->IsInWorld())
					{
						if (!nierPlayer->IsBeingTeleported())
						{
							if (NierScript_Base* nierScript = nierPlayer->_nierScript)
							{
								nierScript->Update(pDiff);
								if (updates > sNierConfig.UpdateCount)
								{
									updates++;
									break;
								}
							}
						}
					}
				}
			}
			updateIndex++;
		}
	}
}