#include "Script_Warrior.h"

#ifndef WARRIOR_CHARGE_DISTANCE
# define WARRIOR_CHARGE_DISTANCE 10
#endif

#ifndef WARRIOR_RANGE_DISTANCE
# define WARRIOR_RANGE_DISTANCE 25
#endif

Script_Warrior::Script_Warrior(RobotAI* pmSourceAI) :Script_Base(pmSourceAI)
{

}

bool Script_Warrior::HealMe()
{
	return false;
}

bool Script_Warrior::Tank(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance > WARRIOR_CHARGE_DISTANCE&& targetDistance < WARRIOR_RANGE_DISTANCE)
	{
		if (sourceAI->CastSpell(pmTarget, "Charge", WARRIOR_RANGE_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 500)
	{
		if (sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}
	if (rage > 150)
	{
		if (sourceAI->CastSpell(pmTarget, "Thunder Clap", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		if (sourceAI->CastSpell(pmTarget, "Sunder Armor", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	return true;
}

bool Script_Warrior::Healer(Unit* pmTarget)
{
	return false;
}

bool Script_Warrior::DPS(Unit* pmTarget)
{
	switch (sourceAI->characterTalentTab)
	{
	case 0:
	{
		return DPS_Arms(pmTarget);
	}
	case 1:
	{
		return DPS_Fury(pmTarget);
	}
	case 2:
	{
		return Tank(pmTarget);
	}
	default:
		return DPS_Common(pmTarget);
	}
}

bool Script_Warrior::DPS_Arms(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance > WARRIOR_CHARGE_DISTANCE&& targetDistance < WARRIOR_RANGE_DISTANCE)
	{
		if (sourceAI->CastSpell(pmTarget, "Charge", WARRIOR_RANGE_DISTANCE))
		{
			return true;
		}
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		//if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		//{
		//	return true;
		//}
		if (sourceAI->CastSpell(pmTarget, "Rend", MELEE_MAX_DISTANCE, true, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE);
	}

	return true;
}

bool Script_Warrior::DPS_Fury(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(me, "Recklessness", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (pmTarget->IsNonMeleeSpellCasted(false))
		{
			if (sourceAI->CastSpell(pmTarget, "Pummel", MELEE_MAX_DISTANCE))
			{
				return true;
			}
		}
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		if (pmTarget->GetHealthPercent() < 30.0f)
		{
			if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
		}
	}
	if (rage > 250)
	{
		if (sourceAI->CastSpell(pmTarget, "Bloodthirst", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Whirlwind", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Cleave", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	return true;
}

bool Script_Warrior::DPS_Common(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance > WARRIOR_CHARGE_DISTANCE&& targetDistance < WARRIOR_RANGE_DISTANCE)
	{
		if (sourceAI->CastSpell(pmTarget, "Charge", WARRIOR_RANGE_DISTANCE))
		{
			return true;
		}
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		//if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		//{
		//	return true;
		//}
		if (sourceAI->CastSpell(pmTarget, "Rend", MELEE_MAX_DISTANCE, true, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE);
	}

	return true;
}

bool Script_Warrior::Attack(Unit* pmTarget)
{
	switch (sourceAI->characterTalentTab)
	{
	case 0:
	{
		return Attack_Arms(pmTarget);
	}
	case 1:
	{
		return Attack_Fury(pmTarget);
	}
	case 2:
	{
		return Attack_Protection(pmTarget);
	}
	default:
		return Attack_Common(pmTarget);
	}
}

bool Script_Warrior::Attack_Arms(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance > WARRIOR_CHARGE_DISTANCE&& targetDistance < WARRIOR_RANGE_DISTANCE)
	{
		if (sourceAI->CastSpell(pmTarget, "Charge", WARRIOR_RANGE_DISTANCE))
		{
			return true;
		}
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		//if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		//{
		//	return true;
		//}
		if (sourceAI->CastSpell(pmTarget, "Rend", MELEE_MAX_DISTANCE, true, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE);
	}

	return true;
}

bool Script_Warrior::Attack_Fury(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (me->getLevel() >= 20)
		{
			if (sourceAI->CastSpell(me, "Berserker Stance", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
		}
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(me, "Recklessness", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		if (pmTarget->IsNonMeleeSpellCasted(false))
		{
			if (sourceAI->CastSpell(pmTarget, "Pummel", MELEE_MAX_DISTANCE))
			{
				return true;
			}
		}
		if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 250)
	{
		if (sourceAI->CastSpell(pmTarget, "Bloodthirst", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Whirlwind", MELEE_MAX_DISTANCE))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Cleave", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	return true;
}

bool Script_Warrior::Attack_Protection(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance < WARRIOR_CHARGE_DISTANCE)
	{
		if (sourceAI->CastSpell(me, "Bloodrage", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}

	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		//if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		//{
		//	return true;
		//}
		if (sourceAI->CastSpell(pmTarget, "Rend", MELEE_MAX_DISTANCE, true, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Revenge", MELEE_MAX_DISTANCE))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE);
	}

	return true;
}

bool Script_Warrior::Attack_Common(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	else if (!me->CanAttack(pmTarget))
	{
		return false;
	}
	else if (!pmTarget->isAlive())
	{
		return false;
	}
	float targetDistance = me->GetDistance(pmTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	else if (targetDistance > WARRIOR_CHARGE_DISTANCE&& targetDistance < WARRIOR_RANGE_DISTANCE)
	{
		if (sourceAI->CastSpell(pmTarget, "Charge", WARRIOR_RANGE_DISTANCE))
		{
			return true;
		}
	}
	sourceAI->BaseMove(pmTarget);
	uint32 rage = me->GetPower(Powers::POWER_RAGE);
	if (rage > 100)
	{
		if (sourceAI->CastSpell(me, "Battle Shout", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
		//if (sourceAI->CastSpell(pmTarget, "Demoralizing Shout", MELEE_MAX_DISTANCE, true))
		//{
		//	return true;
		//}
		if (sourceAI->CastSpell(pmTarget, "Rend", MELEE_MAX_DISTANCE, true, true))
		{
			return true;
		}
		if (sourceAI->CastSpell(pmTarget, "Hamstring", MELEE_MAX_DISTANCE, true))
		{
			return true;
		}
	}
	if (rage > 200)
	{
		sourceAI->CastSpell(pmTarget, "Heroic Strike", MELEE_MAX_DISTANCE);
	}

	return true;
}

bool Script_Warrior::Buff(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	switch (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()])
	{
	case 1:
	{
		if (me->getLevel() >= 10)
		{
			if (sourceAI->CastSpell(me, "Defensive Stance", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
		}
		else
		{
			if (sourceAI->CastSpell(me, "Battle Stance", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
		}
		break;
	}
	default:
	{
		switch (sourceAI->characterTalentTab)
		{
		case 0:
		{
			if (sourceAI->CastSpell(me, "Battle Stance", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
			break;
		}
		case 1:
		{
			if (me->getLevel() >= 20)
			{
				if (sourceAI->CastSpell(me, "Berserker Stance", MELEE_MAX_DISTANCE, true))
				{
					return true;
				}
			}
			else
			{
				if (sourceAI->CastSpell(me, "Battle Stance", MELEE_MAX_DISTANCE, true))
				{
					return true;
				}
			}
			break;
		}
		case 2:
		{
			if (me->getLevel() >= 10)
			{
				if (sourceAI->CastSpell(me, "Defensive Stance", MELEE_MAX_DISTANCE, true))
				{
					return true;
				}
			}
			else
			{
				if (sourceAI->CastSpell(me, "Battle Stance", MELEE_MAX_DISTANCE, true))
				{
					return true;
				}
			}
			break;
		}
		default:
		{
			if (sourceAI->CastSpell(me, "Battle Stance", MELEE_MAX_DISTANCE, true))
			{
				return true;
			}
			break;
		}
		}
	}
	}

	return false;
}
