#include "Nier_Rogue.h"
#include "NierManager.h"

#include "Groups/Group.h"

Nier_Rogue::Nier_Rogue() :Nier_Base()
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

bool Nier_Rogue::Prepare()
{
	if (Nier_Base::Prepare())
	{

	}

	return false;
}

void Nier_Rogue::Update(uint32 pDiff)
{
	Nier_Base::Update(pDiff);
}

void Nier_Rogue::Update_Online(uint32 pDiff)
{
	Nier_Base::Update_Online(pDiff);
}

void Nier_Rogue::InitializeCharacter()
{
	target_specialty = 1;
	Nier_Base::InitializeCharacter();

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

bool Nier_Rogue::Tank(Unit* pTarget)
{
	return Nier_Base::Tank(pTarget);
}

bool Nier_Rogue::Heal(Unit* pTarget)
{
	return Nier_Base::Tank(pTarget);
}

bool Nier_Rogue::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (Nier_Base::DPS(pTarget, pTank, pRushing))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (me->CanReachWithMeleeAttack(pTarget))
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
			if (pRushing)
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
		}
		else
		{
			if (spell_Sprint > 0)
			{
				if (targetDistance < INSPECT_DISTANCE)
				{
					if (CastSpell(me, spell_Sprint))
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

bool Nier_Rogue::PVP(Unit* pTarget)
{
	if (Nier_Base::PVP(pTarget))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (me->CanReachWithMeleeAttack(pTarget))
		{
			uint32 myEnergy = me->GetPower(Powers::POWER_ENERGY);
			me->Attack(pTarget, true);
			if (spell_Evasion > 0)
			{
				if (CastSpell(me, spell_Evasion))
				{
					return true;
				}
			}
			uint32 comboPoints = me->GetComboPoints();
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
			if (myEnergy >= 25)
			{
				if (pTarget->IsNonMeleeSpellCasted(false, false, true))
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
			if (myEnergy >= 25)
			{
				if (spell_SliceandDice > 0)
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
				if (spell_Eviscerate > 0)
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
		}
		else
		{
			if (spell_Sprint > 0)
			{
				if (targetDistance < INSPECT_DISTANCE)
				{
					if (CastSpell(me, spell_Sprint))
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

bool Nier_Rogue::Interrupt(Unit* pTarget)
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
		uint32 myEnergy = me->GetPower(Powers::POWER_ENERGY);
		if (myEnergy >= 25)
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
				uint32 comboPoints = me->GetComboPoints();
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

	return false;
}

bool Nier_Rogue::Buff()
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

bool Nier_Rogue::Cure()
{
	return Nier_Base::Cure();
}

bool Nier_Rogue::Revive()
{
	return Nier_Base::Revive();
}
