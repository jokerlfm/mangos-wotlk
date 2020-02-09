#include "RobotAI.h"

#include "Log.h"
#include "Accounts/AccountMgr.h"
#include "Entities/Pet.h"
#include "Entities/Item.h"
#include "Groups/Group.h"
#include "Grids/GridNotifiers.h"
#include "Globals/ObjectMgr.h"
#include "MotionGenerators/MotionMaster.h"
#include "MotionGenerators/MovementGenerator.h"
#include "Maps/MapManager.h"
#include "Spells/SpellMgr.h"
#include "Spells/SpellAuras.h"
#include "Server/WorldSession.h"
#include "World/World.h"

#include "RobotConfig.h"
#include "RobotManager.h"
#include "Scripts/Script_Warrior.h"
#include "Scripts/Script_Hunter.h"
#include "Scripts/Script_Shaman.h"
#include "Scripts/Script_Paladin.h"
#include "Scripts/Script_Warlock.h"
#include "Scripts/Script_Priest.h"
#include "Scripts/Script_Rogue.h"
#include "Scripts/Script_Mage.h"
#include "Scripts/Script_Druid.h"

RobotAI::RobotAI(uint32 pmTargetLevel, uint32 pmTargetClass, uint32 pmTargetRace)
{
	realPrevTime = 0;
	strategiesMap.clear();
	strategiesMap["solo_normal"] = true;
	strategiesMap["group_normal"] = false;
	characterTalentTab = 0;


	realPrevTime = 0;

	targetLevel = pmTargetLevel;
	targetClass = pmTargetClass;
	targetRace = pmTargetRace;

	std::ostringstream accountNameStream;
	accountNameStream << sRobotConfig.RobotAccountNamePrefix << "l" << std::to_string(targetLevel) << "r" << std::to_string(targetRace) << "c" << std::to_string(targetClass);
	accountName = accountNameStream.str();

	accountID = 0;
	characterGUID = ObjectGuid();
	masterGUID = ObjectGuid();
	characterType = 0;

	checkDelay = urand(TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);

	// EJ debug 
	//checkDelay = 30 * TimeConstants::IN_MILLISECONDS;

	onlineDelay = 0;
	offlineDelay = 0;

	robotState = RobotState::RobotState_OffLine;

	spellIDMap.clear();
	spellLevelMap.clear();
	interestMap.clear();

	st_Solo_Normal = new Strategy_Solo_Normal(this);
	st_Group_Normal = new Strategy_Group_Normal(this);

	switch (targetClass)
	{
	case Classes::CLASS_WARRIOR:
	{
		s_base = new Script_Warrior(this);
		break;
	}
	case Classes::CLASS_HUNTER:
	{
		s_base = new Script_Hunter(this);
		break;
	}
	case Classes::CLASS_SHAMAN:
	{
		s_base = new Script_Shaman(this);
		break;
	}
	case Classes::CLASS_PALADIN:
	{
		s_base = new Script_Paladin(this);
		break;
	}
	case Classes::CLASS_WARLOCK:
	{
		s_base = new Script_Warlock(this);
		break;
	}
	case Classes::CLASS_PRIEST:
	{
		s_base = new Script_Priest(this);
		break;
	}
	case Classes::CLASS_ROGUE:
	{
		s_base = new Script_Rogue(this);
		break;
	}
	case Classes::CLASS_MAGE:
	{
		s_base = new Script_Mage(this);
		break;
	}
	case Classes::CLASS_DRUID:
	{
		s_base = new Script_Druid(this);
		break;
	}
	}

	staying = false;
	holding = false;
}

RobotAI::~RobotAI()
{

}

void RobotAI::SetStrategy(std::string pmStrategyName, bool pmEnable)
{
	if (strategiesMap.find(pmStrategyName) != strategiesMap.end())
	{
		strategiesMap[pmStrategyName] = pmEnable;
	}
}

void RobotAI::ResetStrategy()
{
	strategiesMap["solo_normal"] = true;
	st_Solo_Normal->soloDuration = 0;
	strategiesMap["group_normal"] = false;
	staying = false;
	holding = false;
	masterGUID = ObjectGuid();
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	me->StopMoving();
	me->GetMotionMaster()->Clear();
	me->Say("Strategy reset", Language::LANG_UNIVERSAL);

	Prepare();
}

