#include "Strategy_Solo_Normal.h"

#include "Robot/Scripts/Script_Base.h"
#include "Grids/GridNotifiers.h"
#include "MotionGenerators/MotionMaster.h"
#include "Entities/Pet.h"
#include "Entities/Item.h"
#include "Strategy_Group_Normal.h"
#include "Globals/ObjectMgr.h"

Strategy_Solo_Normal::Strategy_Solo_Normal(RobotAI* pmSourceAI)
{
	sourceAI = pmSourceAI;

	instruction = Solo_Instruction::Solo_Instruction_None;
	deathDuration = 0;
	soloDuration = 0;
	restDelay = 0;
	waitDelay = 0;
	strollDelay = 0;
	confuseDelay = 0;
	interestsDelay = 0;
}

void Strategy_Solo_Normal::Update(uint32 pmDiff)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return;
	}
	soloDuration += pmDiff;
	if (restDelay > 0)
	{
		restDelay -= pmDiff;
		if (restDelay == 0)
		{
			restDelay = -1;
		}
	}
	if (waitDelay > 0)
	{
		waitDelay -= pmDiff;
		if (waitDelay == 0)
		{
			waitDelay = -1;
		}
	}
	if (strollDelay > 0)
	{
		strollDelay -= pmDiff;
		if (strollDelay == 0)
		{
			strollDelay = -1;
		}
	}
	if (confuseDelay > 0)
	{
		confuseDelay -= pmDiff;
		if (confuseDelay == 0)
		{
			confuseDelay = -1;
		}
	}
	if (interestsDelay > 0)
	{
		interestsDelay -= pmDiff;
		if (interestsDelay == 0)
		{
			interestsDelay = -1;
		}
	}
	if (!me->isAlive())
	{
		deathDuration += pmDiff;
		restDelay = 0;
		waitDelay = 0;
		strollDelay = 0;
		confuseDelay = 0;
		if (deathDuration > TimeConstants::MINUTE* TimeConstants::IN_MILLISECONDS)
		{
			sLog.outBasic("Revive solo robot %s", me->GetName());
			sourceAI->Refresh();
			sourceAI->RandomTeleport();
			deathDuration = 0;
			return;
		}
	}
	if (me->IsBeingTeleported())
	{
		return;
	}
	if (!me->isAlive())
	{
		return;
	}
	if (soloDuration > TimeConstants::HOUR* TimeConstants::IN_MILLISECONDS)
	{
		soloDuration = 0;
		sourceAI->Refresh();
		sourceAI->RandomTeleport();
		instruction = Solo_Instruction::Solo_Instruction_None;
	}
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return;
	}
	if (me->isInCombat())
	{
		instruction = Solo_Instruction::Solo_Instruction_Battle;
		restDelay = 0;
		waitDelay = 0;
		strollDelay = 0;
	}
	switch (instruction)
	{
	case Solo_Instruction::Solo_Instruction_None:
	{
		instruction = Solo_Instruction::Solo_Instruction_Wander;
		return;
	}
	case Solo_Instruction::Solo_Instruction_Wander:
	{
		if (Rest())
		{
			return;
		}
		if (Buff(me))
		{
			return;
		}
		if (Battle())
		{
			return;
		}
		if (urand(0, 2) < 2)
		{
			if (Stroll())
			{
				return;
			}
		}
		if (urand(0, 2) < 2)
		{
			if (Confuse())
			{
				return;
			}
		}
		if (Wait())
		{
			return;
		}
		break;
	}
	case Solo_Instruction::Solo_Instruction_Battle:
	{
		if (Rest())
		{
			return;
		}
		if (Heal())
		{
			return;
		}
		if (Battle())
		{
			return;
		}
		instruction = Solo_Instruction::Solo_Instruction_Wander;
		break;
	}
	case Solo_Instruction::Solo_Instruction_Rest:
	{
		if (restDelay < 0)
		{
			restDelay = 0;
			instruction = Solo_Instruction::Solo_Instruction_Wander;
			return;
		}
		break;
	}
	case Solo_Instruction::Solo_Instruction_Wait:
	{
		if (waitDelay <= 0)
		{
			waitDelay = 0;
			instruction = Solo_Instruction::Solo_Instruction_Wander;
			return;
		}
		break;
	}
	case Solo_Instruction::Solo_Instruction_Stroll:
	{
		if (strollDelay <= 0)
		{
			strollDelay = 0;
			instruction = Solo_Instruction::Solo_Instruction_Wander;
			return;
		}
		break;
	}
	case Solo_Instruction::Solo_Instruction_Confuse:
	{
		if (confuseDelay <= 0)
		{
			confuseDelay = 0;
			instruction = Solo_Instruction::Solo_Instruction_Wander;
			return;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

bool Strategy_Solo_Normal::Buff(Unit* pmTarget)
{
	return sourceAI->s_base->Buff(pmTarget);
}

bool Strategy_Solo_Normal::Rest()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (me->isInCombat())
	{
		return false;
	}
	else
	{
		float hpRate = me->GetHealthPercent();
		float mpRate = 100;
		if (me->GetPowerType() == Powers::POWER_MANA)
		{
			mpRate = me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA);
		}
		if (hpRate > 50 && mpRate > 50)
		{
			return false;
		}
		uint32 foodEntry = 0;
		if (me->getLevel() >= 75)
		{
			foodEntry = 35950;
		}
		else if (me->getLevel() >= 65)
		{
			foodEntry = 33449;
		}
		else if (me->getLevel() >= 55)
		{
			foodEntry = 21023;
		}
		else if (me->getLevel() >= 45)
		{
			foodEntry = 8932;
		}
		else if (me->getLevel() >= 35)
		{
			foodEntry = 3927;
		}
		else if (me->getLevel() >= 25)
		{
			foodEntry = 1707;
		}
		else if (me->getLevel() >= 15)
		{
			foodEntry = 422;
		}
		else
		{
			return false;
		}
		uint32 drinkEntry = 0;
		if (me->getLevel() >= 75)
		{
			drinkEntry = 33445;
		}
		else if (me->getLevel() >= 65)
		{
			drinkEntry = 35954;
		}
		else if (me->getLevel() >= 55)
		{
			drinkEntry = 18300;
		}
		else if (me->getLevel() >= 45)
		{
			drinkEntry = 8766;
		}
		else if (me->getLevel() >= 45)
		{
			drinkEntry = 8766;
		}
		else if (me->getLevel() >= 35)
		{
			drinkEntry = 1645;
		}
		else if (me->getLevel() >= 25)
		{
			drinkEntry = 1708;
		}
		else if (me->getLevel() >= 15)
		{
			drinkEntry = 1205;
		}

		if (!me->HasItemCount(foodEntry, 1))
		{
			me->StoreNewItemInBestSlots(foodEntry, 20);
		}
		if (!me->HasItemCount(drinkEntry, 1))
		{
			me->StoreNewItemInBestSlots(drinkEntry, 20);
		}

		me->CombatStop(true);
		me->GetMotionMaster()->Clear();
		me->StopMoving();
		me->SetSelectionGuid(ObjectGuid());

		sourceAI->ClearShapeshift();
		Item* pFood = sourceAI->GetItemInInventory(foodEntry);
		if (pFood && !pFood->IsInTrade())
		{
			if (sourceAI->UseItem(pFood, me))
			{
				instruction = Group_Instruction::Group_Instruction_Rest;
				restDelay = 20 * TimeConstants::IN_MILLISECONDS;
			}
		}
		Item* pDrink = sourceAI->GetItemInInventory(drinkEntry);
		if (pDrink && !pDrink->IsInTrade())
		{
			if (sourceAI->UseItem(pDrink, me))
			{
				instruction = Group_Instruction::Group_Instruction_Rest;
				restDelay = 20 * TimeConstants::IN_MILLISECONDS;
			}
		}
		if (restDelay > 0)
		{
			return true;
		}
	}

	return false;
}

bool Strategy_Solo_Normal::Battle()
{
	bool result = false;
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	if (!result)
	{
		Unit* myTarget = me->GetTarget();
		if (Attack(myTarget))
		{
			result = true;
		}
	}
	if (!result)
	{
		Unit* closestAttacker = NULL;
		float closestDistance = DEFAULT_VISIBILITY_DISTANCE;

		for (Unit::AttackerSet::const_iterator attackerIT = me->getAttackers().begin(); attackerIT != me->getAttackers().end(); attackerIT++)
		{
			if (Unit* pTarget = *attackerIT)
			{
				float distance = me->GetDistance(pTarget);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestAttacker = pTarget;
				}
			}
		}

		if (Attack(closestAttacker))
		{
			result = true;
		}
	}
	if (!result)
	{
		Pet* memberPet = me->GetPet();
		if (memberPet)
		{
			Unit* closestAttacker = NULL;
			float closestDistance = 100;

			for (Unit::AttackerSet::const_iterator attackerIT = memberPet->getAttackers().begin(); attackerIT != memberPet->getAttackers().end(); attackerIT++)
			{
				if (Unit* pTarget = *attackerIT)
				{
					float distance = me->GetDistance(pTarget);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						closestAttacker = pTarget;
					}
				}
			}

			if (Attack(closestAttacker))
			{
				result = true;
			}
		}
	}
	if (!result)
	{
		std::list<Unit*> attackTargets;
		MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck u_check(me, DEFAULT_VISIBILITY_DISTANCE);
		MaNGOS::UnitListSearcher<MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck> searcher(attackTargets, u_check);
		Cell::VisitAllObjects(me, searcher, DEFAULT_VISIBILITY_DISTANCE);

		Unit* nearestAttackableTarget = NULL;
		float nearestDistance = MAX_VISIBILITY_DISTANCE;
		for (std::list<Unit*>::iterator it = attackTargets.begin(); it != attackTargets.end(); it++)
		{
			if ((*it)->HasMaster())
			{
				continue;
			}
			if (!me->CanAttack((*it)))
			{
				continue;
			}
			if (!me->IsWithinLOSInMap((*it)))
			{
				continue;
			}
			if ((*it)->GetTypeId() == TypeID::TYPEID_PLAYER)
			{
				if (Attack((*it)))
				{
					result = true;
					break;
				}
			}
			Creature* targetCreature = (Creature*)(*it);
			if (targetCreature)
			{
				if (!targetCreature->CanWalk())
				{
					continue;
				}
			}
			float checkDistance = me->GetDistance((*it));
			if (checkDistance < nearestDistance)
			{
				nearestDistance = checkDistance;
				nearestAttackableTarget = (*it);
			}
		}
		if (!result)
		{
			if (Attack(nearestAttackableTarget))
			{
				result = true;
			}
		}
	}
	if (result)
	{
		instruction = Solo_Instruction::Solo_Instruction_Battle;
	}
	else
	{
		me->InterruptSpell(CURRENT_AUTOREPEAT_SPELL, false);
	}
	return result;
}

