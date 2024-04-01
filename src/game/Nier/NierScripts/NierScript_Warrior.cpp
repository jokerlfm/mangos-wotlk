#include "NierScript_Warrior.h"
#include "../NierManager.h"

NierScript_Warrior::NierScript_Warrior(Player* pMe) :NierScript_Base(pMe)
{
	dpsDistance = CONTACT_DISTANCE;
	followDistance = INTERACTION_DISTANCE;

	spell_SunderArmor = 0;
	spell_SunderArmorAura = 58567;
	spell_BattleShout = 0;
	spell_Taunt = 0;
	spell_ShieldBash = 0;
	spell_DemoralizingShout = 0;
	spell_Revenge = 0;
	spell_ShieldBlock = 0;
	spell_DefensiveStance = 0;
	spell_Rend = 0;
	spell_Disarm = 0;
	spell_LastStand = 0;
	spell_ShieldWall = 0;
}

bool NierScript_Warrior::Prepare()
{
	if (NierScript_Base::Prepare())
	{

	}

	return false;
}

void NierScript_Warrior::Update(uint32 pDiff)
{
	NierScript_Base::Update(pDiff);
}

void NierScript_Warrior::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_Tank;

	uint32 myLevel = me->GetLevel();
	// one-hand swords 
	me->learnSpell(201, true);

	spell_BattleShout = 6673;

	if (myLevel >= 4)
	{
		spell_Rend = 772;
	}
	if (myLevel >= 6)
	{

	}
	if (myLevel >= 8)
	{

	}
	if (myLevel >= 10)
	{
		spell_Taunt = 355;
		spell_DefensiveStance = 71;
		spell_SunderArmor = 7386;
		spell_Rend = 6546;
		// path of defence 
		me->CastSpell(me, 8121, TriggerCastFlags::TRIGGERED_NONE);
	}
	if (myLevel >= 12)
	{
		spell_ShieldBash = 72;
		spell_BattleShout = 5242;
	}
	if (myLevel >= 14)
	{
		spell_Revenge = 6572;
		spell_DemoralizingShout = 1160;
	}
	if (myLevel >= 16)
	{
		spell_ShieldBlock = 2565;
	}
	if (myLevel >= 18)
	{
		spell_Disarm = 676;
	}
	if (myLevel >= 20)
	{
		if (specialty == 2)
		{
			spell_LastStand = 12975;
		}
		spell_Rend = 6547;
	}
	if (myLevel >= 22)
	{
		spell_BattleShout = 6192;
	}
	if (myLevel >= 24)
	{
		spell_DemoralizingShout = 6190;
	}
	if (myLevel >= 28)
	{
		spell_ShieldWall = 871;
	}
	if (myLevel >= 30)
	{
		spell_Rend = 6548;
		// Path of the Berserker
		me->CastSpell(me, 8616, TriggerCastFlags::TRIGGERED_NONE);
	}
	if (myLevel >= 32)
	{
		spell_BattleShout = 11549;
	}
	if (myLevel >= 34)
	{
		spell_DemoralizingShout = 11554;
	}
	if (myLevel >= 38)
	{

	}
	if (myLevel >= 40)
	{
		spell_Rend = 11572;
	}
	if (myLevel >= 42)
	{
		spell_BattleShout = 11550;
	}
	if (myLevel >= 44)
	{
		spell_DemoralizingShout = 11555;
	}
	if (myLevel >= 48)
	{

	}
	if (myLevel >= 50)
	{
		spell_Rend = 11573;
	}
	if (myLevel >= 52)
	{
		spell_BattleShout = 11551;
	}
	if (myLevel >= 54)
	{
		spell_DemoralizingShout = 11556;
	}
	if (myLevel >= 58)
	{

	}
	if (myLevel >= 60)
	{
		spell_Rend = 11574;
		spell_BattleShout = 25289;
	}
	if (myLevel >= 61)
	{

	}
	if (myLevel >= 62)
	{
		spell_DemoralizingShout = 25202;
	}
	if (myLevel >= 65)
	{

	}
	if (myLevel >= 68)
	{
		spell_Rend = 25208;
	}
	if (myLevel >= 69)
	{
		spell_BattleShout = 2048;
	}
	if (myLevel >= 70)
	{
		spell_DemoralizingShout = 25203;
	}
	if (myLevel >= 71)
	{
		spell_Rend = 46845;
	}
	if (myLevel >= 73)
	{

	}
	if (myLevel >= 74)
	{

	}
	if (myLevel >= 75)
	{

	}
	if (myLevel >= 76)
	{
		spell_Rend = 47465;
	}
	if (myLevel >= 77)
	{

	}
	if (myLevel >= 78)
	{
		spell_BattleShout = 47436;
	}
	if (myLevel >= 79)
	{
		spell_DemoralizingShout = 47437;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Warrior::LearnTalents()
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
		// 163 protection 
		LearnTalent(1601);
		LearnTalent(138);
		LearnTalent(147);
		LearnTalent(153);
		LearnTalent(1654);
		LearnTalent(146);
		LearnTalent(140);
		LearnTalent(152);
		LearnTalent(151);
		LearnTalent(702);
		LearnTalent(1652);
		LearnTalent(148);
		LearnTalent(1660);
		LearnTalent(1653);
		LearnTalent(1893);
		LearnTalent(1666);
		LearnTalent(2236);
		LearnTalent(1871);
		LearnTalent(2246);
		LearnTalent(1872);
		break;
	}
	default:
	{
		break;
	}
	}

	me->SendTalentsInfoData(false);

}

