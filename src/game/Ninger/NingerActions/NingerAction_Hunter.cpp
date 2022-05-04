#include "NingerAction_Hunter.h"
#include "World/World.h"
#include "Grids/GridNotifiers.h"

NingerAction_Hunter::NingerAction_Hunter() :NingerAction_Base()
{
	specialty = 0;

	aspectType = HunterAspectType::HunterAspectType_Hawk;

	spell_ArcaneShot = 0;
	spell_SerpentSting = 0;
	spell_ScorpidSting = 0;
	spell_RapidFire = 0;
	spell_FeignDeath = 0;
	spell_HuntersMark = 0;
	spell_Volley = 0;
	spell_SteadyShot = 0;
	spell_AspectOfTheHawk = 0;
	spell_CallPet = 0;
	spell_DismissPet = 0;
	spell_RevivePet = 0;
	spell_TameBeast = 0;
	spell_MendPet = 0;
	spell_AspectOfTheDragonhawk = 0;
	spell_ConcussiveShot = 0;
	spell_AutoShot = 0;
	spell_Readiness = 0;

	spell_ScatterShot = 0;
	spell_TrueshotAura = 0;
	spell_SilencingShot = 0;
	spell_ChimeraShot = 0;

	ammoEntry = 0;
}

void NingerAction_Hunter::InitializeCharacter(uint32 pmTargetLevel, uint32 pmSpecialtyTabIndex)
{
	if (!me)
	{
		return;
	}
	specialty = pmSpecialtyTabIndex;
	me->ClearInCombat();
	if (me->GetLevel() != pmTargetLevel)
	{
		me->GiveLevel(pmTargetLevel);
		me->LearnDefaultSkills();
		me->learnDefaultSpells();

		ResetTalent();
	}
	uint32 myLevel = me->GetLevel();
	spell_AutoShot = 75;
	me->learnSpell(197, false);
	me->learnSpell(264, false);
	ammoEntry = 2512;
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
		ammoEntry = 2515;
		spell_SerpentSting = 13549;
		spell_AspectOfTheHawk = 13165;
		me->CastSpell(me, 23356, TriggerCastFlags::TRIGGERED_NONE);
		me->CastSpell(me, 23357, TriggerCastFlags::TRIGGERED_NONE);
		spell_CallPet = 883;
		spell_DismissPet = 2641;
		spell_RevivePet = 982;
		spell_TameBeast = 1515;
	}
	if (myLevel >= 12)
	{
		spell_ArcaneShot = 14281;
		spell_MendPet = 136;
	}
	if (myLevel >= 18)
	{
		spell_SerpentSting = 13550;
		spell_AspectOfTheHawk = 14318;
	}
	if (myLevel >= 20)
	{
		spell_ArcaneShot = 14282;
		spell_MendPet = 3111;
		me->learnSpell(200, false);
	}
	if (myLevel >= 22)
	{
		spell_ScorpidSting = 3043;
		spell_HuntersMark = 14323;
	}
	if (myLevel >= 26)
	{
		ammoEntry = 3030;
	}
	if (myLevel >= 26)
	{
		spell_SerpentSting = 13551;
		spell_RapidFire = 3045;
	}
	if (myLevel >= 28)
	{
		spell_ArcaneShot = 14283;
		spell_AspectOfTheHawk = 14319;
		spell_MendPet = 3661;
	}
	if (myLevel >= 30)
	{
		spell_FeignDeath = 5384;
	}
	if (myLevel >= 34)
	{
		spell_SerpentSting = 13552;
	}
	if (myLevel >= 36)
	{
		spell_ArcaneShot = 14284;
		spell_MendPet = 3662;
	}
	if (myLevel >= 37)
	{
		spell_Readiness = 23989;
	}
	if (myLevel >= 38)
	{
		spell_AspectOfTheHawk = 14320;
	}
	if (myLevel >= 40)
	{
		ammoEntry = 11285;
		spell_HuntersMark = 14324;
		spell_Volley = 1510;
	}
	if (myLevel >= 41)
	{
		spell_TrueshotAura = 19506;
	}
	if (myLevel >= 42)
	{
		spell_SerpentSting = 13553;
	}
	if (myLevel >= 44)
	{
		spell_ArcaneShot = 14285;
		spell_MendPet = 13542;
	}
	if (myLevel >= 48)
	{
		spell_AspectOfTheHawk = 14321;
	}
	if (myLevel >= 50)
	{
		spell_SerpentSting = 13554;
		spell_Volley = 14294;
		spell_SteadyShot = 56641;
	}
	if (myLevel >= 52)
	{
		spell_ArcaneShot = 14286;
		spell_MendPet = 13543;
		spell_SilencingShot = 34490;
	}
	if (myLevel >= 55)
	{
		ammoEntry = 28053;
	}
	if (myLevel >= 58)
	{
		spell_SerpentSting = 13555;
		spell_HuntersMark = 14325;
		spell_Volley = 14295;
		spell_AspectOfTheHawk = 14322;
	}
	if (myLevel >= 60)
	{
		spell_ArcaneShot = 14287;
		spell_SerpentSting = 25295;
		spell_AspectOfTheHawk = 25296;
		spell_MendPet = 13544;
		spell_ChimeraShot = 53209;
	}
	if (myLevel >= 62)
	{
		spell_SteadyShot = 34120;
	}
	if (myLevel >= 65)
	{
		ammoEntry = 28056;
	}
	if (myLevel >= 67)
	{
		spell_SerpentSting = 27016;
		spell_Volley = 27022;
	}
	if (myLevel >= 68)
	{
		spell_AspectOfTheHawk = 27044;
		spell_MendPet = 27046;
	}
	if (myLevel >= 69)
	{
		spell_ArcaneShot = 27019;
	}
	if (myLevel >= 71)
	{
		spell_SteadyShot = 49051;
	}
	if (myLevel >= 73)
	{
		spell_ArcaneShot = 49044;
		spell_SerpentSting = 49000;
	}
	if (myLevel >= 74)
	{
		spell_Volley = 58431;
		spell_AspectOfTheDragonhawk = 61846;
		spell_MendPet = 48989;
	}
	if (myLevel >= 75)
	{
		ammoEntry = 41586;
	}
	if (myLevel >= 76)
	{
		spell_HuntersMark = 53338;
	}
	if (myLevel >= 77)
	{
		spell_SteadyShot = 49052;
	}
	if (myLevel >= 79)
	{
		spell_ArcaneShot = 49045;
		spell_SerpentSting = 49001;
	}
	if (myLevel >= 80)
	{
		ammoEntry = 52021;
		spell_Volley = 58434;
		spell_AspectOfTheDragonhawk = 61847;
		spell_MendPet = 48990;
	}
	me->UpdateSkillsForLevel(true);
	std::ostringstream msgStream;
	msgStream << me->GetName() << " initialized";
	sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
}

