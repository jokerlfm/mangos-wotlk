#include "Strategy_Group_Normal.h"

#include "Robot/Scripts/Script_Base.h"
#include "Robot/RobotManager.h"
#include "Grids/GridNotifiers.h"
#include "MotionGenerators/MotionMaster.h"
#include "Entities/Pet.h"
#include "Entities/Item.h"
#include "Strategy_Group_Normal.h"
#include "Globals/ObjectMgr.h"

Strategy_Group_Normal::Strategy_Group_Normal(RobotAI* pmSourceAI)
{
	sourceAI = pmSourceAI;
	memberNumber = 0;

	instruction = Group_Instruction::Group_Instruction_None;

	assembleDelay = 0;
	restDelay = 0;

	followDistance = FOLLOW_MIN_DISTANCE;
	if (sourceAI->targetClass == Classes::CLASS_HUNTER || sourceAI->targetClass == Classes::CLASS_MAGE || sourceAI->targetClass == Classes::CLASS_PRIEST || sourceAI->targetClass == Classes::CLASS_WARLOCK)
	{
		followDistance = RANGED_MIN_DISTANCE;
	}
	else if (sourceAI->targetClass == Classes::CLASS_DRUID)
	{
		if (sourceAI->characterTalentTab == 1)
		{
			followDistance = RANGED_MIN_DISTANCE;
		}
	}
	else if (sourceAI->targetClass == Classes::CLASS_SHAMAN)
	{
		if (sourceAI->characterTalentTab == 0 || sourceAI->characterTalentTab == 2)
		{
			followDistance = RANGED_MIN_DISTANCE;
		}
	}
}