void NierScript_Warrior::InitializeEquipments(bool pReset)
{
	NierScript_Base::InitializeEquipments(pReset);

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
	if (requiredLevel >= 40)
	{
		itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE;
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
	inventoryType = InventoryType::INVTYPE_WEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_OFFHAND;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	inventoryType = InventoryType::INVTYPE_SHIELD;
	itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_THROWN;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_THROWN;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

bool NierScript_Warrior::Tank(Unit* pTarget)
{
	if (NierScript_Base::Tank(pTarget))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (me->CanReachWithMeleeAttack(pTarget))
		{
			me->Attack(pTarget, true);
			if (spell_Taunt > 0)
			{
				if (!pTarget->GetSelectionGuid().IsEmpty())
				{
					if (pTarget->GetSelectionGuid() != me->GetObjectGuid())
					{

						if (CastSpell(pTarget, spell_Taunt))
						{
							return true;
						}
					}
				}
			}
			uint32 myRage = me->GetPower(Powers::POWER_RAGE);
			if (spell_BattleShout > 0)
			{
				if (myRage > 100)
				{
					if (!me->HasAura(spell_BattleShout))
					{
						if (CastSpell(me, spell_BattleShout, true))
						{
							return true;
						}
					}
				}
			}

			if (spell_DemoralizingShout > 0)
			{
				if (myRage > 100)
				{
					if (CastSpell(pTarget, spell_DemoralizingShout, true))
					{
						return true;
					}
				}
			}
			if (spell_Disarm > 0)
			{
				if (myRage > 150)
				{
					if (pTarget->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID))
					{
						if (CastSpell(pTarget, spell_Disarm, true))
						{
							return true;
						}
					}
				}
			}
			float healthPercent = me->GetHealthPercent();
			if (spell_LastStand > 0)
			{
				if (healthPercent < 25.0f)
				{
					if (CastSpell(me, spell_LastStand))
					{
						return true;
					}
				}
			}
			if (spell_ShieldWall > 0)
			{
				if (healthPercent < 40.0f)
				{
					if (CastSpell(me, spell_ShieldWall))
					{
						return true;
					}
				}
			}
			if (spell_ShieldBlock > 0)
			{
				if (healthPercent < 70.0f)
				{
					if (CastSpell(me, spell_ShieldBlock))
					{
						return true;
					}
				}
			}
			if (spell_Revenge > 0)
			{
				if (myRage > 50)
				{
					if (CastSpell(pTarget, spell_Revenge))
					{
						return true;
					}
				}
			}
			if (spell_SunderArmor > 0)
			{
				if (myRage > 150)
				{
					bool sunder = true;
					Unit::SpellAuraHolderMap const& auras = pTarget->GetSpellAuraHolderMap();
					for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
					{
						if (SpellAuraHolder* holder = itr->second)
						{
							if (const SpellEntry* se = holder->GetSpellProto())
							{
								if (se->Id == spell_SunderArmorAura)
								{
									if (holder->GetStackAmount() >= 5)
									{
										if (holder->GetAuraDuration() > 5000)
										{
											sunder = false;
										}
									}
									break;
								}
							}
						}
					}
					if (sunder)
					{
						if (CastSpell(pTarget, spell_SunderArmor))
						{
							return true;
						}
					}
				}
			}
			if (spell_Rend > 0)
			{
				if (myRage > 100)
				{
					if (CastSpell(pTarget, spell_Rend, true, true))
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

bool NierScript_Warrior::Heal(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Warrior::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	return NierScript_Base::DPS(pTarget, pTank, pRushing);
}

bool NierScript_Warrior::Interrupt(Unit* pTarget)
{
	if (actionDelay > 0)
	{
		return false;
	}
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}
	else if (!me->CanAttack(pTarget))
	{
		return false;
	}

	if (me->CanReachWithMeleeAttack(pTarget))
	{
		if (spell_ShieldBash > 0)
		{
			if (CastSpell(pTarget, spell_ShieldBash))
			{
				return true;
			}
		}
	}

	return false;
}

bool NierScript_Warrior::Buff()
{
	if (spell_DefensiveStance > 0)
	{
		if (!me->HasAura(spell_DefensiveStance))
		{
			if (CastSpell(me, spell_DefensiveStance, true))
			{
				return true;
			}
		}
	}
	return false;
}

bool NierScript_Warrior::Cure()
{
	return NierScript_Base::Cure();
}

bool NierScript_Warrior::Revive()
{
	return NierScript_Base::Revive();
}
