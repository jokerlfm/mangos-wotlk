#include "Nier_Hunter.h"
#include "NierManager.h"

Nier_Hunter::Nier_Hunter() :Nier_Base()
{
	spell_TameBeast = 1515;

	spell_CallPet = 0;
	spell_RevivePet = 0;
	spell_DismissPet = 0;
	spell_MendPet = 0;
	spell_AutoShot = 75;
	spell_SerpentSting = 0;
	spell_ArcaneShot = 0;
	spell_HuntersMark = 0;
	spell_ConcussiveShot = 0;
	spell_AspectOfTheHawk = 0;

	growlSet.clear();
	growlSet.insert(2649);
	growlSet.insert(14916);
	growlSet.insert(14917);
	growlSet.insert(14918);
	growlSet.insert(14919);
	growlSet.insert(14920);
	growlSet.insert(14921);
	growlSet.insert(27047);
	growlSet.insert(61676);
	prowlSet.clear();
	prowlSet.insert(24450);
	prowlSet.insert(24452);
	prowlSet.insert(24453);

	followDistance = 15.0f;
	dpsDistance = 15.0f;
}

bool Nier_Hunter::Prepare()
{
	if (Nier_Base::Prepare())
	{
		bool inGroup = false;
		if (Group* group = me->GetGroup())
		{
			inGroup = true;
		}
		if (Pet* myPet = me->GetPet())
		{
			for (PetSpellMap::iterator itr = myPet->m_spells.begin(); itr != myPet->m_spells.end(); ++itr)
			{
				if (const SpellEntry* si = sSpellTemplate.LookupEntry<SpellEntry>(itr->first))
				{
					if (growlSet.find(itr->first) != growlSet.end())
					{
						myPet->ToggleAutocast(itr->first, !inGroup);
					}
					else if (prowlSet.find(itr->first) != prowlSet.end())
					{
						myPet->ToggleAutocast(itr->first, false);
					}
					else
					{
						myPet->ToggleAutocast(itr->first, true);
					}
				}
			}
		}
		return true;
	}

	return false;
}

void Nier_Hunter::Update(uint32 pDiff)
{
	Nier_Base::Update(pDiff);
}

void Nier_Hunter::Update_Online(uint32 pDiff)
{
	Nier_Base::Update_Online(pDiff);
}

void Nier_Hunter::InitializeCharacter()
{
	target_specialty = 1;
	Nier_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	// 2 handed axe 
	me->learnSpell(197, false);

	uint32 myLevel = me->GetLevel();

	if (myLevel >= 4)
	{
		spell_SerpentSting = 1978;
	}
	if (myLevel >= 6)
	{
		spell_ArcaneShot = 3044;
		spell_HuntersMark = 1130;
	}
	if (myLevel >= 8)
	{
		spell_ConcussiveShot = 5116;
	}
	if (myLevel >= 10)
	{
		spell_SerpentSting = 13549;
		spell_AspectOfTheHawk = 13165;
		// Tame Beast
		me->CastSpell(me, 1579, TriggerCastFlags::TRIGGERED_NONE);
		// Beast Training 
		me->CastSpell(me, 5300, TriggerCastFlags::TRIGGERED_NONE);
		spell_TameBeast = 1515;
		spell_CallPet = 883;
		spell_RevivePet = 982;
		spell_DismissPet = 2641;
	}
	if (myLevel >= 12)
	{
		spell_MendPet = 136;
		spell_ArcaneShot = 14281;
	}
	if (myLevel >= 14)
	{
	}
	if (myLevel >= 16)
	{
	}
	if (myLevel >= 18)
	{
		spell_SerpentSting = 13550;
		spell_AspectOfTheHawk = 14318;
	}
	if (myLevel >= 20)
	{
		spell_MendPet = 3111;
		spell_ArcaneShot = 14282;
	}
	if (myLevel >= 22)
	{
		spell_HuntersMark = 14323;
	}
	if (myLevel >= 24)
	{
	}
	if (myLevel >= 26)
	{
		spell_SerpentSting = 13551;
	}
	if (myLevel >= 26)
	{
	}
	if (myLevel >= 28)
	{
		spell_MendPet = 3661;
		spell_ArcaneShot = 14283;
		spell_AspectOfTheHawk = 14319;
	}
	if (myLevel >= 30)
	{
	}
	if (myLevel >= 32)
	{
	}
	if (myLevel >= 34)
	{
		spell_SerpentSting = 13552;
	}
	if (myLevel >= 36)
	{
		spell_MendPet = 3662;
		spell_ArcaneShot = 14284;
	}
	if (myLevel >= 37)
	{

	}
	if (myLevel >= 38)
	{
		spell_AspectOfTheHawk = 14320;
	}
	if (myLevel >= 40)
	{
		spell_HuntersMark = 14324;
	}
	if (myLevel >= 41)
	{

	}
	if (myLevel >= 42)
	{
		spell_SerpentSting = 13553;
	}
	if (myLevel >= 44)
	{
		spell_MendPet = 13542;
		spell_ArcaneShot = 14285;
	}
	if (myLevel >= 46)
	{
	}
	if (myLevel >= 48)
	{
		spell_AspectOfTheHawk = 14321;
	}
	if (myLevel >= 50)
	{
		spell_SerpentSting = 13554;
	}
	if (myLevel >= 52)
	{
		spell_MendPet = 13543;
		spell_ArcaneShot = 14286;
	}
	if (myLevel >= 54)
	{
	}
	if (myLevel >= 56)
	{
	}
	if (myLevel >= 58)
	{
		spell_SerpentSting = 13555;
		spell_HuntersMark = 14325;
		spell_AspectOfTheHawk = 14322;
	}
	if (myLevel >= 60)
	{
		spell_MendPet = 13544;
		spell_ArcaneShot = 14287;
	}
	if (myLevel >= 61)
	{
		spell_SerpentSting = 25295;
		spell_AspectOfTheHawk = 25296;
	}
	if (myLevel >= 64)
	{
	}
	if (myLevel >= 67)
	{
		spell_SerpentSting = 27016;
	}
	if (myLevel >= 68)
	{
		spell_MendPet = 27046;
		spell_AspectOfTheHawk = 27044;
	}
	if (myLevel >= 69)
	{
		spell_ArcaneShot = 27019;
	}
	if (myLevel >= 70)
	{
	}
	if (myLevel >= 71)
	{
	}
	if (myLevel >= 72)
	{
	}
	if (myLevel >= 73)
	{
		spell_SerpentSting = 49000;
		spell_ArcaneShot = 49044;
	}
	if (myLevel >= 74)
	{
		spell_MendPet = 48989;
	}
	if (myLevel >= 75)
	{
	}
	if (myLevel >= 76)
	{
		spell_HuntersMark = 53338;
	}
	if (myLevel >= 77)
	{

	}
	if (myLevel >= 78)
	{
	}
	if (myLevel >= 79)
	{
		spell_SerpentSting = 49001;
		spell_ArcaneShot = 49045;
	}
	if (myLevel >= 80)
	{
		spell_MendPet = 48990;
	}
	me->UpdateSkillsForLevel(true);
}

