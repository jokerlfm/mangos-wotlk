#include "NierScript_Warlock.h"
#include "../NierManager.h"

NierScript_Warlock::NierScript_Warlock(Player* pMe) :NierScript_Base(pMe)
{
	spell_Shoot = 0;
	spell_Corruption = 0;
	spell_Curse_Of_Agony = 0;
	spell_SummonImp = 0;
	spell_Immolate = 0;
	spell_Shadowbolt = 0;
	spell_RainOfFire = 0;
	spell_FireShield = 0;

	followDistance = 20.0f;
	dpsDistance = 25.0f;
}

bool NierScript_Warlock::Prepare()
{
	if (NierScript_Base::Prepare())
	{
		if (Pet* myPet = me->GetPet())
		{
			for (PetSpellMap::iterator itr = myPet->m_spells.begin(); itr != myPet->m_spells.end(); ++itr)
			{
				if (const SpellEntry* si = sSpellTemplate.LookupEntry<SpellEntry>(itr->first))
				{
					// phase shift exception
					if (itr->first == 4511)
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

void NierScript_Warlock::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);

	spell_SummonImp = 688;
	spell_Shoot = 5019;
	spell_Immolate = 348;
	spell_Shadowbolt = 686;

	if (myLevel >= 4)
	{
		spell_Corruption = 172;
	}
	if (myLevel >= 6)
	{
		spell_Shadowbolt = 695;
	}
	if (myLevel >= 8)
	{
		spell_Curse_Of_Agony = 980;
	}
	if (myLevel >= 10)
	{
		spell_Immolate = 707;
	}
	if (myLevel >= 12)
	{
		spell_Shadowbolt = 705;
	}
	if (myLevel >= 14)
	{
		spell_Corruption = 6222;
		spell_FireShield = 2947;
	}
	if (myLevel >= 18)
	{
		spell_Curse_Of_Agony = 1014;
	}
	if (myLevel >= 20)
	{
		spell_Immolate = 1094;
		spell_Shadowbolt = 1088;
		spell_RainOfFire = 5740;
	}
	if (myLevel >= 24)
	{
		spell_Corruption = 6223;
		spell_FireShield = 8316;
	}
	if (myLevel >= 28)
	{
		spell_Curse_Of_Agony = 6217;
		spell_Shadowbolt = 1106;
	}
	if (myLevel >= 30)
	{
		spell_Immolate = 2941;
	}
	if (myLevel >= 34)
	{
		spell_Corruption = 7648;
		spell_RainOfFire = 6219;
		spell_FireShield = 8317;
	}
	if (myLevel >= 36)
	{
		spell_Shadowbolt = 7641;
	}
	if (myLevel >= 38)
	{
		spell_Curse_Of_Agony = 11711;
	}
	if (myLevel >= 40)
	{
		spell_Immolate = 11665;
	}
	if (myLevel >= 44)
	{
		spell_Corruption = 11671;
		spell_Shadowbolt = 11659;
		spell_FireShield = 11770;
	}
	if (myLevel >= 46)
	{
		spell_RainOfFire = 11677;
	}
	if (myLevel >= 48)
	{
		spell_Curse_Of_Agony = 11712;
	}
	if (myLevel >= 50)
	{
		spell_Immolate = 11667;
	}
	if (myLevel >= 52)
	{
		spell_Shadowbolt = 11660;
	}
	if (myLevel >= 54)
	{
		spell_Corruption = 11672;
		spell_FireShield = 11771;
	}
	if (myLevel >= 58)
	{
		spell_Curse_Of_Agony = 11713;
		spell_RainOfFire = 11678;
	}
	if (myLevel >= 60)
	{
		spell_Corruption = 25311;
		spell_Shadowbolt = 11661;
		spell_Immolate = 11668;
	}
	if (myLevel >= 61)
	{
		spell_Shadowbolt = 25307;
		spell_Immolate = 25309;
	}
	if (myLevel >= 64)
	{
		spell_FireShield = 27269;
	}
	if (myLevel >= 65)
	{
		spell_Corruption = 27216;
	}
	if (myLevel >= 67)
	{
		spell_Curse_Of_Agony = 27218;
	}
	if (myLevel >= 69)
	{
		spell_Immolate = 27215;
		spell_Shadowbolt = 27209;
		spell_RainOfFire = 27212;
	}
	if (myLevel >= 71)
	{
		spell_Corruption = 47812;
	}
	if (myLevel >= 72)
	{
		spell_RainOfFire = 47819;
	}
	if (myLevel >= 73)
	{
		spell_Curse_Of_Agony = 47863;
	}
	if (myLevel >= 74)
	{
		spell_Shadowbolt = 47808;
	}
	if (myLevel >= 75)
	{
		spell_Immolate = 47810;
	}
	if (myLevel >= 76)
	{
		spell_FireShield = 47983;
	}
	if (myLevel >= 77)
	{
		spell_Corruption = 47813;
	}
	if (myLevel >= 79)
	{
		spell_Curse_Of_Agony = 47864;
		spell_Shadowbolt = 47809;
		spell_RainOfFire = 47820;
	}
	if (myLevel >= 80)
	{
		spell_Immolate = 47811;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Warlock::LearnTalents()
{
	NierScript_Base::LearnTalents();

	specialty = 2;
	switch (specialty)
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

	me->SendTalentsInfoData(false);

}

void NierScript_Warlock::InitializeEquipments(bool pReset)
{
	NierScript_Base::InitializeEquipments(pReset);

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
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
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_RANGEDRIGHT;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

bool NierScript_Warlock::DPS(Unit* pTarget)
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
	if (spell_Immolate > 0)
	{
		if (CastSpell(pTarget, spell_Immolate, true, true))
		{
			return true;
		}
	}
	if (rushing)
	{
		if (spell_Shadowbolt > 0)
		{
			if (CastSpell(pTarget, spell_Shadowbolt))
			{
				return true;
			}
		}
	}
	if (spell_Shoot > 0)
	{
		if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			return true;
		}
		else
		{
			if (CastSpell(pTarget, spell_Shoot))
			{
				return true;
			}
		}
	}
	//if (spell_Corruption > 0)
	//{
	//	if (CastSpell(pTarget, spell_Corruption, true, true))
	//	{
	//		return true;
	//	}
	//}
	//if (spell_Curse_Of_Agony > 0)
	//{
	//	if (CastSpell(pTarget, spell_Curse_Of_Agony, true, true))
	//	{
	//		return true;
	//	}
	//}

	return true;
}

bool NierScript_Warlock::PVP(Unit* pTarget)
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
	if (spell_Immolate > 0)
	{
		if (CastSpell(pTarget, spell_Immolate, true, true))
		{
			return true;
		}
	}
	if (spell_Shadowbolt > 0)
	{
		if (CastSpell(pTarget, spell_Shadowbolt))
		{
			return true;
		}
	}
	if (spell_Shoot > 0)
	{
		if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			return true;
		}
		else
		{
			if (CastSpell(pTarget, spell_Shoot))
			{
				return true;
			}
		}
	}
	//if (spell_Corruption > 0)
	//{
	//	if (CastSpell(pTarget, spell_Corruption, true, true))
	//	{
	//		return true;
	//	}
	//}
	//if (spell_Curse_Of_Agony > 0)
	//{
	//	if (CastSpell(pTarget, spell_Curse_Of_Agony, true, true))
	//	{
	//		return true;
	//	}
	//}

	return true;
}

bool NierScript_Warlock::Buff()
{
	if (Pet* myPet = me->GetPet())
	{

	}
	else
	{
		if (CastSpell(me, spell_SummonImp))
		{
			return true;
		}
	}

	return false;
}
