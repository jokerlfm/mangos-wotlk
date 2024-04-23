#include "NierScript_Mage.h"
#include "../NierManager.h"

#include "Groups/Group.h"

NierScript_Mage::NierScript_Mage(Player* pMe) :NierScript_Base(pMe)
{
	spell_Shoot = 0;
	spell_FireBlast = 0;
	spell_ArcaneIntellect = 0;

	followDistance = 20.0f;
	dpsDistance = 25.0f;
}

bool NierScript_Mage::Prepare()
{
	if (NierScript_Base::Prepare())
	{

	}

	return false;
}

void NierScript_Mage::Update(uint32 pDiff)
{
	NierScript_Base::Update(pDiff);
}

void NierScript_Mage::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);

	spell_Shoot = 5019;
	spell_ArcaneIntellect = 1459;

	if (myLevel >= 6)
	{
		spell_FireBlast = 2136;
	}
	if (myLevel >= 8)
	{

	}
	if (myLevel >= 14)
	{
		spell_FireBlast = 2137;
		spell_ArcaneIntellect = 1460;
	}
	if (myLevel >= 22)
	{
		spell_FireBlast = 2138;
	}
	if (myLevel >= 28)
	{
		spell_ArcaneIntellect = 1461;
	}
	if (myLevel >= 30)
	{
		spell_FireBlast = 8412;
	}
	if (myLevel >= 38)
	{
		spell_FireBlast = 8413;
	}
	if (myLevel >= 42)
	{
		spell_ArcaneIntellect = 10156;
	}
	if (myLevel >= 46)
	{
		spell_FireBlast = 10197;
	}
	if (myLevel >= 54)
	{
		spell_FireBlast = 10199;
	}
	if (myLevel >= 56)
	{
		spell_ArcaneIntellect = 10157;
	}
	if (myLevel >= 61)
	{
		spell_FireBlast = 27078;
	}
	if (myLevel >= 70)
	{
		spell_FireBlast = 27079;
		spell_ArcaneIntellect = 27126;
	}
	if (myLevel >= 74)
	{
		spell_FireBlast = 42872;
	}
	if (myLevel >= 80)
	{
		spell_FireBlast = 42873;
		spell_ArcaneIntellect = 42995;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Mage::LearnTalents()
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
		LearnTalent(37);
		LearnTalent(1649);
		LearnTalent(73, 2);
		LearnTalent(69);
		LearnTalent(73);
		LearnTalent(63);
		LearnTalent(66);
		LearnTalent(38);
		LearnTalent(72);
		LearnTalent(61);
		LearnTalent(1737);
		LearnTalent(68);
		LearnTalent(1738);
		LearnTalent(1740);
		LearnTalent(1853);
		LearnTalent(67, 1);
		LearnTalent(1741);
		LearnTalent(1855);
		LearnTalent(67, 2);
		LearnTalent(1856);
		LearnTalent(1857);
		LearnTalent(1854);
		LearnTalent(67);
		LearnTalent(65);
		LearnTalent(62);
		LearnTalent(741);
		break;
	}
	default:
	{
		break;
	}
	}

	me->SendTalentsInfoData(false);

}

void NierScript_Mage::InitializeEquipments(bool pReset)
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

bool NierScript_Mage::Tank(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Mage::Heal(Unit* pTarget)
{
	return false;
}

bool NierScript_Mage::DPS(Unit* pTarget)
{
	if (spell_FireBlast > 0)
	{
		if (CastSpell(pTarget, spell_FireBlast))
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

	return true;
}

bool NierScript_Mage::PVP(Unit* pTarget)
{
	if (spell_FireBlast > 0)
	{
		if (CastSpell(pTarget, spell_FireBlast))
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

	return true;
}

bool NierScript_Mage::Buff()
{
	if (Group* group = me->GetGroup())
	{
		for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (CastSpell(member, spell_ArcaneIntellect, true))
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (CastSpell(me, spell_ArcaneIntellect, true))
		{
			return true;
		}
	}

	return false;
}

bool NierScript_Mage::Cure()
{
	return NierScript_Base::Cure();
}

bool NierScript_Mage::Revive()
{
	return NierScript_Base::Revive();
}
