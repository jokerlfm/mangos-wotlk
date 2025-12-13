#include "NierScript_Hunter.h"
#include "../NierManager.h"

NierScript_Hunter::NierScript_Hunter(Player* pMe) :NierScript_Base(pMe)
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

	spell_RaptorStrike = 0;
	spell_MongooseBite = 0;
	spell_WingClip = 0;

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

	followDistance = 25.0f;
	dpsDistance = 30.0f;
}

bool NierScript_Hunter::Prepare()
{
	if (NierScript_Base::Prepare())
	{
		uint32 ammoEntry = 2512;
		uint32 myLevel = me->GetLevel();
		if (myLevel >= 10)
		{
			ammoEntry = 2515;
		}
		if (myLevel >= 25)
		{
			ammoEntry = 3030;
		}
		if (myLevel >= 40)
		{
			ammoEntry = 11285;
		}
		if (myLevel >= 55)
		{
			ammoEntry = 28053;
		}
		if (myLevel >= 65)
		{
			ammoEntry = 28056;
		}
		if (myLevel >= 75)
		{
			ammoEntry = 41586;
		}
		if (myLevel >= 80)
		{
			ammoEntry = 52021;
		}
		if (!me->HasItemCount(ammoEntry, 400))
		{
			me->StoreNewItemInBestSlots(ammoEntry, 400);
		}
		me->RemoveAmmo();
		me->SetAmmo(ammoEntry);

		if (Pet* myPet = me->GetPet())
		{
			bool inGroup = false;
			if (Group* group = me->GetGroup())
			{
				inGroup = true;
			}
			for (PetSpellMap::iterator itr = myPet->m_spells.begin(); itr != myPet->m_spells.end(); ++itr)
			{
				if (const SpellEntry* si = sSpellTemplate.LookupEntry<SpellEntry>(itr->first))
				{
					// hunter pet do not cast any spell 
					myPet->ToggleAutocast(itr->first, false);

					//if (growlSet.find(itr->first) != growlSet.end())
					//{
					//	myPet->ToggleAutocast(itr->first, !inGroup);
					//}
					//else if (prowlSet.find(itr->first) != prowlSet.end())
					//{
					//	myPet->ToggleAutocast(itr->first, false);
					//}
					//else
					//{
					//	myPet->ToggleAutocast(itr->first, true);
					//}
				}
			}
		}
		return true;
	}

	return false;
}

void NierScript_Hunter::Update(uint32 pDiff)
{
	NierScript_Base::Update(pDiff);
}