bool Nier_Hunter::Tank(Unit* pTarget)
{
	return Nier_Base::Tank(pTarget);
}

bool Nier_Hunter::Heal(Unit* pTarget)
{
	if (Nier_Base::Heal(pTarget))
	{
		if (actionDelay > 0)
		{
			return true;
		}
	}

	return false;
}

bool Nier_Hunter::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (Nier_Base::DPS(pTarget, pTank, pRushing))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (targetDistance < dpsDistance)
		{
			if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
			{
				return true;
			}
			else
			{
				if (CastSpell(pTarget, spell_AutoShot))
				{
					return true;
				}
			}
			if (Pet* myPet = me->GetPet())
			{
				if (myPet->IsAlive())
				{
					if (!myPet->GetVictim())
					{
						if (UnitAI* pai = myPet->AI())
						{
							pai->AttackStart(pTarget);
						}
						if (CharmInfo* ci = myPet->GetCharmInfo())
						{
							if (ci->GetCommandState() != CommandStates::COMMAND_ATTACK)
							{
								ci->SetCommandState(CommandStates::COMMAND_ATTACK);
							}
						}
					}
				}
			}
			if (spell_HuntersMark > 0)
			{
				if (CastSpell(pTarget, spell_HuntersMark, true))
				{
					return true;
				}
			}
			if (spell_SerpentSting > 0)
			{
				if (CastSpell(pTarget, spell_SerpentSting, true, true))
				{
					return true;
				}
			}
			uint32 healthTimes = pTarget->GetMaxHealth() / me->GetMaxHealth();
			if (healthTimes > 2)
			{
				if (spell_ArcaneShot > 0)
				{
					if (CastSpell(pTarget, spell_ArcaneShot))
					{
						return true;
					}
				}
			}
		}
		return true;
	}

	return false;
}

bool Nier_Hunter::PVP(Unit* pTarget)
{
	if (Nier_Base::PVP(pTarget))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (targetDistance < dpsDistance)
		{
			if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
			{
				return true;
			}
			else
			{
				if (CastSpell(pTarget, spell_AutoShot))
				{
					return true;
				}
			}
			if (Pet* myPet = me->GetPet())
			{
				if (myPet->IsAlive())
				{
					if (!myPet->GetVictim())
					{
						if (UnitAI* pai = myPet->AI())
						{
							pai->AttackStart(pTarget);
						}
						if (CharmInfo* ci = myPet->GetCharmInfo())
						{
							if (ci->GetCommandState() != CommandStates::COMMAND_ATTACK)
							{
								ci->SetCommandState(CommandStates::COMMAND_ATTACK);
							}
						}
					}
				}
			}
			if (spell_HuntersMark > 0)
			{
				if (CastSpell(pTarget, spell_HuntersMark, true))
				{
					return true;
				}
			}
			if (spell_SerpentSting > 0)
			{
				if (CastSpell(pTarget, spell_SerpentSting, true, true))
				{
					return true;
				}
			}
			if (spell_ConcussiveShot > 0)
			{
				if (CastSpell(pTarget, spell_ConcussiveShot, true))
				{
					return true;
				}
			}
			if (spell_ArcaneShot > 0)
			{
				if (CastSpell(pTarget, spell_ArcaneShot))
				{
					return true;
				}
			}
		}
		return true;
	}

	return false;
}

