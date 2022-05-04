#include "NingerStrategy_Base.h"
#include "NingerConfig.h"
#include "NingerManager.h"

#include "Grids/GridNotifiers.h"

NingerStrategy_Base::NingerStrategy_Base()
{
	me = nullptr;
	initialized = false;

	dpsDelay = sNingerConfig.DPSDelay;
	randomTeleportDelay = 0;

	corpseRunDelay = 0;

	restLimit = 0;
	drinkDelay = 0;

	wanderDuration = 0;
	combatDuration = 0;
	pvpDelay = 0;

	freeze = false;
	cure = true;
	aoe = true;
	petting = true;
	following = true;
	rushing = false;

	combatAngle = 0.0f;
	forceBack = false;

	actionLimit = 0;
	ogActionTarget = ObjectGuid();
	ogTank = ObjectGuid();
	ogHealer = ObjectGuid();
	actionType = ActionType::ActionType_None;

	dpsDistance = MELEE_MAX_DISTANCE;
	dpsDistanceMin = 0.0f;
	followDistance = FOLLOW_MIN_DISTANCE;
	followAngle = M_PI;
}

void NingerStrategy_Base::Report()
{
	if (Group* myGroup = me->GetGroup())
	{
		if (Player* leaderPlayer = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (leaderPlayer->GetObjectGuid() != me->GetObjectGuid())
			{
				me->Whisper("My strategy set to base.", Language::LANG_UNIVERSAL, leaderPlayer->GetObjectGuid());
			}
		}
	}
}