void NingerAction_Hunter::ResetTalent()
{
	if (!me)
	{
		return;
	}
	me->resetTalents(true);
	me->SendTalentsInfoData(false);

	// talent tab : 361 - Beast Mastery, 363 - Marksmanship
	LearnTalent(2197);
	LearnTalent(1344);
	LearnTalent(1349);
	LearnTalent(1343);
	LearnTalent(1346);
	LearnTalent(1348);
	LearnTalent(1342);
	LearnTalent(1353);
	LearnTalent(1362);
	LearnTalent(1361);
	LearnTalent(1804);
	LearnTalent(1806);
	LearnTalent(1807);
	LearnTalent(1808);
	LearnTalent(2132);
	LearnTalent(2134);
	LearnTalent(2135);
	LearnTalent(2132, 3);
	LearnTalent(2130);
	LearnTalent(2133);
	LearnTalent(1818);

	LearnTalent(1382);
	LearnTalent(1624);
	LearnTalent(1395);
	LearnTalent(2138);

	me->SendTalentsInfoData(false);

	// hunter trainer Nightrunner
	TrainSpells(4138);
}

bool NingerAction_Hunter::InitializeEquipments(bool pmReset)
{
	if (!me)
	{
		return true;
	}
	if (pmReset)
	{
		for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
		{
			if (Item* inventoryItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
			{
				me->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
			}
		}
		for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
		{
			if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
			{
				me->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
			}
		}
	}
	uint32 myLevel = me->GetLevel();
	uint32 minQuality = ItemQualities::ITEM_QUALITY_UNCOMMON;
	if (myLevel < 20)
	{
		minQuality = ItemQualities::ITEM_QUALITY_POOR;
	}
	for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
	{
		if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HEAD)
		{
			if (myLevel < 30)
			{
				continue;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS)
		{
			if (myLevel < 30)
			{
				continue;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_NECK)
		{
			if (myLevel < 30)
			{
				continue;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1 || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
		{
			if (myLevel < 30)
			{
				continue;
			}
		}
		uint32 modType = ItemModType::ITEM_MOD_AGILITY;
		uint32 equipItemClass = 0;
		uint32 equipItemSubClass = 0;
		if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HEAD)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WRISTS)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WAIST)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FEET)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HANDS)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_CHEST)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_LEGS)
		{
			equipItemClass = 4;
			if (myLevel < 40)
			{
				equipItemSubClass = 2;
			}
			else
			{
				equipItemSubClass = 3;
			}
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_BACK)
		{
			equipItemClass = 4;
			equipItemSubClass = 1;
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_NECK)
		{
			equipItemClass = 4;
			equipItemSubClass = 0;
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1)
		{
			equipItemClass = 4;
			equipItemSubClass = 0;
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
		{
			equipItemClass = 4;
			equipItemSubClass = 0;
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
		{
			equipItemClass = 2;
			if (myLevel < 20)
			{
				equipItemSubClass = 1;
			}
			else
			{
				equipItemSubClass = 6;
			}
			modType = -1;
		}
		else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_RANGED)
		{
			equipItemClass = 2;
			equipItemSubClass = 2;
			modType = -1;
		}
		else
		{
			continue;
		}
		if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
		{
			if (const ItemPrototype* checkIT = currentEquip->GetProto())
			{
				if (checkIT->Quality >= minQuality)
				{
					continue;
				}
				else
				{
					me->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
				}
			}
		}
		EquipRandomItem(checkEquipSlot, equipItemClass, equipItemSubClass, minQuality, modType);
	}

	return true;
}