bool Nier_Hunter::Buff()
{
	if (spell_CallPet > 0)
	{
		if (Pet* myPet = me->GetPet())
		{
			if (!myPet->IsAlive())
			{
				// revive pet 
				if (CastSpell(me, spell_RevivePet))
				{
					return true;
				}
			}
			else if (myPet->GetHealthPercent() < 50.0f)
			{
				if (CastSpell(me, spell_MendPet))
				{
					return true;
				}
			}
		}
		else
		{
			SpellCastResult scr = me->CastSpell(me, spell_CallPet, TriggerCastFlags::TRIGGERED_NONE);
			if (scr == SpellCastResult::SPELL_CAST_OK)
			{
				return true;
			}
			else if (scr == SpellCastResult::SPELL_FAILED_NO_PET)
			{
				// generate one 
				uint32 targetCreatureEntry = urand(0, sNierManager->tamableBeastMap.size() - 1);
				targetCreatureEntry = sNierManager->tamableBeastMap[targetCreatureEntry];
				if (const CreatureInfo* ci = sObjectMgr.GetCreatureTemplate(targetCreatureEntry))
				{
					Pet* pet = new Pet(HUNTER_PET);
					uint32 guid = me->GetMap()->GenerateLocalLowGuid(HIGHGUID_PET);
					uint32 pet_number = sObjectMgr.GeneratePetNumber();
					CreatureCreatePos pos(me, me->GetOrientation());
					if (pet->Create(guid, pos, ci, pet_number))
					{
						pet->SetLoading(true);
						pet->SetDisplayId(ci->ModelId[0]);
						pet->SetNativeDisplayId(ci->ModelId[0]);
						pet->SetMaxPower(POWER_HAPPINESS, pet->GetCreatePowers(POWER_HAPPINESS));
						pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
						pet->SetPowerType(POWER_FOCUS);
						pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, 0);
						pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
						pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, sObjectMgr.GetXPForPetLevel(me->GetLevel()));
						pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
						pet->SetName(ci->Name);
						pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS, CLASS_WARRIOR);
						pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_GENDER, GENDER_NONE);
						pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_POWER_TYPE, POWER_FOCUS);
						pet->SetSheath(SHEATH_STATE_MELEE);

						pet->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_AURAS);
						pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);

						pet->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_RENAME);

						pet->SetOwnerGuid(me->GetObjectGuid());
						pet->setFaction(me->GetFaction());
						pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, spell_TameBeast);

						if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
						{
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
						}
						if (me->IsPvP())
						{
							pet->SetPvP(true);
						}
						if (me->IsPvPFreeForAll())
						{
							pet->SetPvPFreeForAll(true);
						}
						if (me->IsPvPSanctuary())
						{
							pet->SetPvPSanctuary(true);
						}
						pet->GetCharmInfo()->SetPetNumber(sObjectMgr.GeneratePetNumber(), true);
						pet->SetCanModifyStats(true);
						pet->InitStatsForLevel(me->GetLevel());
						pet->InitLevelupSpellsForLevel();
						pet->InitTalentForLevel();
						pet->SetHealthPercent(me->GetHealthPercent());
						pet->GetMap()->Add((Creature*)pet);
						pet->AIM_Initialize();
						pet->AI()->SetReactState(REACT_DEFENSIVE);
						pet->InitPetCreateSpells();
						pet->LearnPetPassives();
						pet->CastPetAuras(true);
						pet->CastOwnerTalentAuras();
						pet->UpdateAllStats();
						me->SetPet(pet);
						me->PetSpellInitialize();
						pet->SetLoading(false);
						pet->SavePetToDB(PET_SAVE_AS_CURRENT, me);
					}
				}
			}
			else
			{
				// revive pet 
				if (CastSpell(me, spell_RevivePet))
				{
					return true;
				}
			}
		}
	}
	if (spell_AspectOfTheHawk > 0)
	{
		if (CastSpell(me, spell_AspectOfTheHawk, true))
		{
			return true;
		}
	}

	return false;
}

bool Nier_Hunter::Cure()
{
	return Nier_Base::Cure();
}

bool Nier_Hunter::Revive()
{
	return Nier_Base::Revive();
}
