#include "NierScript_Paladin.h"
#include "../NierManager.h"

NierScript_Paladin::NierScript_Paladin(Player* pMe) :NierScript_Base(pMe)
{

}

bool NierScript_Paladin::Prepare()
{
    if (NierScript_Base::Prepare())
    {

    }

    return false;
}

void NierScript_Paladin::Update(uint32 pDiff)
{
    NierScript_Base::Update(pDiff);
}

void NierScript_Paladin::InitializeCharacter()
{
    NierScript_Base::InitializeCharacter();
}

void NierScript_Paladin::LearnTalents()
{
	NierScript_Base::LearnTalents();

	me->SendTalentsInfoData(false);
}

void NierScript_Paladin::InitializeEquipments(bool pReset)
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
	inventoryType = InventoryType::INVTYPE_2HWEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE2;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

bool NierScript_Paladin::Tank(Unit* pTarget)
{
    return NierScript_Base::Tank(pTarget);
}

bool NierScript_Paladin::Heal(Unit* pTarget)
{
    return NierScript_Base::Tank(pTarget);
}

bool NierScript_Paladin::DPS(Unit* pTarget)
{
    return NierScript_Base::DPS(pTarget);
}

bool NierScript_Paladin::Buff()
{
    return NierScript_Base::Buff();
}

bool NierScript_Paladin::Cure()
{
    return NierScript_Base::Cure();
}

bool NierScript_Paladin::Revive()
{
    return NierScript_Base::Revive();
}