void NingerAction_Hunter::Prepare()
{
	NingerAction_Base::Prepare();
	if (!me)
	{
		return;
	}
	else if (!me->IsAlive())
	{
		return;
	}
	if (!me->HasItemCount(ammoEntry, 1000))
	{
		me->StoreNewItemInBestSlots(ammoEntry, 1000);
	}
	me->SetAmmo(ammoEntry);

	if (Pet* myPet = me->GetPet())
	{
		if (myPet->AI()->GetReactState() != ReactStates::REACT_DEFENSIVE)
		{
			myPet->AI()->SetReactState(ReactStates::REACT_DEFENSIVE);
		}
		for (PetSpellMap::iterator itr = myPet->m_spells.begin(); itr != myPet->m_spells.end(); itr++)
		{
			myPet->ToggleAutocast(itr->first, true);
		}
		myPet->SetPower(Powers::POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
	}
	me->Say("Prepared", Language::LANG_UNIVERSAL);
}

bool NingerAction_Hunter::DPS(Unit* pmTarget, bool pmAOE, bool pmRush)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!pmTarget->IsAlive())
	{
		if (me->GetSelectionGuid() == pmTarget->GetObjectGuid())
		{
			ClearTarget();
		}
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		if (me->GetSelectionGuid() == pmTarget->GetObjectGuid())
		{
			ClearTarget();
		}
		return false;
	}
	else if (pmTarget->IsImmuneToDamage(SpellSchoolMask::SPELL_SCHOOL_MASK_NORMAL))
	{
		if (me->GetSelectionGuid() == pmTarget->GetObjectGuid())
		{
			ClearTarget();
		}
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	me->ningerMovement->Chase(pmTarget);
	PetAttack(pmTarget);
	if (CastSpell(pmTarget, spell_HuntersMark, true))
	{
		return true;
	}
	if (targetDistance > RANGE_MAX_DISTANCE)
	{
		return true;
	}
	bool shooting = false;
	if (Spell* spell = me->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL))
	{
		if (spell->m_spellInfo->Id == spell_AutoShot)
		{
			if (spell->m_targets.getUnitTargetGuid() == pmTarget->GetObjectGuid())
			{
				shooting = true;
			}
			else
			{
				me->InterruptSpell(CURRENT_AUTOREPEAT_SPELL, true);
			}
		}
		else
		{
			me->InterruptSpell(CURRENT_AUTOREPEAT_SPELL, true);
		}
	}
	if (!shooting)
	{
		CastSpell(pmTarget, spell_AutoShot);
	}
	if (pmAOE)
	{
		if (spell_Volley > 0)
		{
			int attackerInRangeCount = 0;
			std::list<Creature*> creatureList;
			MaNGOS::AnyUnitInObjectRangeCheck u_check(pmTarget, INTERACTION_DISTANCE);
			MaNGOS::CreatureListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(creatureList, u_check);
			Cell::VisitAllObjects(pmTarget, searcher, INTERACTION_DISTANCE);
			if (!creatureList.empty())
			{
				for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
				{
					if (Creature* hostileCreature = *itr)
					{
						if (!hostileCreature->IsTemporarySummon() && !hostileCreature->IsPet())
						{
							if (me->CanAttack(hostileCreature))
							{
								attackerInRangeCount++;
							}
						}
					}
				}
			}
			if (attackerInRangeCount > 2)
			{
				if (CastSpell(pmTarget, spell_Volley))
				{
					return true;
				}
			}
		}
	}
	if (pmTarget->hasUnitState(UNIT_STAT_MOVING))
	{
		if (CastSpell(pmTarget, spell_ConcussiveShot, true))
		{
			return true;
		}
	}
	if (CastSpell(pmTarget, spell_SerpentSting, true, true))
	{
		return true;
	}
	if (pmRush)
	{
		if (CastSpell(me, spell_RapidFire, true))
		{
			return true;
		}
		if (CastSpell(pmTarget, spell_ChimeraShot))
		{
			return true;
		}
		if (CastSpell(pmTarget, spell_ArcaneShot))
		{
			return true;
		}
		if (CastSpell(pmTarget, spell_SteadyShot))
		{
			return true;
		}
	}

	return true;
}