void Strategy_Group_Normal::Update(uint32 pmDiff)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return;
	}
	Player* master = ObjectAccessor::FindPlayer(sourceAI->masterGUID);
	if (!master)
	{
		// wait for checking to reset
		return;
	}
	if (!master->IsInWorld())
	{
		// wait for checking to reset
		return;
	}
	if (!me->IsInGroup(master))
	{
		return;
	}
	if (restDelay > 0)
	{
		restDelay -= pmDiff;
		if (restDelay == 0)
		{
			restDelay = -1;
		}
	}
	if (assembleDelay > 0)
	{
		assembleDelay -= pmDiff;
		if (assembleDelay == 0)
		{
			assembleDelay = -1;
		}
		if (assembleDelay < 0)
		{
			if (master) {
				sourceAI->WhisperTo("I have come", Language::LANG_UNIVERSAL, master);
				if (!me->isAlive())
				{
					me->ResurrectPlayer(0.1f);
				}
				instruction = Group_Instruction::Group_Instruction_Wander;
				me->ClearInCombat();
				me->getThreatManager().clearReferences();
				me->GetMotionMaster()->Clear();
				me->StopMoving();
				me->TeleportTo(master->GetMapId(), master->GetPositionX(), master->GetPositionY(), master->GetPositionZ(), master->GetOrientation());
			}
			assembleDelay = 0;
			return;
		}
	}
	if (me->IsBeingTeleported())
	{
		return;
	}
	if (!me->isAlive())
	{
		restDelay = 0;
		me->SetSelectionGuid(ObjectGuid());
		me->GetMotionMaster()->Clear();
		return;
	}
	Group* myGroup = me->GetGroup();
	if (!master)
	{
		sourceAI->ResetStrategy();
		me->Say("I do not have a master. I will reset my strategy", Language::LANG_UNIVERSAL);
		return;
	}
	if (!myGroup)
	{
		sourceAI->ResetStrategy();
		me->Say("I am not in a group. I will reset my strategy", Language::LANG_UNIVERSAL);
		return;
	}
	if (sourceAI->staying)
	{
		return;
	}
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return;
	}
	if (GroupInCombat())
	{
		restDelay = 0;
		instruction = Group_Instruction::Group_Instruction_Battle;
	}
	switch (instruction)
	{
	case Group_Instruction::Group_Instruction_None:
	{
		instruction = Group_Instruction::Group_Instruction_Wander;
		break;
	}
	case Group_Instruction::Group_Instruction_Wander:
	{
		if (Rest())
		{
			return;
		}
		if (Buff())
		{
			return;
		}
		if (Follow())
		{
			return;
		}
		break;
	}
	case Group_Instruction::Group_Instruction_Battle:
	{
		if (Rest())
		{
			return;
		}
		if (Battle())
		{
			return;
		}
		if (!GroupInCombat())
		{
			instruction = Group_Instruction::Group_Instruction_Wander;
		}
		break;
	}
	case Group_Instruction::Group_Instruction_Rest:
	{
		if (restDelay <= 0)
		{
			restDelay = 0;
			instruction = Group_Instruction::Group_Instruction_Wander;
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

bool Strategy_Group_Normal::Rest(bool pmForce)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (GroupInCombat())
	{
		return false;
	}
	else
	{
		if (!pmForce)
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

bool Strategy_Group_Normal::Buff()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	Group* myGroup = me->GetGroup();
	for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
	{
		Player* member = groupRef->getSource();
		if (Buff(member))
		{
			return true;
		}
	}
	return false;
}

bool Strategy_Group_Normal::Buff(Player* pmTarget)
{
	return sourceAI->s_base->Buff(pmTarget);
}

bool Strategy_Group_Normal::Battle()
{
	bool result = false;
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	switch (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()])
	{
	case 0:
	{
		result = DPS();
		break;
	}
	case 1:
	{
		result = Tank();
		break;
	}
	case 2:
	{
		result = Healer();
		break;
	}
	default:
	{
		break;
	}
	}
	if (result)
	{
		instruction = Group_Instruction::Group_Instruction_Battle;
	}
	else
	{
		me->InterruptSpell(CURRENT_AUTOREPEAT_SPELL, false);
	}

	return result;
}

bool Strategy_Group_Normal::DPS()
{
	bool result = false;
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	Player* master = ObjectAccessor::FindPlayer(sourceAI->masterGUID);
	if (!result)
	{
		if (GroupInCombat())
		{
			Group* myGroup = me->GetGroup();
			for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				Player* member = groupRef->getSource();
				if (sRobotManager->playerGroupRoleCache[member->GetGUIDLow()] == 1)
				{
					Unit* tankTarget = member->GetTarget();
					if (DPS(tankTarget))
					{
						result = true;
						break;
					}
				}
			}
		}
	}

	if (!result)
	{
		if (master)
		{
			Unit* masterTarget = master->GetTarget();
			if (DPS(masterTarget))
			{
				result = true;
			}
		}
	}

	if (!result)
	{
		Unit* myTarget = me->GetTarget();
		if (DPS(myTarget))
		{
			result = true;
		}
	}

	if (!result)
	{
		Unit* closestAttacker = NULL;
		float closestDistance = DEFAULT_VISIBILITY_DISTANCE;
		Group* myGroup = me->GetGroup();
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			if (Unit* memberAttacker = member->getAttackerForHelper())
			{
				float distance = me->GetDistance(memberAttacker);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestAttacker = memberAttacker;
				}
			}
		}

		if (DPS(closestAttacker))
		{
			result = true;
		}
	}

	if (!result)
	{
		Unit* closestAttacker = NULL;
		float closestDistance = DEFAULT_VISIBILITY_DISTANCE;
		Group* myGroup = me->GetGroup();
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			Pet* memberPet = member->GetPet();
			if (memberPet)
			{
				if (Unit* memberPetAttacker = memberPet->getAttackerForHelper())
				{
					float distance = me->GetDistance(memberPetAttacker);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						closestAttacker = memberPetAttacker;
					}
				}
			}
		}

		if (DPS(closestAttacker))
		{
			result = true;
		}
	}

	return result;
}

bool Strategy_Group_Normal::DPS(Unit* pmTarget)
{
	return sourceAI->s_base->DPS(pmTarget);
}

