#include "Nier_Warrior.h"
#include "NierManager.h"

Nier_Warrior::Nier_Warrior() :Nier_Base()
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

bool Nier_Warrior::Prepare()
{
	if (Nier_Base::Prepare())
	{

	}

	return false;
}

void Nier_Warrior::Update(uint32 pDiff)
{
	Nier_Base::Update(pDiff);
}

void Nier_Warrior::Update_Online(uint32 pDiff)
{
	Nier_Base::Update_Online(pDiff);
}

void Nier_Warrior::InitializeCharacter()
{
	target_specialty = 2;
	Nier_Base::InitializeCharacter();

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
		if (target_specialty == 2)
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

bool Nier_Warrior::Threating(Unit* pTarget)
{
	if (Nier_Base::Threating(pTarget))
	{
		if (pTarget->GetSelectionGuid() != me->GetObjectGuid())
		{
			return true;
		}
		if (spell_SunderArmor > 0)
		{
			Unit::SpellAuraHolderMap const& auras = pTarget->GetSpellAuraHolderMap();
			for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
			{
				if (SpellAuraHolder* holder = itr->second)
				{
					if (const SpellEntry* se = holder->GetSpellProto())
					{
						if (se->Id == spell_SunderArmorAura)
						{
							if (holder->GetStackAmount() < 2)
							{
								return true;
							}
							return false;
						}
					}
				}
			}
			return true;
		}
	}

	return true;
}

bool Nier_Warrior::Tank(Unit* pTarget)
{
	if (Nier_Base::Tank(pTarget))
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

bool Nier_Warrior::Heal(Unit* pTarget)
{
	return Nier_Base::Tank(pTarget);
}

bool Nier_Warrior::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	return Nier_Base::DPS(pTarget, pTank, pRushing);
}

bool Nier_Warrior::Interrupt(Unit* pTarget)
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

bool Nier_Warrior::Buff()
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

bool Nier_Warrior::Cure()
{
	return Nier_Base::Cure();
}

bool Nier_Warrior::Revive()
{
	return Nier_Base::Revive();
}
