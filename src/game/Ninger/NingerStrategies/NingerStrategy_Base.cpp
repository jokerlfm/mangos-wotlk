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
	rti = -1;

	basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
	cure = true;
	aoe = true;
	petting = false;
	rushing = false;

	forceBack = false;
	instantOnly = false;

	actionLimit = 0;
	ogActionTarget = ObjectGuid();
	ogTank = ObjectGuid();
	ogHealer = ObjectGuid();
	actionType = ActionType::ActionType_None;

	dpsDistance = MELEE_MIN_DISTANCE;
	dpsDistanceMin = 0.0f;
	followDistance = FOLLOW_MIN_DISTANCE;

	cautionSpellMap.clear();
}

void NingerStrategy_Base::Report()
{
	if (Group* myGroup = me->GetGroup())
	{
		if (Player* leaderPlayer = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (leaderPlayer->GetObjectGuid() != me->GetObjectGuid())
			{
				std::ostringstream reportStream;
				reportStream << "Strategy : " << me->activeStrategyIndex << " - Role : " << me->groupRole << " - Specialty : " << sNingerManager->characterTalentTabNameMap[me->getClass()][me->ningerAction->specialty] << " RTI : " << rti;
				me->Whisper(reportStream.str(), Language::LANG_UNIVERSAL, leaderPlayer->GetObjectGuid());
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

	dpsDelay = sNingerConfig.DPSDelay;

	basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;

	cure = true;
	aoe = true;
	petting = false;
	rushing = false;
	rti = -1;

	forceBack = false;
	instantOnly = false;

	actionLimit = 0;
	ogActionTarget = ObjectGuid();
	ogTank = ObjectGuid();
	ogHealer = ObjectGuid();
	actionType = ActionType::ActionType_None;

	cautionSpellMap.clear();

	dpsDistance = MELEE_MIN_DISTANCE;
	dpsDistanceMin = 0.0f;
	followDistance = FOLLOW_MIN_DISTANCE;

	switch (me->getClass())
	{
	case Classes::CLASS_WARRIOR:
	{
		me->groupRole = GroupRole::GroupRole_Tank;
		break;
	}
	case Classes::CLASS_HUNTER:
	{
		aoe = false;
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_FAR_DISTANCE;
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
		aoe = false;
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		break;
	}
	case Classes::CLASS_PRIEST:
	{
		aoe = false;
		me->groupRole = GroupRole::GroupRole_Healer;
		followDistance = FOLLOW_FAR_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		break;
	}
	case Classes::CLASS_ROGUE:
	{
		break;
	}
	case Classes::CLASS_MAGE:
	{
		aoe = false;
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
		dpsDistanceMin = RANGE_NEAR_DISTANCE;
		break;
	}
	case Classes::CLASS_DRUID:
	{
		aoe = false;
		followDistance = FOLLOW_NORMAL_DISTANCE;
		dpsDistance = RANGE_NORMAL_DISTANCE;
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
	if (!me->IsAlive())
	{
		return;
	}
	if (!initialized)
	{
		return;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_ROAMING_MOVE))
	{
		return;
	}
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
		case ActionType::ActionType_ReadyTank:
		{
			if (Unit* actionTarget = ObjectAccessor::GetUnit(*me, ogActionTarget))
			{
				if (me->ningerAction->ReadyTank(actionTarget))
				{
					return;
				}
			}
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
		if (basicStrategyType == BasicStrategyType::BasicStrategyType_Glue)
		{
			switch (me->groupRole)
			{
			case GroupRole::GroupRole_DPS:
			{
				TryDPS(false, true, false);
				break;
			}
			case GroupRole::GroupRole_Healer:
			{
				TryHeal(true);
				Cure();
				break;
			}
			case GroupRole::GroupRole_Tank:
			{
				break;
			}
			default:
			{
				break;
			}
			}
			if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
			{
				if (leader->IsAlive())
				{
					if (me->GetDistance(leader) > MIN_DISTANCE_GAP)
					{
						me->ningerAction->nm->ResetMovement();
						me->InterruptNonMeleeSpells(false);
						if (me->CanFly())
						{
							me->GetMotionMaster()->MovePoint(0, leader->GetPosition(), ForcedMovement::FORCED_MOVEMENT_FLIGHT);
						}
						else
						{
							me->GetMotionMaster()->MovePoint(0, leader->GetPosition(), ForcedMovement::FORCED_MOVEMENT_RUN);
						}
					}
					return;
				}
				else
				{
					basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
				}
			}
			else
			{
				basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
			}
		}
		if (GroupInCombat())
		{
			combatDuration += pmDiff;
			restLimit = 0;
			if (basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze)
			{
				return;
			}
			uint32 cautionDelay = Caution();
			if (cautionDelay > 0)
			{
				actionType = ActionType::ActionType_Move;
				actionLimit = cautionDelay;
				basicStrategyType = BasicStrategyType::BasicStrategyType_Hold;
				return;
			}
			if (Assist())
			{
				return;
			}
			switch (me->groupRole)
			{
			case GroupRole::GroupRole_DPS:
			{
				if (Cure())
				{
					return;
				}
				if (TryDPS(true, false, true))
				{
					return;
				}
				break;
			}
			case GroupRole::GroupRole_Healer:
			{
				if (TryHeal(false))
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
				if (TryTank())
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
			combatDuration = 0;
			combatAngleINT = 0;
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
			if (basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze)
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
				me->ningerAction->nm->ResetMovement();
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
			if (TryHeal(false))
			{
				return;
			}
			if (Cure())
			{
				return;
			}
			if (TryDPS(false, false, true))
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
						me->ningerAction->nm->ResetMovement();
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
			if (me->ningerAction->Tank(pmTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
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
			return DoDPS(pmTarget, false, true);
		}
		default:
		{
			break;
		}
		}
	}

	return false;
}

bool NingerStrategy_Base::TryTank()
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
					if (me->ningerAction->Tank(ogTankTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
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
					if (me->ningerAction->Tank(myTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
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
												if (eachDistance < RANGE_NEAR_DISTANCE)
												{
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
		}
		if (nearestOTUnit)
		{
			if (me->ningerAction->Tank(nearestOTUnit, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
			{
				myGroup->SetTargetIcon(7, me->GetObjectGuid(), nearestOTUnit->GetObjectGuid());
				return true;
			}
		}
		if (me->ningerAction->Tank(ogTankTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
		{
			return true;
		}
		if (myTarget)
		{
			if (!sNingerManager->IsPolymorphed(myTarget))
			{
				if (me->ningerAction->Tank(myTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
				{
					myGroup->SetTargetIcon(7, me->GetObjectGuid(), myTarget->GetObjectGuid());
					return true;
				}
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
					if (!sNingerManager->IsPolymorphed(eachAttacker))
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
		}
		if (nearestAttacker)
		{
			if (me->ningerAction->Tank(nearestAttacker, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
			{
				myGroup->SetTargetIcon(7, me->GetObjectGuid(), nearestAttacker->GetObjectGuid());
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::DoTank(Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (Group* myGroup = me->GetGroup())
	{
		if (me->groupRole == GroupRole::GroupRole_Tank)
		{
			if (me->ningerAction->Tank(pmTarget, aoe, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
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

bool NingerStrategy_Base::TryDPS(bool pmDelay, bool pmForceInstantOnly, bool pmChasing)
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
				if (DoDPS(tankTarget, pmForceInstantOnly, pmChasing))
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
					if (!sNingerManager->IsPolymorphed(leaderTarget))
					{
						if (DoDPS(leaderTarget, pmForceInstantOnly, pmChasing))
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
			if (!sNingerManager->IsPolymorphed(myTarget))
			{
				if (DoDPS(myTarget, pmForceInstantOnly, pmChasing))
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
				if (me->CanAttack(eachAttacker))
				{
					if (!sNingerManager->IsPolymorphed(eachAttacker))
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
			if (DoDPS(nearestAttacker, pmForceInstantOnly, pmChasing))
			{
				return true;
			}
		}
	}

	return false;
}

bool NingerStrategy_Base::DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	bool instant = pmForceInstantOnly;
	if (!instant)
	{
		instant = instantOnly;
	}

	if (aoe)
	{
		int attackerInRangeCount = 0;
		std::list<Creature*> creatureList;
		MaNGOS::AnyUnitInObjectRangeCheck u_check(pmTarget, INTERACTION_DISTANCE);
		MaNGOS::CreatureListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(creatureList, u_check);
		Cell::VisitAllObjects(pmTarget, searcher, INTERACTION_DISTANCE);
		if (!creatureList.empty())
		{
			for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
			{
				if (Creature* hostileCreature = *itr)
				{
					if (!hostileCreature->IsPet())
					{
						if (me->CanAttack(hostileCreature))
						{
							attackerInRangeCount++;
						}
					}
				}
			}
		}
		if (attackerInRangeCount > 3)
		{
			if (me->ningerAction->AOE(pmTarget, rushing, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold, instant, pmChasing))
			{
				return true;
			}
		}
	}

	if (me->ningerAction->DPS(pmTarget, rushing, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold, instant, pmChasing, forceBack))
	{
		return true;
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
		if (me->GetHealthPercent() < 70.0f)
		{
			doRest = true;
		}
		else
		{
			if (me->GetPowerType() == Powers::POWER_MANA)
			{
				if (me->GetPowerPercent() < 50.0f)
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

bool NingerStrategy_Base::TryHeal(bool pmForceInstantOnly)
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
			if (DoHeal(me, pmForceInstantOnly))
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
				if (me->IsWithinDist(member, RANGE_MAX_DISTANCE))
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
				if (DoHeal(tank, pmForceInstantOnly))
				{
					return true;
				}
			}
		}
		if (lowMemberCount > 1)
		{
			if (me->ningerAction->GroupHeal(lowMember, pmForceInstantOnly))
			{
				return true;
			}
		}
		if (tank)
		{
			if (DoHeal(tank, pmForceInstantOnly))
			{
				return true;
			}
		}
		for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (me->ningerAction->SimpleHeal(member, pmForceInstantOnly))
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (DoHeal(me, pmForceInstantOnly))
		{
			return true;
		}
	}

	return false;
}

bool NingerStrategy_Base::DoHeal(Unit* pmTarget, bool pmForceInstantOnly)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	bool instant = pmForceInstantOnly;
	if (!instant)
	{
		instant = instantOnly;
	}
	if (me->ningerAction->Heal(pmTarget, instant))
	{
		return true;
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
	if (Player* targetPlayer = (Player*)pmTarget)
	{
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

bool NingerStrategy_Base::Assist()
{
	if (!me)
	{
		return false;
	}
	if (me->ningerAction->Assist(rti))
	{
		return true;
	}

	return false;
}

uint32 NingerStrategy_Base::Caution()
{
	uint32 result = 0;

	if (me)
	{
		if (me->IsAlive())
		{
			if (NingerStrategy_Base* ns = me->strategyMap[me->activeStrategyIndex])
			{
				if (ns->cautionSpellMap.size() > 0)
				{
					for (std::unordered_map<std::string, std::unordered_set<uint32>>::iterator nameIT = ns->cautionSpellMap.begin(); nameIT != ns->cautionSpellMap.end(); nameIT++)
					{
						for (std::unordered_set<uint32>::iterator idIT = nameIT->second.begin(); idIT != nameIT->second.end(); idIT++)
						{
							if (me->HasAura(*idIT))
							{
								result = me->ningerAction->Caution();
								if (result > 0)
								{
									return result;
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
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
	if (basicStrategyType == BasicStrategyType::BasicStrategyType_Freeze)
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
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return true;
	}
	if (Group* myGroup = me->GetGroup())
	{
		if (me->groupRole != GroupRole::GroupRole_Tank)
		{
			if (Player* tank = ObjectAccessor::FindPlayer(ogTank))
			{
				if (me->ningerAction->Follow(tank, followDistance, 0.0f, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
				{
					ogActionTarget = tank->GetObjectGuid();
					return true;
				}
			}
		}
		if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
		{
			if (me->ningerAction->Follow(leader, followDistance, 0.0f, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold))
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
	me->ningerAction->nm->Point(dest, wanderDuration);
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

NingerStrategy_The_Black_Morass::NingerStrategy_The_Black_Morass() : NingerStrategy_Base()
{

}

bool NingerStrategy_The_Black_Morass::DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing)
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (aoe)
	{
		int attackerInRangeCount = 0;
		std::list<Creature*> creatureList;
		MaNGOS::AnyUnitInObjectRangeCheck u_check(pmTarget, INTERACTION_DISTANCE);
		MaNGOS::CreatureListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(creatureList, u_check);
		Cell::VisitAllObjects(pmTarget, searcher, INTERACTION_DISTANCE);
		if (!creatureList.empty())
		{
			for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
			{
				if (Creature* hostileCreature = *itr)
				{
					if (!hostileCreature->IsPet())
					{
						if (me->CanAttack(hostileCreature))
						{
							attackerInRangeCount++;
						}
					}
				}
			}
		}
		if (attackerInRangeCount > 2)
		{
			if (me->ningerAction->AOE(pmTarget, rushing, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold, instantOnly, pmChasing))
			{
				return true;
			}
		}
	}

	if (pmTarget->GetEntry() == 20745 || pmTarget->GetEntry() == 17880)
	{
		if (pmTarget->HasAura(38592))
		{
			return false;
		}
	}
	if (me->ningerAction->DPS(pmTarget, rushing, dpsDistance, dpsDistanceMin, basicStrategyType == BasicStrategyType::BasicStrategyType_Hold, instantOnly, pmChasing, forceBack))
	{
		return true;
	}

	return false;
}

NingerStrategy_Magisters_Terrace::NingerStrategy_Magisters_Terrace() : NingerStrategy_Base()
{
	kael = false;
}

void NingerStrategy_Magisters_Terrace::Update(uint32 pmDiff)
{
	if (!me)
	{
		kael = false;
	}
	else if (!me->IsAlive())
	{
		kael = false;
	}
	else if (!me->IsInCombat())
	{
		kael = false;
	}
	else if (kael)
	{
		me->ningerAction->Update(pmDiff);
		if (actionLimit > 0)
		{
			actionLimit -= pmDiff;
			if (actionLimit < 0)
			{
				actionLimit = 0;
			}
			return;
		}
		if (Creature* phoenix = GetClosestCreatureWithEntry(me, 24674, RANGE_MIN_DISTANCE - MAX_DISTANCE_GAP))
		{
			me->ningerAction->nm->ResetMovement();
			me->InterruptNonMeleeSpells(false);
			Position pos;
			me->GetNearPoint(phoenix, pos.x, pos.y, pos.z, phoenix->GetObjectBoundingRadius(), RANGE_MIN_DISTANCE, phoenix->GetAngle(me));
			me->GetMotionMaster()->MovePoint(0, pos, ForcedMovement::FORCED_MOVEMENT_RUN);
			actionType = ActionType::ActionType_Move;
			actionLimit = 2000;
			return;
		}
		if (Creature* flame = GetClosestCreatureWithEntry(me, 24666, RANGE_MIN_DISTANCE - MAX_DISTANCE_GAP))
		{
			me->ningerAction->nm->ResetMovement();
			me->InterruptNonMeleeSpells(false);
			Position pos;
			me->GetNearPoint(flame, pos.x, pos.y, pos.z, flame->GetObjectBoundingRadius(), RANGE_MIN_DISTANCE, flame->GetAngle(me));
			me->GetMotionMaster()->MovePoint(0, pos, ForcedMovement::FORCED_MOVEMENT_RUN);
			actionType = ActionType::ActionType_Move;
			actionLimit = 2000;
			return;
		}
	}
	if (!me->HasAura(44227))
	{
		me->SetCanFly(false);
		me->m_movementInfo.RemoveMovementFlag(MovementFlags::MOVEFLAG_FLYING);
	}
	if (basicStrategyType == BasicStrategyType::BasicStrategyType_Glue)
	{
		switch (me->groupRole)
		{
		case GroupRole::GroupRole_DPS:
		{
			TryDPS(false, true, false);
			break;
		}
		case GroupRole::GroupRole_Healer:
		{
			TryHeal(true);
			Cure();
			break;
		}
		case GroupRole::GroupRole_Tank:
		{
			break;
		}
		default:
		{
			break;
		}
		}
		if (Group* myGroup = me->GetGroup())
		{
			if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
			{
				if (leader->IsAlive())
				{
					if (me->GetDistance(leader) > MIN_DISTANCE_GAP)
					{
						me->ningerAction->nm->ResetMovement();
						me->InterruptNonMeleeSpells(false);
						if (me->HasAura(44227))
						{
							me->SetCanFly(true);
							me->m_movementInfo.AddMovementFlag(MovementFlags::MOVEFLAG_FLYING);
							me->GetMotionMaster()->MovePoint(0, leader->GetPosition(), ForcedMovement::FORCED_MOVEMENT_FLIGHT, me->GetSpeed(UnitMoveType::MOVE_RUN) * 3.0f, false);
						}
						else
						{
							me->GetMotionMaster()->MovePoint(0, leader->GetPosition(), ForcedMovement::FORCED_MOVEMENT_RUN);
						}
					}
					return;
				}
				else
				{
					basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
				}
			}
			else
			{
				basicStrategyType = BasicStrategyType::BasicStrategyType_Normal;
			}
		}
	}
	NingerStrategy_Base::Update(pmDiff);
}

bool NingerStrategy_Magisters_Terrace::TryTank()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}

	if (kael)
	{
		if (Creature* egg = GetClosestCreatureWithEntry(me, 24675, VISIBILITY_DISTANCE_NORMAL))
		{
			if (DoTank(egg))
			{
				return true;
			}
		}
		Creature* boss = GetClosestCreatureWithEntry(me, 24664, VISIBILITY_DISTANCE_NORMAL);
		if (!boss)
		{
			boss = GetClosestCreatureWithEntry(me, 24857, VISIBILITY_DISTANCE_NORMAL);
		}
		if (DoTank(boss))
		{
			return true;
		}
	}

	return NingerStrategy_Base::TryTank();
}

bool NingerStrategy_Magisters_Terrace::DoTank(Unit* pmTarget)
{
	if (!pmTarget)
	{
		return false;
	}
	else if (!pmTarget->IsAlive())
	{
		return false;
	}
	if (pmTarget->GetEntry() == 24664 || pmTarget->GetEntry() == 24857)
	{
		kael = true;
	}
	return NingerStrategy_Base::DoTank(pmTarget);
}

bool NingerStrategy_Magisters_Terrace::TryDPS(bool pmDelay, bool pmForceInstantOnly, bool pmChasing)
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

	if (kael)
	{
		if (Creature* phoenix = GetClosestCreatureWithEntry(me, 24674, VISIBILITY_DISTANCE_NORMAL))
		{
			if (DoDPS(phoenix, false, pmChasing))
			{
				rushing = true;
				return true;
			}
		}
		if (Creature* egg = GetClosestCreatureWithEntry(me, 24675, VISIBILITY_DISTANCE_NORMAL))
		{
			if (DoDPS(egg, false, pmChasing))
			{
				rushing = true;
				return true;
			}
		}
		Creature* boss = GetClosestCreatureWithEntry(me, 24664, VISIBILITY_DISTANCE_NORMAL);
		if (!boss)
		{
			boss = GetClosestCreatureWithEntry(me, 24857, VISIBILITY_DISTANCE_NORMAL);
		}
		if (DoDPS(boss, false, pmChasing))
		{
			rushing = true;
			return true;
		}
	}
	if (Group* myGroup = me->GetGroup())
	{
		if (ObjectGuid ogTankTarget = myGroup->GetGuidByTargetIcon(7))
		{
			if (Unit* tankTarget = ObjectAccessor::GetUnit(*me, ogTankTarget))
			{
				if (DoDPS(tankTarget, pmForceInstantOnly, pmChasing))
				{
					if (tankTarget->GetEntry() == 24664 || tankTarget->GetEntry() == 24857)
					{
						kael = true;
					}
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
					if (!sNingerManager->IsPolymorphed(leaderTarget))
					{
						if (DoDPS(leaderTarget, pmForceInstantOnly, pmChasing))
						{
							if (leaderTarget->GetEntry() == 24664 || leaderTarget->GetEntry() == 24857)
							{
								kael = true;
							}
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool NingerStrategy_Magisters_Terrace::DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing)
{
	if (!pmTarget)
	{
		return false;
	}
	else if (!pmTarget->IsAlive())
	{
		return false;
	}
	if (pmTarget->GetEntry() == 24664 || pmTarget->GetEntry() == 24857)
	{
		kael = true;
	}

	return NingerStrategy_Base::DoDPS(pmTarget, pmForceInstantOnly, pmChasing);
}

bool NingerStrategy_Magisters_Terrace::DoHeal(Unit* pmTarget, bool pmForceInstantOnly)
{
	if (!pmTarget)
	{
		return false;
	}
	else if (!pmTarget->IsAlive())
	{
		return false;
	}
	if (Unit* targetTarget = pmTarget->GetTarget())
	{
		if (targetTarget->GetEntry() == 24664 || targetTarget->GetEntry() == 24857)
		{
			kael = true;
		}
	}

	return NingerStrategy_Base::DoHeal(pmTarget, pmForceInstantOnly);
}