bool Strategy_Group_Normal::Tank()
{
	bool result = false;
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	Player* master = ObjectAccessor::FindPlayer(sourceAI->masterGUID);
	Group* myGroup = me->GetGroup();
	// tank OT target first
	for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
	{
		Player* member = groupRef->getSource();
		if (member->getAttackers().size() > 0)
		{
			for (Unit::AttackerSet::const_iterator attackerIT = member->getAttackers().begin(); attackerIT != member->getAttackers().end(); attackerIT++)
			{
				Unit* pTarget = *attackerIT;
				if (!pTarget)
				{
					continue;
				}
				float distance = me->GetDistance(pTarget);
				if (distance < DEFAULT_VISIBILITY_DISTANCE)
				{
					if (pTarget->GetSelectionGuid() != me->GetObjectGuid())
					{
						if (Tank(pTarget))
						{
							result = true;
							break;
						}
					}
				}
			}
			if (result)
			{
				break;
			}
		}
	}

	if (!result)
	{
		if (master)
		{
			Unit* masterTarget = master->GetTarget();
			if (Tank(masterTarget))
			{
				result = true;
			}
		}
	}

	if (!result)
	{
		Unit* myTarget = me->GetTarget();
		if (Tank(myTarget))
		{
			result = true;
		}
	}

	if (!result)
	{
		Unit* closestAttacker = NULL;
		float closestDistance = DEFAULT_VISIBILITY_DISTANCE;
		Group* myGroup = me->GetGroup();
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			if (Unit* memberAttacker = member->getAttackerForHelper())
			{
				float distance = me->GetDistance(memberAttacker);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestAttacker = memberAttacker;
				}
			}
		}

		if (Tank(closestAttacker))
		{
			result = true;
		}
	}

	if (!result)
	{
		Unit* closestAttacker = NULL;
		float closestDistance = DEFAULT_VISIBILITY_DISTANCE;
		Group* myGroup = me->GetGroup();
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			Pet* memberPet = member->GetPet();
			if (memberPet)
			{
				if (Unit* memberPetAttacker = memberPet->getAttackerForHelper())
				{
					float distance = me->GetDistance(memberPetAttacker);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						closestAttacker = memberPetAttacker;
					}
				}
			}
		}

		if (Tank(closestAttacker))
		{
			result = true;
		}
	}

	return result;
}

bool Strategy_Group_Normal::Tank(Unit* pmTarget)
{
	return sourceAI->s_base->Tank(pmTarget);
}

bool Strategy_Group_Normal::Attack(Unit* pmTarget)
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	switch (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()])
	{
	case 0:
	{
		return DPS(pmTarget);
	}
	case 1:
	{
		return Tank(pmTarget);
	}
	default:
	{
		break;
	}
	}

	return false;
}

bool Strategy_Group_Normal::Healer()
{
	bool result = false;
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	//Player* master = master;
	Group* myGroup = me->GetGroup();
	Player* tank = NULL;
	Player* lowestMember = NULL;
	float lowestPCT = 50;
	if (!result)
	{
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			if (!member->isAlive())
			{
				continue;
			}
			if (sRobotManager->playerGroupRoleCache[member->GetGUIDLow()] == 1)
			{
				tank = member;
			}
			else
			{
				float eachPCT = member->GetHealthPercent();
				if (eachPCT < lowestPCT)
				{
					lowestPCT = eachPCT;
					lowestMember = member;
				}
			}
		}
	}
	if (!result)
	{
		if (tank)
		{
			if (tank->GetHealthPercent() < 80)
			{
				if (Healer(tank))
				{
					result = true;
				}
			}
		}
	}
	if (!result)
	{
		if (lowestMember)
		{
			if (Healer(lowestMember))
			{
				result = true;
			}
		}
	}

	if (GroupInCombat())
	{
		result = true;
	}

	return result;
}

bool Strategy_Group_Normal::Healer(Unit* pmTarget)
{
	return sourceAI->s_base->Healer(pmTarget);
}

bool Strategy_Group_Normal::GroupInCombat()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	Group* myGroup = me->GetGroup();
	if (!myGroup)
	{
		return false;
	}
	for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
	{
		Player* member = groupRef->getSource();
		if (me->GetDistance(member) < 100)
		{
			if (member->isAlive())
			{
				if (member->isInCombat())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Strategy_Group_Normal::Follow()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me->isAlive())
	{
		return false;
	}
	Player* master = ObjectAccessor::FindPlayer(sourceAI->masterGUID);
	if (!master)
	{
		return false;
	}
	Player* followTarget = master;
	if (sRobotManager->playerGroupRoleCache[me->GetGUIDLow()] != 1)
	{
		Group* myGroup = me->GetGroup();
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			Player* member = groupRef->getSource();
			if (sRobotManager->playerGroupRoleCache[member->GetGUIDLow()] == 1)
			{
				followTarget = member;
				break;
			}
		}
	}
	if (!followTarget)
	{
		return false;
	}
	float targetDistance = me->GetDistance(followTarget);
	if (targetDistance > 200)
	{
		return false;
	}
	sourceAI->BaseMove(followTarget, followDistance, false);

	return true;
}

bool Strategy_Group_Normal::Follow(float pmFollowDistance)
{
	followDistance = pmFollowDistance;
	return Follow();
}

bool Strategy_Group_Normal::Stay()
{
	Player* me = ObjectAccessor::FindPlayer(sourceAI->characterGUID);
	if (!me)
	{
		return false;
	}
	me->GetMotionMaster()->Clear();
	me->StopMoving();
	me->SetSelectionGuid(ObjectGuid());
	return true;
}