void NierScript_Hunter::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	// 2 handed axe 
	me->learnSpell(197, false);
	// bows 
	me->learnSpell(264, false);

	uint32 myLevel = me->GetLevel();

	spell_RaptorStrike = 2973;

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
		spell_RaptorStrike = 14260;
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
		spell_WingClip = 2974;
	}
	if (myLevel >= 14)
	{
	}
	if (myLevel >= 16)
	{
		spell_RaptorStrike = 14261;
		spell_MongooseBite = 1495;
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
		spell_RaptorStrike = 14262;
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
		spell_MongooseBite = 14269;
	}
	if (myLevel >= 32)
	{
		spell_RaptorStrike = 14263;
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
		spell_RaptorStrike = 14264;
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
		spell_MongooseBite = 14270;
	}
	if (myLevel >= 46)
	{
	}
	if (myLevel >= 48)
	{
		spell_AspectOfTheHawk = 14321;
		spell_RaptorStrike = 14265;
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
		spell_RaptorStrike = 14266;
	}
	if (myLevel >= 58)
	{
		spell_SerpentSting = 13555;
		spell_HuntersMark = 14325;
		spell_AspectOfTheHawk = 14322;
		spell_MongooseBite = 14271;
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
	if (myLevel >= 63)
	{
		spell_RaptorStrike = 27014;
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
		spell_MongooseBite = 36916;
	}
	if (myLevel >= 71)
	{
		spell_RaptorStrike = 48995;
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
		spell_RaptorStrike = 48996;
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
		spell_MongooseBite = 53339;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Hunter::LearnTalents()
{
	NierScript_Base::LearnTalents();

	specialty = 1;
	switch (specialty)
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

	me->SendTalentsInfoData(false);

}

void NierScript_Hunter::InitializeEquipments(bool pReset)
{
	NierScript_Base::InitializeEquipments(pReset);

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
	if (requiredLevel >= 40)
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
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
	inventoryType = InventoryType::INVTYPE_2HWEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_RANGED;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

bool NierScript_Hunter::Tank(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Hunter::Heal(Unit* pTarget)
{
	if (NierScript_Base::Heal(pTarget))
	{
		return true;
	}

	return false;
}

bool NierScript_Hunter::DPS(Unit* pTarget)
{
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
	if (me->CanReachWithMeleeAttack(pTarget))
	{
		me->Attack(pTarget, true);
		if (spell_WingClip > 0)
		{
			if (CastSpell(pTarget, spell_WingClip, true))
			{
				return true;
			}
		}
		if (spell_RaptorStrike > 0)
		{
			if (CastSpell(pTarget, spell_RaptorStrike))
			{
				return true;
			}
		}
		if (spell_MongooseBite > 0)
		{
			if (CastSpell(pTarget, spell_MongooseBite))
			{
				return true;
			}
		}
	}
	else
	{
		if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			if (spell_HuntersMark > 0)
			{
				if (CastSpell(pTarget, spell_HuntersMark, true))
				{
					return true;
				}
			}
			if (rushing)
			{
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
		}
		else
		{
			if (CastSpell(pTarget, spell_AutoShot))
			{
				return true;
			}
		}
	}

	return true;
}

bool NierScript_Hunter::PVP(Unit* pTarget)
{
	float targetDistance = me->GetDistance(pTarget, true, DistanceCalculation::DIST_CALC_NONE);
	targetDistance = sqrtf(targetDistance);
	float maxDistance = pTarget->GetCombatReach() + me->GetCombatReach() + dpsDistance + RANGED_FLOATING;
	if (targetDistance < maxDistance)
	{
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
		if (me->CanReachWithMeleeAttack(pTarget))
		{
			me->Attack(pTarget, true);
			if (spell_WingClip > 0)
			{
				if (CastSpell(pTarget, spell_WingClip, true))
				{
					return true;
				}
			}
			if (spell_RaptorStrike > 0)
			{
				if (CastSpell(pTarget, spell_RaptorStrike))
				{
					return true;
				}
			}
			if (spell_MongooseBite > 0)
			{
				if (CastSpell(pTarget, spell_MongooseBite))
				{
					return true;
				}
			}
		}
		else
		{
			if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
			{
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
			else
			{
				if (CastSpell(pTarget, spell_AutoShot))
				{
					return true;
				}
			}
		}
	}

	return true;
}

bool NierScript_Hunter::Buff()
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
			if (me->GetMap())
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
                            pet->SetOwnerGuid(me->GetObjectGuid());
                            pet->setFaction(me->GetFaction());
                            pet->SetGuidValue(UNIT_FIELD_CREATEDBY, me->GetObjectGuid());
                            pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, spell_TameBeast);
                            pet->SetLevel(me->GetLevel());
                            pet->InitStatsForLevel(me->GetLevel());
                            pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
                            pet->GetCharmInfo()->SetPetNumber(pet->GetObjectGuid().GetEntry(), true);
                            pet->GetMap()->Add((Creature*)pet);
                            pet->AIM_Initialize();
                            pet->InitPetCreateSpells();
                            pet->LearnPetPassives();
                            pet->CastPetAuras(true);
                            pet->CastOwnerTalentAuras();
                            pet->UpdateAllStats();
                            me->SetPet(pet);
                            me->SetPet(pet);
                            me->SetPetGuid(pet->GetObjectGuid());
                            pet->SavePetToDB(PET_SAVE_AS_CURRENT, me);
                            me->PetSpellInitialize();
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

bool NierScript_Hunter::Cure()
{
	return NierScript_Base::Cure();
}

bool NierScript_Hunter::Revive()
{
	return NierScript_Base::Revive();
}