bool NingerAction_Hunter::Buff(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	if (me->GetObjectGuid() == pmTarget->GetObjectGuid())
	{
		if (CastSpell(me, spell_TrueshotAura, true))
		{
			return true;
		}
		switch (aspectType)
		{
		case HunterAspectType::HunterAspectType_Hawk:
		{
			if (spell_AspectOfTheDragonhawk > 0)
			{
				if (CastSpell(me, spell_AspectOfTheDragonhawk, true))
				{
					return true;
				}
			}
			else if (spell_AspectOfTheHawk > 0)
			{
				if (CastSpell(me, spell_AspectOfTheHawk, true))
				{
					return true;
				}
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	return false;
}

bool NingerAction_Hunter::Petting(bool pmSummon, bool pmReset)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (spell_CallPet > 0 && spell_DismissPet > 0 && spell_RevivePet > 0 && spell_TameBeast > 0)
	{
		if (pmReset)
		{
			if (Pet* myPet = me->GetPet())
			{
				me->RemovePet(PetSaveMode::PET_SAVE_AS_CURRENT);
				me->SaveToDB();
				return true;
			}
			return false;
		}
		Pet* myPet = me->GetPet();
		if (pmSummon)
		{
			if (!myPet)
			{
				SpellCastResult scr = me->CastSpell(me, spell_CallPet, TriggerCastFlags::TRIGGERED_NONE);
				switch (scr)
				{
				case SPELL_CAST_OK:
				{
					return true;
				}
				case SPELL_FAILED_DONT_REPORT:
				{
					me->CastSpell(me, spell_RevivePet, TriggerCastFlags::TRIGGERED_NONE);
					return true;
				}
				case SPELL_FAILED_NO_PET:
				{
					uint32 beastEntry = 0;
					QueryResult* beastQR = WorldDatabase.Query("SELECT entry FROM creature_template where CreatureType = 1 and CreatureTypeFlags & 1 order by rand()");
					if (beastQR)
					{
						do
						{
							Field* fields = beastQR->Fetch();
							beastEntry = fields[0].GetUInt32();
							if (CreatureInfo const* cInfo = sObjectMgr.GetCreatureTemplate(beastEntry))
							{
								Pet* pet = new Pet(HUNTER_PET);
								CreatureCreatePos pos(me, me->GetOrientation());
								uint32 petGuid = me->GetMap()->GenerateLocalLowGuid(HIGHGUID_PET);
								uint32 pet_number = sObjectMgr.GeneratePetNumber();
								if (!pet->Create(petGuid, pos, cInfo, pet_number))
								{
									return false;
								}
								pet->SetDisplayId(cInfo->ModelId[0]);
								pet->SetNativeDisplayId(cInfo->ModelId[0]);
								pet->SetMaxPower(POWER_HAPPINESS, pet->GetCreatePowers(POWER_HAPPINESS));
								pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
								pet->SetPowerType(POWER_FOCUS);
								pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, 0);
								pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
								pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, sObjectMgr.GetXPForPetLevel(me->GetLevel()));
								pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
								pet->SetName(cInfo->Name);
								pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS, CLASS_WARRIOR);
								pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_GENDER, GENDER_NONE);
								pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_POWER_TYPE, POWER_FOCUS);
								pet->SetSheath(SHEATH_STATE_MELEE);
								pet->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_AURAS);
								pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);
								pet->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_RENAME);
								pet->SetLoading(true);
								pet->SetOwnerGuid(me->GetObjectGuid());
								pet->setFaction(me->GetFaction());
								pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, spell_TameBeast);
								if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
								{
									pet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
								}
								if (me->IsImmuneToNPC())
								{
									pet->SetImmuneToNPC(true);
								}
								if (me->IsImmuneToPlayer())
								{
									pet->SetImmuneToPlayer(true);
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
								pet->SetCanModifyStats(true);
								pet->InitStatsForLevel(me->GetLevel());
								pet->InitLevelupSpellsForLevel();
								pet->InitTalentForLevel();
								pet->SetHealthPercent(100.0f);
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
								return true;
							}
						} while (beastQR->NextRow());
					}
					return true;
				}
				default:
				{
					break;
				}
				}
			}
			else
			{
				if (myPet->IsAlive())
				{
					PetStop();
				}
				else
				{
					me->CastSpell(me, spell_RevivePet, TriggerCastFlags::TRIGGERED_NONE);
					return true;
				}
			}
		}
		else
		{
			if (myPet)
			{
				me->CastSpell(me, spell_DismissPet, TriggerCastFlags::TRIGGERED_NONE);
				return true;
			}
		}
	}

	return false;
}

void NingerAction_Hunter::PetStop()
{
	if (Pet* myPet = me->GetPet())
	{
		myPet->AttackStop();
		myPet->AI()->CombatStop();
		if (CharmInfo* ci = myPet->GetCharmInfo())
		{
			ci->SetCommandState(CommandStates::COMMAND_FOLLOW);
		}
	}
}

void NingerAction_Hunter::PetAttack(Unit* pmTarget)
{
	if (Pet* myPet = me->GetPet())
	{
		myPet->AI()->AttackStart(pmTarget);
		if (CharmInfo* ci = myPet->GetCharmInfo())
		{
			ci->SetCommandState(CommandStates::COMMAND_ATTACK);
		}
	}
}