void NingerStrategy_Base::Reset()
{
	corpseRunDelay = 0;

	restLimit = 0;
	drinkDelay = 0;

	wanderDuration = 0;
	combatDuration = 0;
	pvpDelay = 0;

	freeze = false;
	cure = true;
	aoe = true;
	petting = true;
	following = true;
	rushing = false;

	combatAngle = 0.0f;
	forceBack = false;

	actionLimit = 0;
	ogActionTarget = ObjectGuid();
	ogTank = ObjectGuid();
	ogHealer = ObjectGuid();
	actionType = ActionType::ActionType_None;

	dpsDistance = MELEE_MAX_DISTANCE;
	dpsDistanceMin = 0.0f;
	followDistance = FOLLOW_MIN_DISTANCE;
	followAngle = M_PI;

	switch (me->getClass())
	{
	case Classes::CLASS_WARRIOR:
	{
		me->groupRole = GroupRole::GroupRole_Tank;
		break;
	}
	case Classes::CLASS_HUNTER:
	{
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		dpsDistanceMin = RANGE_MIN_DISTANCE;
		break;
	}
	case Classes::CLASS_SHAMAN:
	{
		break;
	}
	case Classes::CLASS_PALADIN:
	{
		break;
	}
	case Classes::CLASS_WARLOCK:
	{
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		break;
	}
	case Classes::CLASS_PRIEST:
	{
		me->groupRole = GroupRole::GroupRole_Healer;
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		break;
	}
	case Classes::CLASS_ROGUE:
	{
		break;
	}
	case Classes::CLASS_MAGE:
	{
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		break;
	}
	case Classes::CLASS_DRUID:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void NingerStrategy_Base::Update(uint32 pmDiff)
{
	if (!me)
	{
		return;
	}
	if (!initialized)
	{
		return;
	}
	if (freeze)
	{
		return;
	}
	me->ningerMovement->Update(pmDiff);
	me->ningerAction->Update(pmDiff);
	if (actionLimit > 0)
	{
		actionLimit -= pmDiff;
		if (actionLimit < 0)
		{
			actionLimit = 0;
		}
		switch (actionType)
		{
		case ActionType_None:
		{
			break;
		}
		case ActionType_Engage:
		{
			if (Unit* actionTarget = ObjectAccessor::GetUnit(*me, ogActionTarget))
			{
				if (Engage(actionTarget))
				{
					return;
				}
			}
			break;
		}
		case ActionType_Revive:
		{
			if (Unit* actionTarget = ObjectAccessor::GetUnit(*me, ogActionTarget))
			{
				if (Revive(actionTarget))
				{
					return;
				}
			}
			break;
		}
		case ActionType::ActionType_Move:
		{
			return;
		}
		default:
			break;
		}
		actionLimit = 0;
		ogActionTarget.Clear();
	}
	if (Group* myGroup = me->GetGroup())
	{
		if (GroupInCombat())
		{
			restLimit = 0;
			combatDuration += pmDiff;
			switch (me->groupRole)
			{
			case GroupRole::GroupRole_DPS:
			{
				if (Cure())
				{
					return;
				}
				if (DPS())
				{
					return;
				}
				break;
			}
			case GroupRole::GroupRole_Healer:
			{
				if (Heal())
				{
					return;
				}
				if (Cure())
				{
					return;
				}
				break;
			}
			case GroupRole::GroupRole_Tank:
			{
				if (Tank())
				{
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
		else
		{
			rushing = false;
			combatAngle = 0.0f;
			combatDuration = 0;
			if (restLimit > 0)
			{
				restLimit -= pmDiff;
				if (drinkDelay >= 0)
				{
					drinkDelay -= pmDiff;
					if (drinkDelay < 0)
					{
						me->ningerAction->Drink();
					}
				}
				return;
			}
			if (Rest())
			{
				return;
			}
			if (Buff())
			{
				return;
			}
			if (Cure())
			{
				return;
			}
			if (Petting())
			{
				return;
			}
		}
		Follow();
	}
	else
	{
		if (!me->IsAlive())
		{
			if (me->GetDeathTimer() > sNingerConfig.ReviveDelay)
			{
				me->ClearInCombat();
				me->ningerAction->ClearTarget();
				me->ningerMovement->ResetMovement();
				float nearX = 0.0f;
				float nearY = 0.0f;
				float nearZ = 0.0f;
				float nearDistance = frand(40.0f, 50.0f);
				float nearAngle = frand(0.0f, M_PI * 2);
				me->GetNearPoint(me, nearX, nearY, nearZ, me->GetCombatReach(), nearDistance, nearAngle);
				me->ResurrectPlayer(0.2f);
				me->TeleportTo(me->GetMapId(), nearX, nearY, nearZ, 0);
				me->SpawnCorpseBones();
			}
			return;
		}
		if (me->IsInCombat())
		{
			restLimit = 0;
			wanderDuration = 0;
			combatDuration += pmDiff;
			if (Heal())
			{
				return;
			}
			if (Cure())
			{
				return;
			}
			if (DPS(false))
			{
				return;
			}
		}
		else
		{
			if (Buff())
			{
				return;
			}
			combatDuration = 0;
			if (randomTeleportDelay > 0)
			{
				randomTeleportDelay -= pmDiff;
				if (randomTeleportDelay <= 0)
				{
					randomTeleportDelay = urand(20 * MINUTE * IN_MILLISECONDS, 40 * MINUTE * IN_MILLISECONDS);
					int myClass = me->getClass();
					if (myClass == Classes::CLASS_HUNTER || myClass == Classes::CLASS_ROGUE)
					{
						me->ClearInCombat();
						me->ningerAction->ClearTarget();
						me->ningerMovement->ResetMovement();
						me->ningerAction->RandomTeleport();
					}
				}
			}
			if (wanderDuration > 0)
			{
				wanderDuration -= pmDiff;
				if (wanderDuration <= 0)
				{
					if (Wander())
					{
						return;
					}
				}
			}
			if (pvpDelay >= 0)
			{
				pvpDelay -= pmDiff;
				if (pvpDelay <= 0)
				{
					pvpDelay = 5000;
					std::list<Player*> targets;
					MaNGOS::AnyPlayerInObjectRangeCheck check(me, VISIBILITY_DISTANCE_NORMAL);
					MaNGOS::PlayerListSearcher<MaNGOS::AnyPlayerInObjectRangeCheck> searcher(targets, check);
					Cell::VisitWorldObjects(me, searcher, VISIBILITY_DISTANCE_NORMAL);
					for (std::list<Player*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
					{
						if (Engage(*iter))
						{
							actionLimit = 30 * IN_MILLISECONDS;
							ogActionTarget = (*iter)->GetObjectGuid();
							actionType = ActionType::ActionType_Engage;
							break;
						}
					}
				}
			}
		}
	}
}

bool NingerStrategy_Base::GroupInCombat()
{
	if (!me)
	{
		return false;
	}
	if (Group* myGroup = me->GetGroup())
	{
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (member->IsInCombat())
				{
					if (me->GetDistance(member) < VISIBILITY_DISTANCE_NORMAL)
					{
						return true;
					}
				}
				else if (Pet* memberPet = member->GetPet())
				{
					if (memberPet->IsInCombat())
					{
						if (me->GetDistance(memberPet) < VISIBILITY_DISTANCE_NORMAL)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::Engage(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (NingerAction_Base* nab = me->ningerAction)
	{
		switch (me->groupRole)
		{
		case GroupRole::GroupRole_Tank:
		{
			if (me->ningerAction->Tank(pmTarget, aoe))
			{
				if (Group* myGroup = me->GetGroup())
				{
					if (myGroup->GetGuidByTargetIcon(7) != pmTarget->GetObjectGuid())
					{
						myGroup->SetTargetIcon(7, me->GetObjectGuid(), pmTarget->GetObjectGuid());
					}
				}
				return true;
			}
		}
		case GroupRole::GroupRole_DPS:
		{
			return me->ningerAction->DPS(pmTarget, aoe, rushing);
		}
		default:
		{
			break;
		}
		}
	}

	return false;
}

bool NingerStrategy_Base::Tank(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (Group* myGroup = me->GetGroup())
	{
		if (me->groupRole == GroupRole::GroupRole_Tank)
		{
			if (me->ningerAction->Tank(pmTarget, aoe))
			{
				if (myGroup->GetGuidByTargetIcon(7) != pmTarget->GetObjectGuid())
				{
					myGroup->SetTargetIcon(7, me->GetObjectGuid(), pmTarget->GetObjectGuid());
				}
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::DPS(bool pmDelay)
{
	if (pmDelay)
	{
		if (combatDuration < dpsDelay)
		{
			return false;
		}
	}

	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		if (ObjectGuid ogTankTarget = myGroup->GetGuidByTargetIcon(7))
		{
			if (Unit* tankTarget = ObjectAccessor::GetUnit(*me, ogTankTarget))
			{
				if (me->ningerAction->DPS(tankTarget, aoe, rushing))
				{
					return true;
				}
			}
		}

		if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (Unit* leaderTarget = leader->GetTarget())
			{
				if (leaderTarget->IsInCombat())
				{
					if (me->ningerAction->DPS(leaderTarget, aoe, rushing))
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		if (Unit* myTarget = me->GetTarget())
		{
			if (me->ningerAction->DPS(myTarget, aoe, rushing))
			{
				return true;
			}
		}
		Unit* nearestAttacker = nullptr;
		float nearestDistance = VISIBILITY_DISTANCE_NORMAL;
		std::set<Unit*> myAttackers = me->getAttackers();
		for (std::set<Unit*>::iterator ait = myAttackers.begin(); ait != myAttackers.end(); ++ait)
		{
			if (Unit* eachAttacker = *ait)
			{
				if (me->CanAttack(eachAttacker))
				{
					float eachDistance = me->GetDistance(eachAttacker);
					if (eachDistance < nearestDistance)
					{
						nearestDistance = eachDistance;
						nearestAttacker = eachAttacker;
					}
				}
			}
		}
		if (nearestAttacker)
		{
			if (me->ningerAction->DPS(nearestAttacker, aoe, rushing))
			{
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::Tank()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		Unit* ogTankTarget = ObjectAccessor::GetUnit(*me, myGroup->GetGuidByTargetIcon(7));
		if (ogTankTarget)
		{
			if (!ogTankTarget->GetSelectionGuid().IsEmpty())
			{
				if (ogTankTarget->GetSelectionGuid() != me->GetObjectGuid())
				{
					if (me->ningerAction->Tank(ogTankTarget, aoe))
					{
						return true;
					}
				}
			}
		}
		Unit* myTarget = me->GetTarget();
		if (myTarget)
		{
			if (!myTarget->GetSelectionGuid().IsEmpty())
			{
				if (myTarget->GetSelectionGuid() != me->GetObjectGuid())
				{
					if (me->ningerAction->Tank(myTarget, aoe))
					{
						myGroup->SetTargetIcon(7, me->GetObjectGuid(), myTarget->GetObjectGuid());
						return true;
					}
				}
			}
		}
		Unit* nearestOTUnit = nullptr;
		float nearestDistance = VISIBILITY_DISTANCE_NORMAL;
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (member->IsAlive())
				{
					if (member->GetObjectGuid() != me->GetObjectGuid())
					{
						float memberDistance = me->GetDistance(member);
						if (memberDistance < VISIBILITY_DISTANCE_NORMAL)
						{
							std::set<Unit*> memberAttackers = member->getAttackers();
							for (std::set<Unit*>::iterator ait = memberAttackers.begin(); ait != memberAttackers.end(); ++ait)
							{
								if (Unit* eachAttacker = *ait)
								{
									if (me->CanAttack(eachAttacker))
									{
										if (!eachAttacker->GetSelectionGuid().IsEmpty())
										{
											if (eachAttacker->GetSelectionGuid() != me->GetObjectGuid())
											{
												float eachDistance = me->GetDistance(eachAttacker);
												if (eachDistance < nearestDistance)
												{
													if (myGroup->GetTargetIconByGuid(eachAttacker->GetObjectGuid()) == -1)
													{
														nearestDistance = eachDistance;
														nearestOTUnit = eachAttacker;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (nearestOTUnit)
		{
			if (me->ningerAction->Tank(nearestOTUnit, aoe))
			{
				myGroup->SetTargetIcon(7, me->GetObjectGuid(), nearestOTUnit->GetObjectGuid());
				return true;
			}
		}
		if (me->ningerAction->Tank(ogTankTarget, aoe))
		{
			return true;
		}
		if (myTarget)
		{
			if (me->ningerAction->Tank(myTarget, aoe))
			{
				myGroup->SetTargetIcon(7, me->GetObjectGuid(), myTarget->GetObjectGuid());
				return true;
			}
		}
		Unit* nearestAttacker = nullptr;
		nearestDistance = VISIBILITY_DISTANCE_NORMAL;
		std::set<Unit*> myAttackers = me->getAttackers();
		for (std::set<Unit*>::iterator ait = myAttackers.begin(); ait != myAttackers.end(); ++ait)
		{
			if (Unit* eachAttacker = *ait)
			{
				if (me->CanAttack(eachAttacker))
				{
					float eachDistance = me->GetDistance(eachAttacker);
					if (eachDistance < nearestDistance)
					{
						if (myGroup->GetTargetIconByGuid(eachAttacker->GetObjectGuid()) == -1)
						{
							if (!eachAttacker->IsImmuneToDamage(SpellSchoolMask::SPELL_SCHOOL_MASK_NORMAL))
							{
								nearestDistance = eachDistance;
								nearestAttacker = eachAttacker;
							}
						}
					}
				}
			}
		}
		if (nearestAttacker)
		{
			if (me->ningerAction->Tank(nearestAttacker, aoe))
			{
				myGroup->SetTargetIcon(7, me->GetObjectGuid(), nearestAttacker->GetObjectGuid());
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::Rest(bool pmForce)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	bool doRest = false;
	if (pmForce)
	{
		doRest = true;
	}
	else
	{
		if (me->GetHealthPercent() < 60.0f)
		{
			doRest = true;
		}
		else
		{
			if (me->GetPowerType() == Powers::POWER_MANA)
			{
				if (me->GetPowerPercent() < 30.0f)
				{
					doRest = true;
				}
			}
		}
	}

	if (doRest)
	{
		if (me->ningerAction->Eat())
		{
			restLimit = 25 * IN_MILLISECONDS;
			drinkDelay = 1 * IN_MILLISECONDS;
			return true;
		}
	}

	return false;
}

bool NingerStrategy_Base::Heal()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		if (me->GetHealthPercent() < 50.0f)
		{
			if (me->ningerAction->Heal(me))
			{
				return true;
			}
		}
		Player* tank = nullptr;
		Player* lowMember = nullptr;
		uint32 lowMemberCount = 0;
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (me->IsWithinDist(member, RANGE_FAR_DISTANCE))
				{
					if (member->GetHealthPercent() < 60.0f)
					{
						lowMember = member;
						lowMemberCount++;
					}
					if (member->groupRole == GroupRole::GroupRole_Tank)
					{
						tank = member;
					}
				}
			}
		}
		if (tank)
		{
			if (tank->GetHealthPercent() < 50.0f)
			{
				if (me->ningerAction->Heal(tank))
				{
					return true;
				}
			}
		}
		if (lowMemberCount > 1)
		{
			if (me->ningerAction->GroupHeal(lowMember))
			{
				return true;
			}
		}
		if (tank)
		{
			if (me->ningerAction->Heal(tank))
			{
				return true;
			}
		}
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (me->ningerAction->SimpleHeal(member))
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (me->ningerAction->Heal(me))
		{
			return true;
		}
	}

	return false;
}

bool NingerStrategy_Base::Revive()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (Group* myGroup = me->GetGroup())
	{
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (!member->IsAlive())
				{
					if (me->ningerAction->Revive(member))
					{
						actionLimit = DEFAULT_ACTION_LIMIT_DELAY;
						ogActionTarget = member->GetObjectGuid();
						actionType = ActionType::ActionType_Revive;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::Revive(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}
	if (Player* targetPlayer = (Player*)pmTarget)
	{
		if (targetPlayer->IsAlive())
		{
			return false;
		}
		if (me->ningerAction->Revive(targetPlayer))
		{
			return true;
		}
	}

	return false;
}

bool NingerStrategy_Base::Buff()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (me->ningerAction->Buff(member))
				{
					return true;
				}
			}
		}
	}
	else
	{
		return me->ningerAction->Buff(me);
	}

	return false;
}

bool NingerStrategy_Base::Petting()
{
	if (me->ningerAction->Petting(petting))
	{
		return true;
	}

	return false;
}

bool NingerStrategy_Base::Cure()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (me->ningerAction->Cure(member))
				{
					return true;
				}
			}
		}
	}
	else
	{
		return me->ningerAction->Cure(me);
	}

	return false;
}

bool NingerStrategy_Base::Follow()
{
	if (!following)
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	if (Group* myGroup = me->GetGroup())
	{
		//if (me->groupRole != GroupRole::GroupRole_Tank)
		//{
		//	if (Player* tank = ObjectAccessor::FindPlayer(ogTank))
		//	{
		//		if (me->ningerMovement->Follow(tank))
		//		{
		//			ogActionTarget = tank->GetObjectGuid();
		//			return true;
		//		}
		//	}
		//}
		if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (me->ningerMovement->Follow(leader))
			{
				ogActionTarget = leader->GetObjectGuid();
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::Wander()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	else if (me->IsMoving())
	{
		return true;
	}

	float angle = frand(0, 2 * M_PI);
	float distance = frand(10.0f, 30.0f);
	Position dest;
	me->GetNearPoint(me, dest.x, dest.y, dest.z, me->GetCombatReach(), distance, angle);
	wanderDuration = urand(20 * IN_MILLISECONDS, 40 * IN_MILLISECONDS);
	me->ningerMovement->Point(dest, wanderDuration);
	return true;
}

std::string NingerStrategy_Base::GetGroupRole()
{
	if (!me)
	{
		return "";
	}
	switch (me->groupRole)
	{
	case GroupRole::GroupRole_DPS:
	{
		return "dps";
	}
	case GroupRole::GroupRole_Tank:
	{
		return "tank";
	}
	case GroupRole::GroupRole_Healer:
	{
		return "healer";
	}
	default:
	{
		break;
	}
	}
	return "dps";
}

void NingerStrategy_Base::SetGroupRole(std::string pmRoleName)
{
	if (!me)
	{
		return;
	}
	if (pmRoleName == "dps")
	{
		me->groupRole = GroupRole::GroupRole_DPS;
	}
	else if (pmRoleName == "tank")
	{
		me->groupRole = GroupRole::GroupRole_Tank;
	}
	else if (pmRoleName == "healer")
	{
		me->groupRole = GroupRole::GroupRole_Healer;
	}
}

NingerStrategy_The_Underbog::NingerStrategy_The_Underbog() :NingerStrategy_Base()
{
	hungarfen = false;
}

void NingerStrategy_The_Underbog::Report()
{
	if (Group* myGroup = me->GetGroup())
	{
		if (Player* leaderPlayer = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (leaderPlayer->GetObjectGuid() != me->GetObjectGuid())
			{
				me->Whisper("My strategy set to the underbog.", Language::LANG_UNIVERSAL, leaderPlayer->GetObjectGuid());
			}
		}
	}
}

bool NingerStrategy_The_Underbog::DPS(bool pmDelay)
{
	if (pmDelay)
	{
		if (combatDuration < dpsDelay)
		{
			return false;
		}
	}

	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (Group* myGroup = me->GetGroup())
	{
		if (ObjectGuid ogTankTarget = myGroup->GetGuidByTargetIcon(7))
		{
			if (Unit* tankTarget = ObjectAccessor::GetUnit(*me, ogTankTarget))
			{
				if (tankTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM && tankTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM_1)
				{
					if (me->ningerAction->DPS(tankTarget, aoe, rushing))
					{
						return true;
					}
				}
			}
		}

		if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (Unit* leaderTarget = leader->GetTarget())
			{
				if (leaderTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM && leaderTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM_1)
				{
					if (leaderTarget->IsInCombat())
					{
						if (me->ningerAction->DPS(leaderTarget, aoe, rushing))
						{
							return true;
						}
					}
				}
			}
		}
	}
	else
	{
		if (Unit* myTarget = me->GetTarget())
		{
			if (myTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM && myTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM_1)
			{
				if (me->ningerAction->DPS(myTarget, aoe, rushing))
				{
					return true;
				}
			}
		}
		Unit* nearestAttacker = nullptr;
		float nearestDistance = VISIBILITY_DISTANCE_NORMAL;
		std::set<Unit*> myAttackers = me->getAttackers();
		for (std::set<Unit*>::iterator ait = myAttackers.begin(); ait != myAttackers.end(); ++ait)
		{
			if (Unit* eachAttacker = *ait)
			{
				if (eachAttacker->GetEntry() != ENTRY_UNDERBOG_MUSHROOM && eachAttacker->GetEntry() != ENTRY_UNDERBOG_MUSHROOM_1)
				{
					if (me->CanAttack(eachAttacker))
					{
						float eachDistance = me->GetDistance(eachAttacker);
						if (eachDistance < nearestDistance)
						{
							nearestDistance = eachDistance;
							nearestAttacker = eachAttacker;
						}
					}
				}
			}
		}
		if (nearestAttacker)
		{
			if (me->ningerAction->DPS(nearestAttacker, aoe, rushing))
			{
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_The_Underbog::Tank(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (pmTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM && pmTarget->GetEntry() != ENTRY_UNDERBOG_MUSHROOM_1)
	{
		if (Group* myGroup = me->GetGroup())
		{
			if (me->groupRole == GroupRole::GroupRole_Tank)
			{
				if (me->ningerAction->Tank(pmTarget, aoe))
				{
					if (myGroup->GetGuidByTargetIcon(7) != pmTarget->GetObjectGuid())
					{
						myGroup->SetTargetIcon(7, me->GetObjectGuid(), pmTarget->GetObjectGuid());
					}
					return true;
				}
			}
		}
	}

	return false;
}
