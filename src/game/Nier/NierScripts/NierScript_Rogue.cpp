#include "NierScript_Rogue.h"
#include "../NierManager.h"

NierScript_Rogue::NierScript_Rogue(Player* pMe) :NierScript_Base(pMe)
{
	spell_Dismantle = 0;
	spell_Eviscerate = 0;
	spell_FanofKnives = 0;
	spell_BladeFlurry = 0;
	spell_Sprint = 0;
	spell_TricksoftheTrade = 0;
	spell_Kick = 0;
	spell_Sap = 0;
	spell_Stealth = 0;
	spell_SliceandDice = 0;
	spell_Evasion = 0;
	spell_KidneyShot = 0;
	spell_CheapShot = 0;
	spell_Vanish = 0;
	spell_Feint = 0;
	spell_SinisterStrike = 0;
	spell_DeadlyThrow = 0;

	spell_AdrenalineRush = 0;
	spell_KillingSpree = 0;
	spell_Cloak_Of_Shadows = 0;

	item_InstantPoison = 0;
	item_SlowPoison = 0;
}

bool NierScript_Rogue::Prepare()
{
	if (NierScript_Base::Prepare())
	{

	}

	return false;
}

void NierScript_Rogue::Update(uint32 pDiff)
{
	NierScript_Base::Update(pDiff);
}

void NierScript_Rogue::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();

	spell_Stealth = 1784;
	spell_SinisterStrike = 1752;
	spell_Eviscerate = 2098;
	// dual wield 
	me->learnSpell(674, false);
	me->learnSpell(1180, false);
	me->learnSpell(15590, false);
	me->learnSpell(2567, false);
	item_SlowPoison = 3775;

	if (myLevel >= 4)
	{

	}
	if (myLevel >= 6)
	{
		spell_SinisterStrike = 1757;
	}
	if (myLevel >= 8)
	{
		spell_Eviscerate = 6760;
		spell_Evasion = 5277;
	}
	if (myLevel >= 10)
	{
		spell_Sprint = 2983;
		spell_Sap = 6770;
		spell_SliceandDice = 5171;
	}
	if (myLevel >= 12)
	{
		spell_Kick = 1766;
	}
	if (myLevel >= 14)
	{
		spell_SinisterStrike = 1758;
	}
	if (myLevel >= 16)
	{
		spell_Eviscerate = 6761;
		spell_Feint = 1966;
	}
	if (myLevel >= 18)
	{

	}
	if (myLevel >= 20)
	{
		item_InstantPoison = 6947;
		spell_Dismantle = 51722;
	}
	if (myLevel >= 22)
	{
		spell_Vanish = 1856;
		spell_SinisterStrike = 1759;
	}
	if (myLevel >= 24)
	{
		spell_Eviscerate = 6762;
	}
	if (myLevel >= 26)
	{

	}
	if (myLevel >= 26)
	{
		spell_CheapShot = 1833;
	}
	if (myLevel >= 28)
	{
		item_InstantPoison = 6949;
		spell_Sap = 2070;
		spell_Feint = 6768;
	}
	if (myLevel >= 30)
	{
		spell_BladeFlurry = 13877;
		spell_KidneyShot = 408;
		spell_SinisterStrike = 1760;
	}
	if (myLevel >= 32)
	{
		spell_Eviscerate = 8623;
	}
	if (myLevel >= 34)
	{
		spell_Sprint = 8696;
	}
	if (myLevel >= 36)
	{
		item_InstantPoison = 6950;
	}
	if (myLevel >= 37)
	{

	}
	if (myLevel >= 38)
	{
		spell_SinisterStrike = 8621;
	}
	if (myLevel >= 40)
	{
		spell_Eviscerate = 8624;
		spell_Feint = 8637;
		spell_AdrenalineRush = 13750;
	}
	if (myLevel >= 41)
	{

	}
	if (myLevel >= 42)
	{
		spell_SliceandDice = 6774;
		spell_Vanish = 1857;
	}
	if (myLevel >= 44)
	{
		item_InstantPoison = 8926;
	}
	if (myLevel >= 46)
	{
		spell_SinisterStrike = 11293;
	}
	if (myLevel >= 48)
	{
		spell_Eviscerate = 11299;
		spell_Sap = 11297;
	}
	if (myLevel >= 50)
	{
		spell_Evasion = 26669;
		spell_KidneyShot = 8643;
	}
	if (myLevel >= 52)
	{
		item_InstantPoison = 8927;
		spell_Feint = 11303;
	}
	if (myLevel >= 54)
	{
		spell_SinisterStrike = 11294;
	}
	if (myLevel >= 56)
	{
		spell_Eviscerate = 11300;
	}
	if (myLevel >= 58)
	{
		spell_Sprint = 11305;
	}
	if (myLevel >= 60)
	{
		item_InstantPoison = 8928;
		spell_Eviscerate = 31016;
		spell_Feint = 25302;
		spell_KillingSpree = 51690;
	}
	if (myLevel >= 62)
	{
		spell_Vanish = 26889;
		spell_SinisterStrike = 26861;
	}
	if (myLevel >= 64)
	{
		spell_Eviscerate = 26865;
		spell_Feint = 27448;
		spell_DeadlyThrow = 26679;
	}
	if (myLevel >= 66)
	{
		spell_Cloak_Of_Shadows = 31224;
	}
	if (myLevel >= 68)
	{
		item_InstantPoison = 21927;
	}
	if (myLevel >= 69)
	{

	}
	if (myLevel >= 70)
	{
		spell_SinisterStrike = 26862;
		spell_DeadlyThrow = 48673;
	}
	if (myLevel >= 71)
	{
		spell_Sap = 51724;
	}
	if (myLevel >= 72)
	{
		spell_Feint = 48658;
	}
	if (myLevel >= 73)
	{
		item_InstantPoison = 43230;
		spell_Eviscerate = 48667;
	}
	if (myLevel >= 74)
	{

	}
	if (myLevel >= 75)
	{
		spell_TricksoftheTrade = 57934;
	}
	if (myLevel >= 76)
	{
		spell_SinisterStrike = 48637;
		spell_DeadlyThrow = 48674;
	}
	if (myLevel >= 77)
	{

	}
	if (myLevel >= 78)
	{
		spell_Feint = 48659;
	}
	if (myLevel >= 79)
	{
		item_InstantPoison = 43231;
		spell_Eviscerate = 48668;
	}
	if (myLevel >= 80)
	{
		spell_FanofKnives = 51723;
		spell_SinisterStrike = 48638;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Rogue::LearnTalents()
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

	me->SendTalentsInfoData(false);

}