bool Strategy_Solo_Normal::Attack(Unit* pmTarget)
{
	return sourceAI->s_base->Attack(pmTarget);
	//return sourceAI->scriptMap[me->getClass()]->Attack(pmTarget);
}

bool Strategy_Solo_Normal::Heal()
{
	return sourceAI->s_base->HealMe();
}

bool Strategy_Solo_Normal::Wait()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	me->GetMotionMaster()->Clear();
	me->StopMoving();
	waitDelay = 5 * TimeConstants::IN_MILLISECONDS;
	instruction = Solo_Instruction::Solo_Instruction_Wait;

	return true;
}

bool Strategy_Solo_Normal::Stroll()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	float destX = 0;
	float destY = 0;
	float destZ = 0;
	me->GetRandomPoint(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), DEFAULT_VISIBILITY_DISTANCE, destX, destY, destZ);
	me->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
	strollDelay = 5 * TimeConstants::IN_MILLISECONDS;
	instruction = Solo_Instruction::Solo_Instruction_Stroll;
	return true;
}

bool Strategy_Solo_Normal::Confuse()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	me->SetStandState(UNIT_STAND_STATE_STAND);
	me->GetMotionMaster()->MoveConfused();
	confuseDelay = 5 * TimeConstants::IN_MILLISECONDS;
	instruction = Solo_Instruction::Solo_Instruction_Confuse;
	return true;
}