void RobotAI::InitializeCharacter()
{
	bool newCharacter = false;
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	if (me->getLevel() != targetLevel)
	{
		newCharacter = true;
	}

	if (newCharacter)
	{
		me->GiveLevel(targetLevel);
		me->LearnDefaultSkills();
	}

	if (me->GetFreeTalentPoints() > 0)
	{
		me->resetTalents(true, true);
		uint8 specialty = urand(0, 2);
		// EJ fixed specialty
		if (me->getClass() == Classes::CLASS_MAGE)
		{
			specialty = 2;
		}
		else if (me->getClass() == Classes::CLASS_ROGUE)
		{
			specialty = 1;
		}
		else if (me->getClass() == Classes::CLASS_WARRIOR)
		{
			specialty = 1;
		}
		else if (me->getClass() == Classes::CLASS_SHAMAN)
		{
			specialty = 2;
		}
		else if (me->getClass() == Classes::CLASS_PRIEST)
		{
			specialty = 1;
		}
		else if (me->getClass() == Classes::CLASS_WARLOCK)
		{
			specialty = 2;
		}

		uint32 classMask = me->getClassMask();
		std::map<uint32, std::vector<TalentEntry const*> > talentsMap;
		for (uint32 i = 0; i < sTalentStore.GetNumRows(); ++i)
		{
			TalentEntry const* talentInfo = sTalentStore.LookupEntry(i);
			if (!talentInfo)
				continue;

			TalentTabEntry const* talentTabInfo = sTalentTabStore.LookupEntry(talentInfo->TalentTab);
			if (!talentTabInfo || talentTabInfo->tabpage != specialty)
				continue;

			if ((classMask & talentTabInfo->ClassMask) == 0)
				continue;

			talentsMap[talentInfo->Row].push_back(talentInfo);
		}
		for (std::map<uint32, std::vector<TalentEntry const*> >::iterator i = talentsMap.begin(); i != talentsMap.end(); ++i)
		{
			std::vector<TalentEntry const*> eachRowTalents = i->second;
			if (eachRowTalents.empty())
			{
				sLog.outError("%s: No spells for talent row %d", me->GetName(), i->first);
				continue;
			}
			for (std::vector<TalentEntry const*>::iterator it = eachRowTalents.begin(); it != eachRowTalents.end(); it++)
			{
				uint8 maxRank = 4;
				if ((*it)->RankID[4] > 0)
				{
					maxRank = 4;
				}
				else if ((*it)->RankID[3] > 0)
				{
					maxRank = 3;
				}
				else if ((*it)->RankID[2] > 0)
				{
					maxRank = 2;
				}
				else if ((*it)->RankID[1] > 0)
				{
					maxRank = 1;
				}
				else
				{
					maxRank = 0;
				}
				me->LearnTalent((*it)->TalentID, maxRank);
			}
		}
		me->SaveToDB();
	}

	if (newCharacter)
	{
		for (std::set<uint32>::iterator questIT = sRobotManager->spellRewardClassQuestIDSet.begin(); questIT != sRobotManager->spellRewardClassQuestIDSet.end(); questIT++)
		{
			const Quest* eachQuest = sObjectMgr.GetQuestTemplate((*questIT));
			if (me->SatisfyQuestLevel(eachQuest, false) && me->SatisfyQuestClass(eachQuest, false) && me->SatisfyQuestRace(eachQuest, false))
			{
				SpellEntry const* pSTCast = sSpellTemplate.LookupEntry<SpellEntry>(eachQuest->GetRewSpellCast());
				if (pSTCast)
				{
					std::set<uint32> spellToLearnIDSet;
					spellToLearnIDSet.clear();
					for (size_t effectCount = 0; effectCount < MAX_SPELL_EFFECTS; effectCount++)
					{
						if (pSTCast->Effect[effectCount] == SpellEffects::SPELL_EFFECT_LEARN_SPELL)
						{
							spellToLearnIDSet.insert(pSTCast->EffectTriggerSpell[effectCount]);
						}
					}
					if (spellToLearnIDSet.size() == 0)
					{
						spellToLearnIDSet.insert(pSTCast->Id);
					}
					for (std::set<uint32>::iterator toLearnIT = spellToLearnIDSet.begin(); toLearnIT != spellToLearnIDSet.end(); toLearnIT++)
					{
						me->learnSpell((*toLearnIT), false);
					}
				}
				SpellEntry const* pST = sSpellTemplate.LookupEntry<SpellEntry>(eachQuest->GetRewSpell());
				if (pST)
				{
					std::set<uint32> spellToLearnIDSet;
					spellToLearnIDSet.clear();
					for (size_t effectCount = 0; effectCount < MAX_SPELL_EFFECTS; effectCount++)
					{
						if (pST->Effect[effectCount] == SpellEffects::SPELL_EFFECT_LEARN_SPELL)
						{
							spellToLearnIDSet.insert(pST->EffectTriggerSpell[effectCount]);
						}
					}
					if (spellToLearnIDSet.size() == 0)
					{
						spellToLearnIDSet.insert(pST->Id);
					}
					for (std::set<uint32>::iterator toLearnIT = spellToLearnIDSet.begin(); toLearnIT != spellToLearnIDSet.end(); toLearnIT++)
					{
						me->learnSpell((*toLearnIT), false);
					}
				}
			}
		}

		for (uint32 i = 1; i < sCreatureStorage.GetMaxEntry(); ++i)
		{
			CreatureInfo const* cInfo = sCreatureStorage.LookupEntry<CreatureInfo>(i);
			if (!cInfo)
			{
				continue;
			}

			bool isTrainer = false;
			if (cInfo->TrainerType == TRAINER_TYPE_TRADESKILLS)
			{
				isTrainer = true;
			}
			if (!isTrainer)
			{
				if (cInfo->TrainerType == TRAINER_TYPE_CLASS && cInfo->TrainerClass == me->getClass())
				{
					isTrainer = true;
				}
			}
			if (!isTrainer)
			{
				continue;
			}

			uint32 trainerId = cInfo->Entry;

			TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerTemplateSpells(cInfo->TrainerTemplateId);

			if (!trainer_spells)
				continue;

			for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
			{
				TrainerSpell const* tSpell = &itr->second;
				if (!tSpell)
				{
					continue;
				}
				if (tSpell->spell == 0)
				{
					continue;
				}
				if (!me->IsSpellFitByClassAndRace(tSpell->spell))
				{
					continue;
				}
				TrainerSpellState state = me->GetTrainerSpellState(tSpell, tSpell->reqLevel);
				if (state != TRAINER_SPELL_GREEN)
				{
					continue;
				}

				uint32 checkSpellID = tSpell->spell;
				while (true)
				{
					const SpellEntry* pSpell = sSpellTemplate.LookupEntry<SpellEntry>(checkSpellID);
					if (!pSpell)
					{
						break;
					}
					if (pSpell->Effect[0] == SPELL_EFFECT_LEARN_SPELL)
					{
						checkSpellID = pSpell->EffectTriggerSpell[0];
					}
					else
					{
						break;
					}
				}
				me->learnSpell(checkSpellID, false);
			}
		}

		if (me->getClass() == Classes::CLASS_HUNTER)
		{
			sLog.outBasic("Create pet for player %s", me->GetName());
			uint32 beastEntry = urand(0, sRobotManager->tamableBeastEntryMap.size() - 1);
			beastEntry = sRobotManager->tamableBeastEntryMap[beastEntry];
			CreatureInfo const* cinfo = sObjectMgr.GetCreatureTemplate(beastEntry);
			if (!cinfo)
			{
				return;
			}

			Pet* pet = new Pet(HUNTER_PET);
			uint32 guid = me->GetMap()->GenerateLocalLowGuid(HighGuid::HIGHGUID_PET);
			uint32 pet_number = sObjectMgr.GeneratePetNumber();
			CreatureCreatePos pos(me, me->GetOrientation());
			if (!pet->Create(guid, pos, cinfo, pet_number))
			{
				delete pet;
				return;
			}
			pet->SetLoading(true);
			pet->SetOwnerGuid(me->GetObjectGuid());
			pet->setFaction(me->getFaction());
			pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, 1515);
			if (me->IsPvP())
				pet->SetPvP(true);
			if (me->IsPvPFreeForAll())
				pet->SetPvPFreeForAll(true);
			if (me->IsPvPSanctuary())
				pet->SetPvPSanctuary(true);
			pet->SetCanModifyStats(true);
			pet->InitStatsForLevel(me->getLevel());
			pet->InitLevelupSpellsForLevel();
			pet->InitTalentForLevel();
			// add pet object to the world
			pet->GetMap()->Add((Creature*)pet);
			pet->AIM_Initialize();
			pet->AI()->SetReactState(REACT_DEFENSIVE);
			// this enables pet details window (Shift+P)
			pet->InitPetCreateSpells();
			// this add pet auras
			pet->LearnPetPassives();
			pet->CastPetAuras(true);
			pet->CastOwnerTalentAuras();
			pet->UpdateAllStats();
			// caster have pet now
			me->SetPet(pet);
			me->PetSpellInitialize();
			pet->SetLoading(false);
			pet->SavePetToDB(PET_SAVE_AS_CURRENT, me);
		}

		me->SaveToDB();
		sLog.outBasic("Player %s basic info initialized", me->GetName());
	}

	spellLevelMap.clear();
	bool typeChecked = false;
	characterType = 0;
	sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 0;
	for (PlayerSpellMap::iterator it = me->GetSpellMap().begin(); it != me->GetSpellMap().end(); it++)
	{
		SpellEntry const* pST = sSpellTemplate.LookupEntry<SpellEntry>(it->first);
		if (pST)
		{
			std::string checkNameStr = std::string(pST->SpellName[0]);
			if (!typeChecked)
			{
				switch (me->getClass())
				{
				case Classes::CLASS_WARRIOR:
				{
					if (checkNameStr == "Improved Heroic Strike")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Booming Voice")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Shield Specialization")
					{
						characterTalentTab = 2;
						characterType = 1;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 1;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_HUNTER:
				{
					if (checkNameStr == "Improved Aspect of the Hawk")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Concussive Shot")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Monster Slaying")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_SHAMAN:
				{
					if (checkNameStr == "Convection")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Ancestral Knowledge")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Healing Wave")
					{
						characterTalentTab = 2;
						characterType = 2;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_PALADIN:
				{
					if (checkNameStr == "Divine Strength")
					{
						characterTalentTab = 0;
						characterType = 2;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Devotion Aura")
					{
						characterTalentTab = 1;
						characterType = 1;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Blessing of Might")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_WARLOCK:
				{
					if (checkNameStr == "Suppression")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Healthstone")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Shadow Bolt")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_PRIEST:
				{
					if (checkNameStr == "Unbreakable Will")
					{
						characterTalentTab = 0;
						characterType = 2;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
						typeChecked = true;
					}
					if (checkNameStr == "Healing Focus")
					{
						characterTalentTab = 1;
						characterType = 2;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
						typeChecked = true;
					}
					if (checkNameStr == "Spirit Tap")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_ROGUE:
				{
					if (checkNameStr == "Improved Eviscerate")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Gouge")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Master of Deception")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_MAGE:
				{
					if (checkNameStr == "Arcane Subtlety")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Fireball")
					{
						characterTalentTab = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Frost Warding")
					{
						characterTalentTab = 2;
						typeChecked = true;
					}
					break;
				}
				case Classes::CLASS_DRUID:
				{
					if (checkNameStr == "Improved Wrath")
					{
						characterTalentTab = 0;
						typeChecked = true;
					}
					if (checkNameStr == "Ferocity")
					{
						characterTalentTab = 1;
						characterType = 1;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 1;
						typeChecked = true;
					}
					if (checkNameStr == "Improved Mark of the Wild")
					{
						characterTalentTab = 2;
						characterType = 2;
						sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
						typeChecked = true;
					}
					break;
				}
				default:
				{
					break;
				}
				}
			}
			if (spellLevelMap.find(checkNameStr) == spellLevelMap.end())
			{
				spellLevelMap[checkNameStr] = pST->baseLevel;
				spellIDMap[checkNameStr] = it->first;
			}
			else
			{
				if (pST->baseLevel > spellLevelMap[checkNameStr])
				{
					spellLevelMap[checkNameStr] = pST->baseLevel;
					spellIDMap[checkNameStr] = it->first;
				}
			}
		}
	}

	if (newCharacter)
	{
		for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
		{
			if (Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
			{
				me->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
			}
		}
		switch (me->getClass())
		{
		case Classes::CLASS_WARRIOR:
		{
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;

			if (characterType == 1)
			{
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[1][checkLevelRange].size() > 0)
					{
						// use one hand sword
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[1][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[1][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}

				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[4][checkLevelRange].size() > 0)
					{
						// use shield
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[4][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[4][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			else
			{
				int levelRange = me->getLevel() / 10;
				int checkLevelRange = levelRange;
				bool validEquip = false;
				int maxTryTimes = 5;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[2][checkLevelRange].size() > 0)
					{
						// use two hand sword
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[2][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[2][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			int armorType = 2;
			if (me->getLevel() < 40)
			{
				// use mail armor
				armorType = 2;
			}
			else
			{
				// use plate armor
				armorType = 3;
			}
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_HUNTER:
		{
			// set two hand axe, two hand sword, polearms		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				int weaponType = 0;
				if (urand(0, 2) == 0)
				{
					weaponType = 6;
					if (sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() > 0)
					{
						// use polearms
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							if (sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() == 0)
							{
								break;
							}
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[weaponType][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
				}
				if (urand(0, 1) == 0)
				{
					weaponType = 2;
					if (sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() > 0)
					{
						// use two hand sword
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[weaponType][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
				}
				weaponType = 5;
				if (sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() > 0)
				{
					// use two hand axe
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[weaponType][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[weaponType][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}
			checkLevelRange = levelRange;
			validEquip = false;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->rangeWeaponMap[0][checkLevelRange].size() > 0)
				{
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->rangeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->rangeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}
			int armorType = 1;
			if (me->getLevel() < 40)
			{
				// use leather armor
				armorType = 1;
			}
			else
			{
				// use mail armor
				armorType = 2;
			}
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// quiver and ammo pouch
			Item* weapon = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
			if (weapon)
			{
				uint32 subClass = weapon->GetProto()->SubClass;
				if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW || subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW)
				{
					me->StoreNewItemInBestSlots(2101, 1);
				}
				else if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN)
				{
					me->StoreNewItemInBestSlots(2102, 1);
				}
			}
			break;
		}
		case Classes::CLASS_SHAMAN:
		{
			// set staff		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[0][checkLevelRange].size() > 0)
				{
					// use staff
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			int armorType = 1;
			if (me->getLevel() < 40)
			{
				// use leather armor
				armorType = 1;
			}
			else
			{
				// use mail armor
				armorType = 2;
			}
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_PALADIN:
		{
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;

			if (characterType == 1)
			{
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[1][checkLevelRange].size() > 0)
					{
						// use two hand sword
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[1][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[1][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}

				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[4][checkLevelRange].size() > 0)
					{
						// use shield
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[4][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[4][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			else
			{
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->meleeWeaponMap[2][checkLevelRange].size() > 0)
					{
						// use two hand sword
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->meleeWeaponMap[2][checkLevelRange].size() - 1);
							entry = sRobotManager->meleeWeaponMap[2][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			int armorType = 2;
			if (me->getLevel() < 40)
			{
				// use mail armor
				armorType = 2;
			}
			else
			{
				// use plate armor
				armorType = 3;
			}
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[armorType][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_WARLOCK:
		{
			// set staff		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[0][checkLevelRange].size() > 0)
				{
					// use staff
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// set wand
			checkLevelRange = levelRange;
			validEquip = false;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->rangeWeaponMap[1][checkLevelRange].size() > 0)
				{
					// use wand
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->rangeWeaponMap[1][checkLevelRange].size() - 1);
						entry = sRobotManager->rangeWeaponMap[1][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// use cloth armor
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_PRIEST:
		{
			// set staff		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[0][checkLevelRange].size() > 0)
				{
					// use staff
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// set wand
			checkLevelRange = levelRange;
			validEquip = false;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->rangeWeaponMap[1][checkLevelRange].size() > 0)
				{
					// use wand
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->rangeWeaponMap[1][checkLevelRange].size() - 1);
						entry = sRobotManager->rangeWeaponMap[1][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// use cloth armor
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_ROGUE:
		{
			// set dagger		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[3][checkLevelRange].size() > 0)
				{
					// use double dagger
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[3][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[3][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}
			checkLevelRange = levelRange;
			validEquip = false;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[3][checkLevelRange].size() > 0)
				{
					// use double dagger
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[3][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[3][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}
			// use leather armor
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_MAGE:
		{
			// set staff		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[0][checkLevelRange].size() > 0)
				{
					// use staff
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// set wand
			checkLevelRange = levelRange;
			validEquip = false;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->rangeWeaponMap[1][checkLevelRange].size() > 0)
				{
					// use wand
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->rangeWeaponMap[1][checkLevelRange].size() - 1);
						entry = sRobotManager->rangeWeaponMap[1][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// use cloth armor
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[0][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		case Classes::CLASS_DRUID:
		{
			// set staff		
			int levelRange = me->getLevel() / 10;
			int checkLevelRange = levelRange;
			bool validEquip = false;
			int maxTryTimes = 5;
			while (checkLevelRange >= 0)
			{
				if (sRobotManager->meleeWeaponMap[0][checkLevelRange].size() > 0)
				{
					// use staff
					for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
					{
						uint32 entry = urand(0, sRobotManager->meleeWeaponMap[0][checkLevelRange].size() - 1);
						entry = sRobotManager->meleeWeaponMap[0][checkLevelRange][entry];
						if (EquipNewItem(entry))
						{
							validEquip = true;
							break;
						}
					}
					if (validEquip)
					{
						break;
					}
				}
				checkLevelRange--;
			}

			// use leather armor
			for (std::set<uint8>::iterator inventoryTypeIT = sRobotManager->armorInventorySet.begin(); inventoryTypeIT != sRobotManager->armorInventorySet.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange].size() - 1);
							entry = sRobotManager->armorMap[1][(*inventoryTypeIT)][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}

			// misc equip        
			for (std::unordered_map<uint8, uint8>::iterator inventoryTypeIT = sRobotManager->miscInventoryMap.begin(); inventoryTypeIT != sRobotManager->miscInventoryMap.end(); inventoryTypeIT++)
			{
				checkLevelRange = levelRange;
				validEquip = false;
				while (checkLevelRange >= 0)
				{
					if (sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() > 0)
					{
						for (int checkCount = 0; checkCount < maxTryTimes; checkCount++)
						{
							uint32 entry = urand(0, sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange].size() - 1);
							entry = sRobotManager->miscMap[inventoryTypeIT->second][checkLevelRange][entry];
							if (EquipNewItem(entry))
							{
								validEquip = true;
								break;
							}
						}
						if (validEquip)
						{
							break;
						}
					}
					checkLevelRange--;
				}
			}
			break;
		}
		default:
		{
			break;
		}
		}
		me->SaveToDB();
		sLog.outBasic("Player %s equip info initialized", me->GetName());
	}

	me->UpdateSkillsForLevel(true);
	if (!me->IsPvP())
	{
		me->SetPvP(true);
	}

	if (me->getClass() == Classes::CLASS_WARLOCK)
	{
		switch (characterTalentTab)
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
			uint32 g0 = me->GetGlyph(0);
			if (g0 == 0)
			{
				me->CastSpell(me, 56270, TriggerCastFlags::TRIGGERED_NONE);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	me->SaveToDB();

	std::ostringstream msgStream;
	msgStream << me->GetName() << " initialized";
	sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
}

void RobotAI::Prepare()
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	me->DurabilityRepairAll(false, 0, false);
	if (me->getClass() == Classes::CLASS_HUNTER)
	{
		uint32 ammoEntry = 0;
		Item* weapon = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
		if (weapon)
		{
			uint32 subClass = weapon->GetProto()->SubClass;
			uint8 playerLevel = me->getLevel();
			if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW || subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW)
			{
				if (playerLevel >= 40)
				{
					ammoEntry = 11285;
				}
				else if (playerLevel >= 25)
				{
					ammoEntry = 3030;
				}
				else
				{
					ammoEntry = 2515;
				}
			}
			else if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN)
			{
				if (playerLevel >= 40)
				{
					ammoEntry = 11284;
				}
				else if (playerLevel >= 25)
				{
					ammoEntry = 3033;
				}
				else
				{
					ammoEntry = 2519;
				}
			}
			if (ammoEntry > 0)
			{
				if (!me->HasItemCount(ammoEntry, 100))
				{
					me->StoreNewItemInBestSlots(ammoEntry, 1000);
					me->SetAmmo(ammoEntry);
				}
			}
		}
	}
	Pet* checkPet = me->GetPet();
	if (checkPet)
	{
		checkPet->AI()->SetReactState(REACT_DEFENSIVE);
		if (checkPet->getPetType() == PetType::HUNTER_PET)
		{
			checkPet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 3);
		}
		std::unordered_map<uint32, PetSpell> petSpellMap = checkPet->m_spells;
		for (std::unordered_map<uint32, PetSpell>::iterator it = petSpellMap.begin(); it != petSpellMap.end(); it++)
		{
			if (it->second.active == ACT_DISABLED || it->second.active == ACT_ENABLED)
			{
				SpellEntry const* pST = sSpellTemplate.LookupEntry<SpellEntry>(it->first);
				if (pST)
				{
					std::string checkNameStr = std::string(pST->SpellName[0]);
					if (checkNameStr == "Prowl")
					{
						checkPet->ToggleAutocast(pST->Id, false);
					}
					else if (checkNameStr == "Phase Shift")
					{
						checkPet->ToggleAutocast(pST->Id, false);
					}
					else if (checkNameStr == "Fire Shield")
					{
						checkPet->ToggleAutocast(pST->Id, false);
					}
					else
					{
						checkPet->ToggleAutocast(pST->Id, true);
					}
				}
			}
		}
	}
}

void RobotAI::Refresh()
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	if (!me->isAlive())
	{
		me->ResurrectPlayer(100);
	}
	me->SetHealthPercent(100.0f);
	me->ClearInCombat();
	me->getThreatManager().clearReferences();
	me->GetMotionMaster()->Clear();
}

void RobotAI::RandomTeleport()
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	if (me->IsBeingTeleported())
	{
		return;
	}
	uint8 levelRange = me->getLevel() / 10;
	uint32 destIndex = urand(0, sRobotManager->teleportCacheMap[levelRange].size() - 1);
	WorldLocation destWL = sRobotManager->teleportCacheMap[levelRange][destIndex];
	Map* targetMap = sMapMgr.FindMap(destWL.mapid);
	if (targetMap)
	{
		me->TeleportTo(destWL);
		sLog.outBasic("Teleport robot %s (level %d)", me->GetName(), me->getLevel());
	}
}

bool RobotAI::HasAura(Unit* pmTarget, std::string pmSpellName, bool pmOnlyMyAura)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return false;
	}
	Unit* target = pmTarget;
	if (!pmTarget)
	{
		target = me;
	}
	std::set<uint32> spellIDSet = sRobotManager->spellNameEntryMap[pmSpellName];
	for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
	{
		if (pmOnlyMyAura)
		{
			if (target->HasAura((*it)))
			{
				std::multimap< uint32, SpellAuraHolder*> auras = target->GetSpellAuraHolderMap();
				for (std::multimap< uint32, SpellAuraHolder*>::iterator aIT = auras.begin(); aIT != auras.end(); aIT++)
				{
					if (aIT->second->GetCasterGuid() == me->GetObjectGuid())
					{
						return true;
					}
				}
			}
		}
		else
		{
			if (target->HasAura(*it))
			{
				return true;
			}
		}
	}

	return false;
}

bool RobotAI::CastSpell(Unit* pmTarget, std::string pmSpellName, float pmDistance, bool pmCheckAura, bool pmOnlyMyAura, bool pmClearShapeshift)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return false;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_NO_FREE_MOVE))
	{
		return true;
	}
	if (me->IsNonMeleeSpellCasted(true))
	{
		return true;
	}
	if (pmCheckAura)
	{
		if (HasAura(pmTarget, pmSpellName, pmOnlyMyAura))
		{
			return false;
		}
	}
	uint32 spellID = FindSpellID(pmSpellName);
	if (!SpellValid(spellID))
	{
		return false;
	}
	Unit* target = pmTarget;
	if (!target)
	{
		target = me;
	}
	me->SetSelectionGuid(target->GetObjectGuid());
	if (target->GetObjectGuid() != me->GetObjectGuid())
	{
		if (me->GetDistance(target) > pmDistance)
		{
			return false;
		}
		else if (!me->IsWithinLOSInMap(target))
		{
			return true;
		}
		else if (!me->isInFront(pmTarget, pmDistance))
		{
			me->SetFacingToObject(pmTarget);
			return true;
		}
	}
	if (pmClearShapeshift)
	{
		ClearShapeshift();
	}
	SpellEntry const* pST = sSpellTemplate.LookupEntry<SpellEntry>(spellID);
	if (!pST)
	{
		return false;
	}
	if (target->IsImmuneToSpell(pST, false, 0))
	{
		return false;
	}
	for (size_t i = 0; i < MAX_SPELL_REAGENTS; i++)
	{
		if (pST->Reagent[i] > 0)
		{
			if (!me->HasItemCount(pST->Reagent[i], pST->ReagentCount[i]))
			{
				me->StoreNewItemInBestSlots(pST->Reagent[i], pST->ReagentCount[i] * 10);
			}
		}
	}
	if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
	{
		me->SetStandState(UNIT_STAND_STATE_STAND);
	}
	if (IsChanneledSpell(pST) || GetSpellCastTime(pST, me) > 0)
	{
		me->StopMoving();
	}
	me->CastSpell(target, spellID, TriggerCastFlags::TRIGGERED_NONE);

	return true;
}

void RobotAI::BaseMove(Unit* pmTarget, float pmDistance, bool pmAttack)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_CAN_NOT_MOVE))
	{
		return;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_NO_FREE_MOVE))
	{
		return;
	}
	if (me->IsNonMeleeSpellCasted(false))
	{
		return;
	}
	// Can't attack if owner is pacified
	if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
	{
		//pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
		/// @todo Send proper error message to client
		return;
	}
	if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
	{
		me->SetStandState(UNIT_STAND_STATE_STAND);
	}
	me->SetSelectionGuid(pmTarget->GetObjectGuid());
	if (pmAttack)
	{
		me->Attack(pmTarget, true);
	}

	float currentDistance = me->GetDistance(pmTarget);
	if (currentDistance < pmDistance)
	{
		if (!me->isInFront(pmTarget, pmDistance))
		{
			me->SetFacingToObject(pmTarget);
		}
		else
		{
			me->StopMoving();
			me->GetMotionMaster()->Clear();
		}
	}
	else
	{
		if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == MovementGeneratorType::CHASE_MOTION_TYPE)
		{
			const MovementGenerator* mg = me->GetMotionMaster()->GetCurrent();
			if (mg)
			{
				if (mg->GetCurrentTarget()->GetObjectGuid() != pmTarget->GetObjectGuid())
				{
					me->StopMoving();
					me->GetMotionMaster()->Clear();
					me->GetMotionMaster()->MoveChase(pmTarget, pmDistance, 0, false, false, pmAttack);
				}
			}
		}
		else
		{
			me->GetMotionMaster()->MoveChase(pmTarget, pmDistance, 0, false, false, pmAttack);
		}
	}
}

void RobotAI::WhisperTo(std::string pmContent, Language pmLanguage, Player* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	me->Whisper(pmContent, pmLanguage, pmTarget->GetObjectGuid());
}

void RobotAI::HandlePacket(WorldPacket const& pmDestPacket)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	switch (pmDestPacket.GetOpcode())
	{
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
		Group* grp = me->GetGroupInvite();
		if (!grp)
		{
			break;
		}
		Player* inviter = ObjectAccessor::FindPlayer(grp->GetLeaderGuid());
		if (!inviter)
		{
			break;
		}
		bool acceptInvite = true;
		if (inviter->getLevel() < me->getLevel())
		{
			uint32 lowGUID = inviter->GetGUIDLow();
			if (interestMap.find(lowGUID) == interestMap.end())
			{
				uint8 levelGap = me->getLevel() - inviter->getLevel();
				if (urand(0, levelGap) > 0)
				{
					acceptInvite = false;
				}
				interestMap[lowGUID] = acceptInvite;
			}
			else
			{
				acceptInvite = interestMap[lowGUID];
			}
			if (st_Solo_Normal->interestsDelay <= 0)
			{
				st_Solo_Normal->interestsDelay = 300 * TimeConstants::IN_MILLISECONDS;
			}
		}
		if (acceptInvite)
		{
			masterGUID = inviter->GetObjectGuid();
			WorldPacket p;
			uint32 roles_mask = 0;
			p << roles_mask;
			me->GetSession()->HandleGroupAcceptOpcode(p);			
			WhisperTo("Strategy set to group", Language::LANG_UNIVERSAL, inviter);
			WhisperTo("You are my master", Language::LANG_UNIVERSAL, inviter);
			std::ostringstream replyStream_Talent;
			replyStream_Talent << "My talent category is " << sRobotManager->characterTalentTabNameMap[me->getClass()][characterTalentTab];
			WhisperTo(replyStream_Talent.str(), Language::LANG_UNIVERSAL, inviter);
			std::ostringstream replyStream_GroupRole;
			replyStream_GroupRole << "My group role is ";
			switch (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()])
			{
			case 0:
			{
				replyStream_GroupRole << "dps";
				break;
			}
			case 1:
			{
				replyStream_GroupRole << "tank";
				break;
			}
			case 2:
			{
				replyStream_GroupRole << "healer";
				break;
			}
			default:
			{
				replyStream_GroupRole << "dps";
				break;
			}
			}
			WhisperTo(replyStream_GroupRole.str(), Language::LANG_UNIVERSAL, inviter);
			me->GetMotionMaster()->Clear();
			Prepare();
			SetStrategy("solo_normal", false);
			SetStrategy("group_normal", true);
			break;
		}
		else
		{
			WorldPacket p;
			me->GetSession()->HandleGroupDeclineOpcode(p);
			std::ostringstream timeLeftStream;
			timeLeftStream << "Not interested.";
			WhisperTo(timeLeftStream.str(), Language::LANG_UNIVERSAL, inviter);
			break;
		}
	}
	case SMSG_GROUP_UNINVITE:
	{
		ResetStrategy();
		me->Say("Strategy set to solo", Language::LANG_UNIVERSAL);
		//sRobotManager->RefreshRobot(me);
		break;
	}
	case SMSG_GROUP_LIST:
	{
		//uint8 groupType = 0;
		//const uint8* pktContents = pmDestPacket.contents();
		//groupType = *pktContents;
		//pktContents += 3;
		//if (groupType & GROUPTYPE_LFD)
		//{
		//	pktContents += 5;
		//}
		//pktContents += 8;
		//pktContents += 4;
		//uint32 memberCount = *pktContents;
		//if (memberCount == 0);
		//{
		//	ResetStrategy();
		//	me->Say("Strategy set to solo", Language::LANG_UNIVERSAL);
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
		//        if (!newLeader->isRobot)
		//        {
		//            master = newLeader;
		//        }
		//    }
		//}
		break;
	}
	case SMSG_FORCE_RUN_SPEED_CHANGE:
	{
		break;
	}
	case SMSG_RESURRECT_REQUEST:
	{
		if (!me->isAlive())
		{
			st_Solo_Normal->deathDuration = 0;
		}
		if (me->isRessurectRequested())
		{
			me->ResurrectUsingRequestDataInit();
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
	case SMSG_QUESTUPDATE_ADD_KILL:
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
		if (!me->duel)
		{
			break;
		}
		me->DuelComplete(DuelCompleteType::DUEL_INTERRUPTED);
		WhisperTo("Not interested", Language::LANG_UNIVERSAL, me->duel->opponent);
		break;
	}
	default:
	{
		break;
	}
	}
}

void RobotAI::Update()
{
	uint32 realCurrTime = WorldTimer::getMSTime();
	uint32 diff = WorldTimer::getMSTimeDiff(realPrevTime, realCurrTime);

	realPrevTime = realCurrTime;
	switch (robotState)
	{
	case RobotState_None:
	{
		break;
	}
	case RobotState_OffLine:
	{
		if (onlineDelay > 0)
		{
			onlineDelay -= diff;
			if (onlineDelay <= 0)
			{
				onlineDelay = 0;
				robotState = RobotState::RobotState_CheckAccount;
				allDelay = 5 * TimeConstants::IN_MILLISECONDS;
				sLog.outBasic("Robot account %s is ready to go online.", accountName.c_str());
			}
		}
		else if (checkDelay > 0)
		{
			checkDelay -= diff;
			if (checkDelay <= 0)
			{
				checkDelay = urand(TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
				bool levelPlayerOnline = false;
				if (sRobotConfig.OnlineLevel == targetLevel)
				{
					levelPlayerOnline = true;
					sLog.outBasic("Robot account %s is at online level.", accountName.c_str());
				}
				if (!levelPlayerOnline)
				{
					std::unordered_map<uint32, WorldSession*> allSessionMap = sWorld.GetSessions();
					for (std::unordered_map<uint32, WorldSession*>::iterator it = allSessionMap.begin(); it != allSessionMap.end(); it++)
					{
						if (!sRobotManager->IsRobot(it->first))
						{
							Player* eachPlayer = it->second->GetPlayer();
							if (eachPlayer)
							{
								if (eachPlayer->IsInWorld())
								{
									uint32 eachPlayerLevel = eachPlayer->getLevel();
									if (eachPlayerLevel > 10)
									{
										if (eachPlayerLevel == targetLevel)
										{
											levelPlayerOnline = true;
											sLog.outBasic("Robot account %s is same level as active player.", accountName.c_str());
											break;
										}
									}
								}
							}
						}
					}
				}
				if (levelPlayerOnline)
				{
					onlineDelay = urand(sRobotConfig.OnlineMinDelay, sRobotConfig.OnlineMaxDelay);
					sLog.outBasic("Robot account %s online delay is set to %d.", accountName.c_str(), onlineDelay);
				}
			}
		}
		break;
	}
	case RobotState_CheckAccount:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			accountID = sRobotManager->CheckRobotAccount(accountName);
			if (accountID > 0)
			{
				sRobotManager->robotAICache[accountID] = this;
				sLog.outBasic("Robot account %s account ready.", accountName.c_str());
				robotState = RobotState::RobotState_CheckCharacter;
			}
			else
			{
				sLog.outBasic("Robot account %s account not ready.", accountName.c_str());
				robotState = RobotState::RobotState_CreateAccount;
			}
		}
		break;
	}
	case RobotState_CreateAccount:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			if (sRobotManager->CreateRobotAccount(accountName))
			{
				robotState = RobotState::RobotState_CheckAccount;
			}
			else
			{
				robotState = RobotState::RobotState_None;
			}
		}
		break;
	}
	case RobotState_CheckCharacter:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			characterGUID = sRobotManager->CheckAccountCharacter(accountID);
			if (!characterGUID.IsEmpty())
			{
				sLog.outBasic("Robot account %s character ready.", accountName.c_str());
				robotState = RobotState::RobotState_DoLogin;
			}
			else
			{
				sLog.outBasic("Robot account %s character not ready.", accountName.c_str());
				robotState = RobotState::RobotState_CreateCharacter;
			}
		}
		break;
	}
	case RobotState_CreateCharacter:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			if (sRobotManager->CreateRobotCharacter(accountID, targetClass, targetRace))
			{
				robotState = RobotState::RobotState_CheckCharacter;
			}
			else
			{
				robotState = RobotState::RobotState_None;
			}
		}
		break;
	}
	case RobotState_CheckLogin:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 10 * TimeConstants::IN_MILLISECONDS;
			Player* me = sRobotManager->CheckLogin(accountID, characterGUID);
			if (me)
			{
				WorldSession* mySession = me->GetSession();
				InitializeCharacter();
				me->SetPvP(true);
				robotState = RobotState::RobotState_Online;
				checkDelay = urand(TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
			}
			else
			{
				robotState = RobotState::RobotState_DoLogin;
				allDelay = 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;
			}
		}
		break;
	}
	case RobotState_DoLogin:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 10 * TimeConstants::IN_MILLISECONDS;
			sRobotManager->LoginRobot(accountID, characterGUID);
			robotState = RobotState::RobotState_CheckLogin;
		}
		break;
	}
	case RobotState_Online:
	{
		Player* me = ObjectAccessor::FindPlayer(characterGUID);
		if (!me)
		{
			break;
		}
		if (strategiesMap["solo_normal"])
		{
			st_Solo_Normal->Update(diff);
		}
		if (strategiesMap["group_normal"])
		{
			st_Group_Normal->Update(diff);
		}
		if (offlineDelay > 0)
		{
			offlineDelay -= diff;
			if (offlineDelay <= 0)
			{
				offlineDelay = 0;
				robotState = RobotState::RobotState_DoLogoff;
				allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			}
		}
		else if (checkDelay > 0)
		{
			checkDelay -= diff;
			if (checkDelay <= 0)
			{
				Prepare();
				checkDelay = urand(TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
				bool levelPlayerOnline = false;
				if (sRobotConfig.OnlineLevel == targetLevel)
				{
					levelPlayerOnline = true;
				}
				if (!levelPlayerOnline)
				{
					if (me->GetGroup())
					{
						Player* master = ObjectAccessor::FindPlayer(masterGUID);
						if (master)
						{
							if (master->IsInWorld())
							{
								if (me->IsInGroup(master))
								{
									levelPlayerOnline = true;
								}
							}
						}

						if (!levelPlayerOnline)
						{
							me->RemoveFromGroup();							
							RandomTeleport();
						}
					}
				}
				if (!levelPlayerOnline)
				{
					std::unordered_map<uint32, WorldSession*> allSessionMap = sWorld.GetSessions();
					for (std::unordered_map<uint32, WorldSession*>::iterator it = allSessionMap.begin(); it != allSessionMap.end(); it++)
					{
						if (!sRobotManager->IsRobot(it->first))
						{
							Player* eachPlayer = it->second->GetPlayer();
							if (eachPlayer)
							{
								if (eachPlayer->IsInWorld())
								{
									uint32 eachPlayerLevel = eachPlayer->getLevel();
									if (eachPlayerLevel > 10)
									{
										if (eachPlayerLevel == targetLevel)
										{
											levelPlayerOnline = true;
											break;
										}
									}
								}
							}
						}
					}
				}

				// EJ debug
				//levelPlayerOnline = false;

				if (!levelPlayerOnline)
				{
					offlineDelay = urand(5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);

					sLog.outBasic("Robot account %s offline delay is set to %d.", accountName.c_str(), offlineDelay);
				}
			}
		}
		break;
	}
	case RobotState_CheckLogoff:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 5 * TimeConstants::IN_MILLISECONDS;
			Player* checkP = ObjectAccessor::FindPlayer(characterGUID, true);
			if (checkP)
			{
				sLog.outError("Log out robot %s failed", checkP->GetName());
				robotState = RobotState::RobotState_None;
				break;
			}
			//sRobotManager->robotAICache.erase(accountID);

			robotState = RobotState::RobotState_OffLine;
		}
		break;
	}
	case RobotState_DoLogoff:
	{
		allDelay -= diff;
		if (allDelay <= 0)
		{
			allDelay = 0;
			Logout();
			robotState = RobotState::RobotState_CheckLogoff;
			allDelay = 10 * TimeConstants::IN_MILLISECONDS;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

Item* RobotAI::GetItemInInventory(uint32 pmEntry)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return NULL;
	}
	for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
		if (pItem)
		{
			if (pItem->GetEntry() == pmEntry)
			{
				return pItem;
			}
		}
	}

	for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		if (Bag* pBag = (Bag*)me->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		{
			for (uint32 j = 0; j < pBag->GetBagSize(); j++)
			{
				Item* pItem = me->GetItemByPos(i, j);
				if (pItem)
				{
					if (pItem->GetEntry() == pmEntry)
					{
						return pItem;
					}
				}
			}
		}
	}

	return NULL;
}

bool RobotAI::UseItem(Item* pmItem, Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}

	if (me->IsNonMeleeSpellCasted(true))
	{
		return false;
	}
	SpellCastTargets targets;
	targets.Update(pmTarget);

	// item spells cast at use
	ItemPrototype const* proto = pmItem->GetProto();
	if (!proto)
	{
		return false;
	}
	for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
	{
		_Spell const& spellData = proto->Spells[i];

		// no spell
		if (spellData.SpellId <= 0)
		{
			continue;
		}
		// wrong triggering type
		if (spellData.SpellTrigger != ITEM_SPELLTRIGGER_ON_USE)
		{
			continue;
		}
		SpellEntry const* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(spellData.SpellId);
		if (!spellInfo)
		{
			continue;
		}
		me->CastSpell(me, spellInfo, TriggerCastFlags::TRIGGERED_NONE);
		break;
	}

	std::ostringstream useRemarksStream;
	useRemarksStream << "Prepare to use item " << pmItem->GetProto()->Name1;

	me->Say(useRemarksStream.str(), Language::LANG_UNIVERSAL);
	return true;
}

bool RobotAI::EquipNewItem(uint32 pmEntry)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return false;
	}
	const ItemPrototype* targetIP = sObjectMgr.GetItemPrototype(pmEntry);
	if (!targetIP)
	{
		return false;
	}
	if (targetIP->RequiredLevel > me->getLevel())
	{
		return false;
	}
	if (Item* pItem = me->StoreNewItemInInventorySlot(pmEntry, 1))
	{
		uint16 eDest;
		// equip offhand weapon/shield if it attempt equipped before main-hand weapon
		InventoryResult msg = me->CanEquipItem(NULL_SLOT, eDest, pItem, false);
		if (msg == EQUIP_ERR_OK)
		{
			me->RemoveItem(INVENTORY_SLOT_BAG_0, pItem->GetSlot(), true);
			me->EquipItem(eDest, pItem, true);
			return true;
		}
	}

	return false;
}

void RobotAI::HandleChatCommand(Player* pmSender, std::string pmCMD)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	Player* master = ObjectAccessor::FindPlayer(masterGUID);
	std::vector<std::string> commandVector = sRobotManager->SplitString(pmCMD, " ", true);
	std::string commandName = commandVector.at(0);
	if (commandName == "role")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}

		std::ostringstream replyStream;
		if (commandVector.size() > 1)
		{
			std::string newRole = commandVector.at(1);
			if (newRole == "dps")
			{
				sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 0;
			}
			else if (newRole == "tank")
			{
				sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 1;
			}
			else if (newRole == "healer")
			{
				sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] = 2;
			}
		}

		replyStream << "My group role is ";
		switch (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()])
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
		WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
	}
	else if (commandName == "follow")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		staying = false;
		holding = false;
		if (commandVector.size() > 1)
		{
			std::string followDistanceStr = commandVector.at(1);
			float followDistance = atof(followDistanceStr.c_str());
			if (followDistance > FOLLOW_MIN_DISTANCE&& followDistance < FOLLOW_MAX_DISTANCE)
			{
				if (st_Group_Normal->Follow(followDistance))
				{
					WhisperTo("Following", Language::LANG_UNIVERSAL, pmSender);
				}
				else
				{
					WhisperTo("I will not follow you", Language::LANG_UNIVERSAL, pmSender);
				}
			}
			else
			{
				WhisperTo("Follow distance is invalid", Language::LANG_UNIVERSAL, pmSender);
			}
		}
		else
		{
			if (st_Group_Normal->Follow())
			{
				WhisperTo("Following", Language::LANG_UNIVERSAL, pmSender);
			}
			else
			{
				WhisperTo("I will not follow you", Language::LANG_UNIVERSAL, pmSender);
			}
		}
	}
	else if (commandName == "stay")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (st_Group_Normal->Stay())
		{
			staying = true;
			WhisperTo("Staying", Language::LANG_UNIVERSAL, pmSender);
		}
		else
		{
			WhisperTo("I will not stay", Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "hold")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		staying = false;
		holding = true;
		WhisperTo("Holding", Language::LANG_UNIVERSAL, pmSender);
	}
	else if (commandName == "heal")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] == 2)
		{
			staying = false;
			WhisperTo("I am ready to do healing", Language::LANG_UNIVERSAL, pmSender);
		}
		else
		{
			WhisperTo("I am not a healer", Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "attack")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		Unit* senderTarget = pmSender->GetTarget();
		staying = false;
		if (st_Group_Normal->Attack(senderTarget))
		{
			st_Group_Normal->instruction = Group_Instruction::Group_Instruction_Battle;
			me->SetSelectionGuid(senderTarget->GetObjectGuid());
			WhisperTo("Attack your target", Language::LANG_UNIVERSAL, pmSender);
		}
		else
		{
			WhisperTo("Can not attack your target", Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "rest")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		staying = false;
		if (st_Group_Normal->Rest(true))
		{
			WhisperTo("Resting", Language::LANG_UNIVERSAL, pmSender);
		}
		else
		{
			WhisperTo("Do not rest", Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "who")
	{
		WhisperTo(sRobotManager->characterTalentTabNameMap[me->getClass()][characterTalentTab], Language::LANG_UNIVERSAL, pmSender);
	}
	else if (commandName == "assemble")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (st_Group_Normal->assembleDelay > 0)
		{
			WhisperTo("I am coming", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (me->isAlive())
		{
			if (me->GetDistance(pmSender) < 50)
			{
				me->GetMotionMaster()->MovePoint(0, pmSender->GetPositionX(), pmSender->GetPositionY(), pmSender->GetPositionZ());
				WhisperTo("We are close, I will be ready in 5 seconds", Language::LANG_UNIVERSAL, pmSender);
			}
			else
			{
				st_Group_Normal->assembleDelay = 30 * TimeConstants::IN_MILLISECONDS;
				WhisperTo("I will come to you in 30 seconds", Language::LANG_UNIVERSAL, pmSender);
			}
		}
		else
		{
			st_Group_Normal->assembleDelay = 60 * TimeConstants::IN_MILLISECONDS;
			WhisperTo("I will revive and come to you in 60 seconds", Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "tank")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] == 1)
		{
			Unit* senderTarget = pmSender->GetTarget();
			staying = false;
			if (st_Group_Normal->Tank(senderTarget))
			{
				st_Group_Normal->instruction = Group_Instruction::Group_Instruction_Battle;
				me->SetSelectionGuid(senderTarget->GetObjectGuid());
				WhisperTo("Tank your target", Language::LANG_UNIVERSAL, pmSender);
			}
			else
			{
				WhisperTo("Can not tank your target", Language::LANG_UNIVERSAL, pmSender);
			}
		}
	}
	else if (commandName == "prepare")
	{
		Prepare();
		me->Say("I am prepared", Language::LANG_UNIVERSAL);
	}
	else if (commandName == "switch")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}

		if (me->getClass() == Classes::CLASS_WARLOCK || me->getClass() == Classes::CLASS_HUNTER)
		{
			Pet* checkPet = me->GetPet();
			if (!checkPet)
			{
				WhisperTo("I do not have an active pet", Language::LANG_UNIVERSAL, pmSender);
				return;
			}
			if (commandVector.size() > 1)
			{
				std::string autoState = commandVector.at(1);
				if (commandVector.size() > 2)
				{
					std::ostringstream spellNameStream;
					uint32 checkPos = 2;
					while (checkPos < commandVector.size())
					{
						spellNameStream << commandVector.at(checkPos) << " ";
						checkPos++;
					}
					std::string spellName = sRobotManager->TrimString(spellNameStream.str());
					std::unordered_map<uint32, PetSpell> petSpellMap = checkPet->m_spells;
					for (std::unordered_map<uint32, PetSpell>::iterator it = petSpellMap.begin(); it != petSpellMap.end(); it++)
					{
						if (it->second.active == ACT_DISABLED || it->second.active == ACT_ENABLED)
						{
							SpellEntry const* pST = sSpellTemplate.LookupEntry<SpellEntry>(it->first);
							if (pST)
							{
								std::string checkNameStr = std::string(pST->SpellName[0]);
								if (checkNameStr == spellName)
								{
									std::ostringstream replyStream;
									if (autoState == "on")
									{
										checkPet->ToggleAutocast(pST->Id, true);
										replyStream << "Switched " << spellName << " on";
									}
									else if (autoState == "off")
									{
										checkPet->ToggleAutocast(pST->Id, false);
										replyStream << "Switched " << spellName << " off";
									}
									else
									{
										replyStream << "Wrong auto state";
									}
									WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
									return;
								}
							}
						}
					}
					WhisperTo("Spell not found", Language::LANG_UNIVERSAL, pmSender);
					return;
				}
				else
				{
					WhisperTo("No spell name", Language::LANG_UNIVERSAL, pmSender);
					return;
				}
			}
			else
			{
				WhisperTo("No auto state", Language::LANG_UNIVERSAL, pmSender);
				return;
			}
		}
		else
		{
			WhisperTo("I am not hunter or a warlock", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
	}
	else if (commandName == "cast")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		Unit* senderTarget = pmSender->GetTarget();
		if (!senderTarget)
		{
			WhisperTo("You do not have a target", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (commandVector.size() > 1)
		{
			std::ostringstream targetStream;
			uint8 arrayCount = 0;
			for (std::vector<std::string>::iterator it = commandVector.begin(); it != commandVector.end(); it++)
			{
				if (arrayCount > 0)
				{
					targetStream << (*it) << " ";
				}
				arrayCount++;
			}
			std::string spellName = sRobotManager->TrimString(targetStream.str());
			std::ostringstream replyStream;
			if (CastSpell(senderTarget, spellName))
			{
				replyStream << "Cast spell " << spellName << " on " << senderTarget->GetName();
			}
			else
			{
				replyStream << "Can not cast spell " << spellName << " on " << senderTarget->GetName();
			}
			WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "cancel")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (!me->isAlive())
		{
			WhisperTo("I am dead", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		Unit* senderTarget = pmSender->GetTarget();
		if (!senderTarget)
		{
			WhisperTo("You do not have a target", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (commandVector.size() > 1)
		{
			std::ostringstream targetStream;
			uint8 arrayCount = 0;
			for (std::vector<std::string>::iterator it = commandVector.begin(); it != commandVector.end(); it++)
			{
				if (arrayCount > 0)
				{
					targetStream << (*it) << " ";
				}
				arrayCount++;
			}
			std::string spellName = sRobotManager->TrimString(targetStream.str());
			std::ostringstream replyStream;
			if (CancelAura(spellName))
			{
				replyStream << "Cancel aura " << spellName;
			}
			else
			{
				replyStream << "Unknown spell " << spellName;
			}
			WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
		}
	}
	else if (commandName == "use")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}

		std::ostringstream replyStream;
		if (commandVector.size() > 1)
		{
			std::string useType = commandVector.at(1);
			if (useType == "go")
			{
				if (commandVector.size() > 2)
				{
					std::ostringstream goNameStream;
					uint32 checkPos = 2;
					while (checkPos < commandVector.size())
					{
						goNameStream << commandVector.at(checkPos) << " ";
						checkPos++;
					}
					std::string goName = sRobotManager->TrimString(goNameStream.str());

					bool validToUse = false;

					std::list<Unit*> attackTargets;
					MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck u_check(me, DEFAULT_VISIBILITY_DISTANCE);
					MaNGOS::UnitListSearcher<MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck> searcher(attackTargets, u_check);
					Cell::VisitAllObjects(me, searcher, DEFAULT_VISIBILITY_DISTANCE);

					GameObjectList nearGOList;
					MaNGOS::GameObjectInPosRangeCheck go_check(*me, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), MELEE_MAX_DISTANCE);
					MaNGOS::GameObjectListSearcher<MaNGOS::GameObjectInPosRangeCheck> checker(nearGOList, go_check);
					Cell::VisitGridObjects(me, checker, MELEE_MAX_DISTANCE);

					for (std::list<GameObject*>::iterator it = nearGOList.begin(); it != nearGOList.end(); it++)
					{
						if ((*it)->GetName() == goName)
						{
							me->SetFacingToObject((*it));
							WorldPacket* const packetgouse = new WorldPacket(CMSG_GAMEOBJ_REPORT_USE, 8);
							*packetgouse << (*it)->GetObjectGuid();
							me->GetSession()->HandleGameObjectUseOpcode(*packetgouse);
							replyStream << "Use game object : " << goName;
							validToUse = true;
							break;
						}
					}
					if (!validToUse)
					{
						replyStream << "No go with name " << goName << " nearby";
					}
				}
				else
				{
					replyStream << "No go name";
				}
			}
			else if (useType == "item")
			{

			}
			else
			{
				replyStream << "Unknown type";
			}
		}
		else
		{
			replyStream << "Use what?";
		}
		WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
	}
	else if (commandName == "stop")
	{
		if (!master)
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		if (pmSender->GetObjectGuid() != master->GetObjectGuid())
		{
			WhisperTo("You are not my master", Language::LANG_UNIVERSAL, pmSender);
			return;
		}
		me->StopMoving();
		me->InterruptSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL);
		me->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
		me->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
		me->InterruptSpell(CurrentSpellTypes::CURRENT_MELEE_SPELL);
		std::ostringstream replyStream;
		replyStream << "Stopped";
		WhisperTo(replyStream.str(), Language::LANG_UNIVERSAL, pmSender);
	}
}

uint32 RobotAI::FindSpellID(std::string pmSpellName)
{
	if (spellIDMap.find(pmSpellName) != spellIDMap.end())
	{
		return spellIDMap[pmSpellName];
	}

	return 0;
}

bool RobotAI::SpellValid(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return false;
	}
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
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

bool RobotAI::CancelAura(std::string pmSpellName)
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return false;
	}
	std::set<uint32> spellIDSet = sRobotManager->spellNameEntryMap[pmSpellName];
	for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
	{
		if (me->HasAura((*it)))
		{
			CancelAura((*it));
			return true;
		}
	}

	return false;
}

void RobotAI::CancelAura(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return;
	}
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}
	WorldPacket data(CMSG_CANCEL_AURA, 4);
	data << pmSpellID;
	me->GetSession()->HandleCancelAuraOpcode(data);
}

void RobotAI::ClearShapeshift()
{
	Player* me = ObjectAccessor::FindPlayer(characterGUID);
	if (!me)
	{
		return;
	}

	uint32 spellID = 0;
	switch (me->GetShapeshiftForm())
	{
	case ShapeshiftForm::FORM_NONE:
	{
		break;
	}
	case ShapeshiftForm::FORM_CAT:
	{
		spellID = FindSpellID("Cat Form");
		break;
	}
	case ShapeshiftForm::FORM_DIREBEAR:
	{
		spellID = FindSpellID("Dire Bear Form");
		break;
	}
	case ShapeshiftForm::FORM_BEAR:
	{
		spellID = FindSpellID("Bear Form");
		break;
	}
	case ShapeshiftForm::FORM_MOONKIN:
	{
		spellID = FindSpellID("Moonkin Form");
		break;
	}
	default:
	{
		break;
	}
	}
	CancelAura(spellID);
}

void RobotAI::Logout()
{
	Player* checkP = ObjectAccessor::FindPlayer(characterGUID, true);
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