void NierScript_Rogue::InitializeEquipments(bool pReset)
{
	NierScript_Base::InitializeEquipments(pReset);

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
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
	inventoryType = InventoryType::INVTYPE_WEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_OFFHAND;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_WEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_DAGGER;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_THROWN;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

uint32 NierScript_Rogue::Chase(Unit* pTarget)
{
	uint32 chaseResult = NierScript_Base::Chase(pTarget);
	if (chaseResult == ChaseResult::ChaseResult_Moving)
	{
		if (spell_Sprint > 0)
		{
			float targetDistance = me->GetDistance(pTarget);
			if (targetDistance < INSPECT_DISTANCE)
			{
				if (CastSpell(me, spell_Sprint))
				{
					return true;
				}
			}
		}
	}

	return chaseResult;
}

bool NierScript_Rogue::Tank(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Rogue::Heal(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Rogue::DPS(Unit* pTarget)
{
	uint32 myEnergy = me->GetPower(Powers::POWER_ENERGY);
	if (pTarget->GetSelectionGuid() == me->GetObjectGuid())
	{
		if (pTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
		{
			me->AttackStop();
			if (spell_Feint > 0)
			{
				if (myEnergy >= 20)
				{
					if (CastSpell(pTarget, spell_Feint))
					{
						return true;
					}
				}
			}
			if (spell_Evasion > 0)
			{
				if (CastSpell(me, spell_Evasion))
				{
					return true;
				}
			}
			return true;
		}
	}
	me->Attack(pTarget, true);
	if (spell_Dismantle > 0)
	{
		if (pTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
		{
			if (pTarget->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID))
			{
				if (CastSpell(pTarget, spell_Dismantle, true))
				{
					return true;
				}
			}
		}
		else
		{
			if (pTarget->hasWeaponForAttack(WeaponAttackType::BASE_ATTACK))
			{
				if (CastSpell(pTarget, spell_Dismantle, true))
				{
					return true;
				}
			}
		}
	}
	uint32 comboPoints = me->GetComboPoints();
	if (myEnergy >= 25)
	{
		if (pTarget->IsNonMeleeSpellCasted(true, false, true))
		{
			if (spell_Kick > 0)
			{
				if (CastSpell(pTarget, spell_Kick))
				{
					return true;
				}
			}
			if (spell_KidneyShot > 0)
			{
				if (comboPoints > 0)
				{
					if (CastSpell(pTarget, spell_KidneyShot))
					{
						return true;
					}
				}
			}
		}
	}
	if (rushing)
	{
		if (spell_BladeFlurry > 0)
		{
			if (myEnergy >= 25)
			{
				if (CastSpell(me, spell_BladeFlurry))
				{
					return true;
				}
			}
		}
		if (spell_KillingSpree > 0)
		{
			if (CastSpell(pTarget, spell_KillingSpree))
			{
				return true;
			}
		}
		if (spell_AdrenalineRush > 0)
		{
			if (CastSpell(me, spell_AdrenalineRush))
			{
				return true;
			}
		}
	}
	if (spell_SliceandDice > 0)
	{
		if (myEnergy >= 25)
		{
			if (!me->HasAura(spell_SliceandDice))
			{
				if (comboPoints > 1)
				{
					if (CastSpell(pTarget, spell_SliceandDice))
					{
						return true;
					}
				}
			}
		}
	}
	if (spell_Eviscerate > 0)
	{
		if (myEnergy > 35)
		{
			if (comboPoints > 0)
			{
				uint32 finishRate = urand(1, 4);
				if (comboPoints > finishRate)
				{
					if (CastSpell(pTarget, spell_Eviscerate))
					{
						return true;
					}
				}
			}
		}
	}
	if (spell_SinisterStrike > 0)
	{
		if (myEnergy >= 45)
		{
			if (CastSpell(pTarget, spell_SinisterStrike))
			{
				return true;
			}
		}
	}

	return true;
}

bool NierScript_Rogue::PVP(Unit* pTarget)
{
	me->Attack(pTarget, true);
	if (pTarget->GetSelectionGuid() == me->GetObjectGuid())
	{
		if (spell_Evasion > 0)
		{
			if (CastSpell(me, spell_Evasion))
			{
				return true;
			}
		}
	}
	uint32 myEnergy = me->GetPower(Powers::POWER_ENERGY);
	if (spell_Dismantle > 0)
	{
		if (pTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
		{
			if (pTarget->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID))
			{
				if (CastSpell(pTarget, spell_Dismantle, true))
				{
					return true;
				}
			}
		}
		else
		{
			if (pTarget->hasWeaponForAttack(WeaponAttackType::BASE_ATTACK))
			{
				if (CastSpell(pTarget, spell_Dismantle, true))
				{
					return true;
				}
			}
		}
	}
	uint32 comboPoints = me->GetComboPoints();
	if (myEnergy >= 25)
	{
		if (pTarget->IsNonMeleeSpellCasted(true, false, true))
		{
			if (spell_Kick > 0)
			{
				if (CastSpell(pTarget, spell_Kick))
				{
					return true;
				}
			}
			if (spell_KidneyShot > 0)
			{
				if (comboPoints > 0)
				{
					if (CastSpell(pTarget, spell_KidneyShot))
					{
						return true;
					}
				}
			}
		}
	}
	if (spell_BladeFlurry > 0)
	{
		if (myEnergy >= 25)
		{
			if (CastSpell(me, spell_BladeFlurry))
			{
				return true;
			}
		}
	}
	if (spell_KillingSpree > 0)
	{
		if (CastSpell(pTarget, spell_KillingSpree))
		{
			return true;
		}
	}
	if (spell_AdrenalineRush > 0)
	{
		if (CastSpell(me, spell_AdrenalineRush))
		{
			return true;
		}
	}
	if (spell_SliceandDice > 0)
	{
		if (myEnergy >= 25)
		{
			if (!me->HasAura(spell_SliceandDice))
			{
				if (comboPoints > 1)
				{
					if (CastSpell(pTarget, spell_SliceandDice))
					{
						return true;
					}
				}
			}
		}
	}
	if (spell_Eviscerate > 0)
	{
		if (myEnergy > 35)
		{
			if (comboPoints > 0)
			{
				uint32 finishRate = urand(1, 4);
				if (comboPoints > finishRate)
				{
					if (CastSpell(pTarget, spell_Eviscerate))
					{
						return true;
					}
				}
			}
		}
	}
	if (spell_SinisterStrike > 0)
	{
		if (myEnergy >= 45)
		{
			if (CastSpell(pTarget, spell_SinisterStrike))
			{
				return true;
			}
		}
	}

	return true;
}

bool NierScript_Rogue::Buff()
{
	if (item_InstantPoison > 0)
	{
		if (!me->HasItemCount(item_InstantPoison, 1))
		{
			me->StoreNewItemInBestSlots(item_InstantPoison, 20);
		}
		if (Item* poison = me->GetItemByEntry(item_InstantPoison))
		{
			if (Item* oWeapon = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
			{
				if (oWeapon->GetEnchantmentId(EnchantmentSlot::TEMP_ENCHANTMENT_SLOT) == 0)
				{
					if (UseItem(poison, oWeapon))
					{
						return true;
					}
				}
			}
		}
	}
	if (item_SlowPoison > 0)
	{
		if (!me->HasItemCount(item_SlowPoison, 1))
		{
			me->StoreNewItemInBestSlots(item_SlowPoison, 20);
		}
		if (Item* poison = me->GetItemByEntry(item_SlowPoison))
		{
			if (Item* mWeapon = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
			{
				if (mWeapon->GetEnchantmentId(EnchantmentSlot::TEMP_ENCHANTMENT_SLOT) == 0)
				{
					if (UseItem(poison, mWeapon))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool NierScript_Rogue::Cure()
{
	return NierScript_Base::Cure();
}

bool NierScript_Rogue::Revive()
{
	return NierScript_Base::Revive();
}
