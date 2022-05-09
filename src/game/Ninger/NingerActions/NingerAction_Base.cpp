#include "NingerAction_Base.h"
#include "World/World.h"
#include "Grids/GridNotifiers.h"
#include "MotionGenerators/MovementGenerator.h"

NingerMovement::NingerMovement(Player* pmMe)
{
	me = pmMe;
	ogTankTarget = ObjectGuid();
	ogChaseTarget = ObjectGuid();
	ogFollowTarget = ObjectGuid();
	positionTarget = me->GetPosition();
	moveCheckDelay = 0;
	activeMovementType = NingerMovementType::NingerMovementType_None;
}

void NingerMovement::ResetMovement()
{
	me->StopMoving(true);
	me->GetMotionMaster()->Clear();
	ogTankTarget = ObjectGuid();
	ogChaseTarget = ObjectGuid();
	ogFollowTarget = ObjectGuid();
	positionTarget = me->GetPosition();
	moveCheckDelay = 0;
	activeMovementType = NingerMovementType::NingerMovementType_None;
}

void NingerMovement::Run()
{
	if (me)
	{
		if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
		{
			me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
		}
		if (me->IsWalking())
		{
			if (me->HasMovementFlag(MOVEFLAG_WALK_MODE))
			{
				me->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
			}
			if (!me->hasUnitState(UNIT_STAT_RUNNING))
			{
				me->addUnitState(UNIT_STAT_RUNNING);
			}
		}
	}
}

void NingerMovement::Update(uint32 pmDiff)
{
	if (moveCheckDelay > 0)
	{
		moveCheckDelay -= pmDiff;
		return;
	}
	if (!me)
	{
		return;
	}
	if (!me->IsAlive())
	{
		return;
	}
	if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
	{
		return;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_CAN_NOT_MOVE))
	{
		return;
	}
	if (me->hasUnitState(UnitState::UNIT_STAT_ROAMING_MOVE))
	{
		return;
	}
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return;
	}
	if (me->IsBeingTeleported())
	{
		return;
	}
	switch (activeMovementType)
	{
	case NingerMovementType::NingerMovementType_None:
	{
		break;
	}
	case NingerMovementType::NingerMovementType_Point:
	{
		float distance = me->GetDistance(positionTarget.x, positionTarget.y, positionTarget.z, DistanceCalculation::DIST_CALC_BOUNDING_RADIUS);
		if (distance > VISIBILITY_DISTANCE_LARGE)
		{
			ResetMovement();
		}
		else
		{
			if (distance < MIN_DISTANCE_GAP)
			{
				activeMovementType = NingerMovementType::NingerMovementType_None;
			}
			else
			{
				me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
				moveCheckDelay = DEFAULT_MOVEMENT_UPDATE_DELAY;
			}
		}
		break;
	}
	case NingerMovementType::NingerMovementType_Tank:
	{
		if (Unit* chaseTarget = ObjectAccessor::GetUnit(*me, ogTankTarget))
		{
			if (NingerStrategy_Base* ns = me->strategyMap[me->activeStrategyIndex])
			{
				float targetDistance = me->GetDistance(chaseTarget, true, DistanceCalculation::DIST_CALC_BOUNDING_RADIUS);
				float chaseDistance = chaseTarget->GetObjectBoundingRadius() + me->GetObjectBoundingRadius() + ns->dpsDistance;
				if (targetDistance > VISIBILITY_DISTANCE_LARGE)
				{
					ResetMovement();
					break;
				}
				if (me->GetSelectionGuid() != ogTankTarget)
				{
					me->SetSelectionGuid(ogTankTarget);
					me->SetTarget(chaseTarget);
				}
				if (targetDistance < chaseDistance + MAX_DISTANCE_GAP)
				{
					if (me->IsWithinLOSInMap(chaseTarget))
					{
						if (me->IsMoving())
						{
							me->StopMoving(true);
							me->GetMotionMaster()->Clear();
						}
						if (!me->isInFront(chaseTarget, RANGE_MAX_DISTANCE, M_PI / 8))
						{
							me->SetFacingToObject(chaseTarget);
						}
						break;
					}
				}
				float chaseAngle = ns->combatAngle;
				if (chaseAngle == 0.0f)
				{
					chaseAngle = chaseTarget->GetAngle(me);
				}
				if (chaseAngle < 0.0f)
				{
					chaseAngle = chaseAngle + M_PI * 2;
				}
				chaseAngle = chaseAngle + M_PI * 2;
				chaseAngle = frand(chaseAngle - MIN_ANGLE_GAP, chaseAngle + MIN_ANGLE_GAP);
				chaseTarget->GetNearPoint(chaseTarget, positionTarget.x, positionTarget.y, positionTarget.z, me->GetObjectBoundingRadius(), chaseDistance - MAX_DISTANCE_GAP, chaseAngle);
				Run();
				me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
				moveCheckDelay = DEFAULT_MOVEMENT_UPDATE_DELAY;
			}
		}
		else
		{
			ResetMovement();
		}
		break;
	}
	case NingerMovementType::NingerMovementType_Chase:
	{
		if (Unit* chaseTarget = ObjectAccessor::GetUnit(*me, ogChaseTarget))
		{
			if (NingerStrategy_Base* ns = me->strategyMap[me->activeStrategyIndex])
			{
				float targetDistance = me->GetDistance(chaseTarget, true, DistanceCalculation::DIST_CALC_BOUNDING_RADIUS);
				float chaseDistance = chaseTarget->GetObjectBoundingRadius() + me->GetObjectBoundingRadius() + ns->dpsDistance;
				float chaseDistanceMin = 0.0f;
				if (ns->dpsDistanceMin > 0.0f)
				{
					chaseDistanceMin = chaseTarget->GetObjectBoundingRadius() + me->GetObjectBoundingRadius() + ns->dpsDistanceMin;
				}
				if (targetDistance > VISIBILITY_DISTANCE_LARGE)
				{
					ResetMovement();
					break;
				}
				if (me->GetSelectionGuid() != ogChaseTarget)
				{
					me->SetSelectionGuid(ogChaseTarget);
					me->SetTarget(chaseTarget);
				}
				if (targetDistance < chaseDistance + MAX_DISTANCE_GAP && ((ns->dpsDistanceMin > 0.0f) ? (targetDistance > chaseDistanceMin - MAX_DISTANCE_GAP) : true))
				{
					if (me->IsWithinLOSInMap(chaseTarget))
					{
						bool positionOK = true;
						if (chaseTarget->GetSelectionGuid() != me->GetObjectGuid())
						{
							if (ns->forceBack)
							{
								if (!chaseTarget->isInBack(me, RANGE_MAX_DISTANCE, M_PI / 4))
								{
									positionOK = false;
								}
							}
						}
						if (positionOK)
						{
							if (me->IsMoving())
							{
								me->StopMoving(true);
								me->GetMotionMaster()->Clear();
							}
							if (!me->isInFront(chaseTarget, RANGE_MAX_DISTANCE, M_PI / 8))
							{
								me->SetFacingToObject(chaseTarget);
							}
							break;
						}
					}
				}
				float chaseAngle = chaseTarget->GetAngle(me);
				if (ns->forceBack)
				{
					chaseAngle = chaseTarget->GetOrientation() + M_PI;
				}
				if (chaseAngle < 0.0f)
				{
					chaseAngle = chaseAngle + M_PI * 2;
				}
				chaseAngle = chaseAngle + M_PI * 2;
				chaseAngle = frand(chaseAngle - MIN_ANGLE_GAP, chaseAngle + MIN_ANGLE_GAP);
				chaseTarget->GetNearPoint(chaseTarget, positionTarget.x, positionTarget.y, positionTarget.z, me->GetObjectBoundingRadius(), chaseDistance - MAX_DISTANCE_GAP, chaseAngle);
				Run();
				me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
				moveCheckDelay = DEFAULT_MOVEMENT_UPDATE_DELAY;
			}
		}
		else
		{
			ResetMovement();
		}
		break;
	}
	case NingerMovementType::NingerMovementType_Follow:
	{
		if (Unit* followTarget = ObjectAccessor::GetUnit(*me, ogFollowTarget))
		{
			if (NingerStrategy_Base* ns = me->strategyMap[me->activeStrategyIndex])
			{
				float targetDistance = me->GetDistance(followTarget);
				float followDistance = followTarget->GetObjectBoundingRadius() + me->GetObjectBoundingRadius() + ns->followDistance;
				if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
				{
					ResetMovement();
					break;
				}
				if (me->GetSelectionGuid() != ogFollowTarget)
				{
					me->SetSelectionGuid(ogFollowTarget);
					me->SetTarget(followTarget);
				}
				if (targetDistance < followDistance + MAX_DISTANCE_GAP)
				{
					if (me->IsWithinLOSInMap(followTarget))
					{
						if (me->IsMoving())
						{
							me->StopMoving(true);
							me->GetMotionMaster()->Clear();
						}
						if (!me->isInFront(followTarget, RANGE_MAX_DISTANCE, M_PI / 8))
						{
							me->SetFacingToObject(followTarget);
						}
						break;
					}
				}
				float followAngle = followTarget->GetAngle(me);
				if (followAngle < 0.0f)
				{
					followAngle = followAngle + M_PI * 2;
				}
				followAngle = followAngle + M_PI * 2;
				followAngle = frand(followAngle - MIN_ANGLE_GAP, followAngle + MIN_ANGLE_GAP);
				followTarget->GetNearPoint(followTarget, positionTarget.x, positionTarget.y, positionTarget.z, followTarget->GetObjectBoundingRadius(), followDistance - MAX_DISTANCE_GAP, followAngle);
				Run();
				me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
				moveCheckDelay = DEFAULT_MOVEMENT_UPDATE_DELAY;
			}
		}
		else
		{
			ResetMovement();
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

bool NingerMovement::Tank(Unit* pmTankTarget)
{
	if (me && pmTankTarget)
	{
		float distance = me->GetDistance(pmTankTarget);
		if (distance < VISIBILITY_DISTANCE_NORMAL)
		{
			activeMovementType = NingerMovementType::NingerMovementType_Tank;
			if (ogTankTarget != pmTankTarget->GetObjectGuid())
			{
				ogTankTarget = pmTankTarget->GetObjectGuid();
				moveCheckDelay = 0;
			}
			return true;
		}
	}

	return false;
}

bool NingerMovement::Chase(Unit* pmChaseTarget)
{
	if (me && pmChaseTarget)
	{
		if (me->IsWithinDist(pmChaseTarget, VISIBILITY_DISTANCE_NORMAL))
		{
			activeMovementType = NingerMovementType::NingerMovementType_Chase;
			if (ogChaseTarget != pmChaseTarget->GetObjectGuid())
			{
				ogChaseTarget = pmChaseTarget->GetObjectGuid();
				moveCheckDelay = 0;
			}
			return true;
		}
	}

	return false;
}

bool NingerMovement::Follow(Unit* pmFollowTarget)
{
	if (me && pmFollowTarget)
	{
		if (me->IsWithinDist(pmFollowTarget, VISIBILITY_DISTANCE_NORMAL))
		{
			activeMovementType = NingerMovementType::NingerMovementType_Follow;
			if (ogFollowTarget != pmFollowTarget->GetObjectGuid())
			{
				ogFollowTarget = pmFollowTarget->GetObjectGuid();
				moveCheckDelay = 0;
			}
			return true;
		}
	}

	return false;
}

void NingerMovement::Point(Position pmPosTarget, uint32 pmLimit)
{
	if (!me)
	{
		return;
	}
	float distance = me->GetDistance(pmPosTarget.x, pmPosTarget.y, pmPosTarget.z);
	if (distance < VISIBILITY_DISTANCE_NORMAL)
	{
		activeMovementType = NingerMovementType::NingerMovementType_Point;
		if (me->IsNonMeleeSpellCasted(false))
		{
			me->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
			me->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
		}
		positionTarget = pmPosTarget;
		Run();
		me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
		moveCheckDelay = DEFAULT_MOVEMENT_UPDATE_DELAY;
	}
}

bool NingerMovement::Direction(Unit* pmCommander, uint32 pmDirection, uint32 pmLimit, float pmDistance)
{
	bool result = false;

	if (pmCommander)
	{
		float absAngle = pmCommander->GetOrientation();
		switch (pmDirection)
		{
		case NingerMovementDirection_Forward:
		{
			break;
		}
		case NingerMovementDirection_Back:
		{
			absAngle = absAngle + M_PI;
			break;
		}
		case NingerMovementDirection_Left:
		{
			absAngle = absAngle + M_PI / 2;
			break;
		}
		case NingerMovementDirection_Right:
		{
			absAngle = absAngle + M_PI * 3 / 2;
			break;
		}
		default:
		{
			break;
		}
		}
		me->GetNearPoint(me, positionTarget.x, positionTarget.y, positionTarget.z, me->GetObjectBoundingRadius(), pmDistance, absAngle);
		activeMovementType = NingerMovementType::NingerMovementType_Point;
		if (me->IsNonMeleeSpellCasted(false))
		{
			me->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
			me->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
		}
		Run();
		me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
		result = true;
	}

	return result;
}

bool NingerMovement::Direction(float pmAngle, uint32 pmLimit, float pmDistance)
{
	bool result = false;

	me->GetNearPoint(me, positionTarget.x, positionTarget.y, positionTarget.z, me->GetObjectBoundingRadius(), pmDistance, pmAngle);
	activeMovementType = NingerMovementType::NingerMovementType_Point;
	Run();
	if (me->IsNonMeleeSpellCasted(false))
	{
		me->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
		me->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
	}
	me->StopMoving(true);
	me->GetMotionMaster()->Clear();
	me->GetMotionMaster()->MovePoint(0, positionTarget.x, positionTarget.y, positionTarget.z);
	result = true;

	return result;
}

NingerAction_Base::NingerAction_Base()
{
	me = nullptr;
	specialty = 0;
	rti = -1;
}

void NingerAction_Base::Prepare()
{
	if (me)
	{
		me->SetPvP(true);
		me->DurabilityRepairAll(false, 0, false);
	}
}

void NingerAction_Base::Reset()
{
	rti = -1;
	ClearTarget();
}

void NingerAction_Base::Update(uint32 pmDiff)
{

}

bool NingerAction_Base::DPS(Unit* pmTarget, bool pmAOE, bool pmRush)
{
	return false;
}

bool NingerAction_Base::Tank(Unit* pmTarget, bool pmAOE)
{
	return false;
}

bool NingerAction_Base::Heal(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::GroupHeal(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::SimpleHeal(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::Cure(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::Buff(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::Assist(Unit* pmTarget)
{
	return false;
}

bool NingerAction_Base::Revive(Player* pmTarget)
{
	return false;
}

bool NingerAction_Base::Petting(bool pmSummon, bool pmReset)
{
	return false;
}

void NingerAction_Base::InitializeCharacter(uint32 pmTarGetLevel, uint32 pmSpecialtyTabIndex)
{

}

void NingerAction_Base::ResetTalent()
{

}

bool NingerAction_Base::InitializeEquipments(bool pmReset)
{
	return true;
}

void NingerAction_Base::RemoveEquipments()
{
	if (!me)
	{
		return;
	}
	for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
	{
		if (Item* inventoryItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
		{
			me->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
		}
	}
	for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
	{
		if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
		{
			me->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
		}
	}
}

void NingerAction_Base::LearnTalent(uint32 pmTalentId, uint32 pmMaxRank)
{
	if (!me)
	{
		return;
	}
	uint32 checkRank = 0;
	while (checkRank < pmMaxRank)
	{
		me->LearnTalent(pmTalentId, checkRank);
		checkRank++;
	}
}

void NingerAction_Base::TrainSpells(uint32 pmTrainerEntry)
{
	if (const CreatureInfo* ci = sObjectMgr.GetCreatureTemplate(pmTrainerEntry))
	{
		TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerTemplateSpells(ci->TrainerTemplateId);
		bool hasNew = false;
		while (true)
		{
			hasNew = false;
			for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
			{
				TrainerSpell const* tSpell = &itr->second;
				if (me->HasSpell(tSpell->spell))
				{
					continue;
				}
				if (tSpell->reqSkill && !me->HasSpell(tSpell->reqSkill))
				{
					continue;
				}
				TrainerSpellState state = me->GetTrainerSpellState(tSpell, tSpell->reqLevel);
				if (state == TrainerSpellState::TRAINER_SPELL_GREEN)
				{
					me->learnSpell(tSpell->spell, true);
					hasNew = true;
				}
			}
			if (!hasNew)
			{
				break;
			}
		}
	}
}

void NingerAction_Base::EquipRandomItem(uint32 pmEquipSlot, uint32 pmClass, uint32 pmSubclass, uint32 pmMinQuality, int pmModType, std::unordered_set<uint32> pmInventoryTypeSet)
{
	bool checkStat = true;
	if (pmModType < 0)
	{
		checkStat = false;
	}
	uint32 inventoryType = 0;
	if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HEAD)
	{
		inventoryType = 1;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS)
	{
		inventoryType = 3;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WRISTS)
	{
		inventoryType = 9;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WAIST)
	{
		inventoryType = 6;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FEET)
	{
		inventoryType = 8;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HANDS)
	{
		inventoryType = 10;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_CHEST)
	{
		inventoryType = 5;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_LEGS)
	{
		inventoryType = 7;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_BACK)
	{
		inventoryType = 16;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_NECK)
	{
		inventoryType = 2;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1)
	{
		inventoryType = 11;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
	{
		inventoryType = 11;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
	{
		inventoryType = 17;
	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_OFFHAND)
	{

	}
	else if (pmEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_RANGED)
	{
		inventoryType = 15;
	}
	else
	{
		return;
	}

	std::ostringstream inventoryTypeQueryStream;
	if (pmInventoryTypeSet.size() > 0)
	{
		inventoryTypeQueryStream << " ( 1 = 0 ";
		for (std::unordered_set<uint32>::iterator it = pmInventoryTypeSet.begin(); it != pmInventoryTypeSet.end(); it++)
		{
			inventoryTypeQueryStream << " or InventoryType = " << *it;
		}
		inventoryTypeQueryStream << " ) ";
	}
	else
	{
		inventoryTypeQueryStream << " InventoryType = " << inventoryType;
	}
	std::string inventoryTypeQuery = inventoryTypeQueryStream.str();
	int activeReqLevel = me->GetLevel();
	int minReqLevel = activeReqLevel - 3;
	while (minReqLevel > 0)
	{
		int activeQuality = ItemQualities::ITEM_QUALITY_EPIC;
		while (activeQuality >= pmMinQuality)
		{
			std::unordered_set<uint32> availableItemEntrySet;
			std::ostringstream queryStream;
			queryStream << "SELECT entry FROM item_template where " << inventoryTypeQuery << " and class = " << pmClass << " and subclass = " << pmSubclass << " and Quality = " << activeQuality << " and RequiredLevel >= " << minReqLevel << " and RequiredLevel <= " << activeReqLevel << " and AllowableClass = -1 and AllowableRace = -1 and RequiredSkill = 0 and requiredspell = 0 and requiredhonorrank = 0 and RequiredCityRank = 0 and RequiredReputationFaction = 0 order by rand()";
			QueryResult* itemQR = WorldDatabase.Query(queryStream.str().c_str());
			if (itemQR)
			{
				do
				{
					Field* fields = itemQR->Fetch();
					availableItemEntrySet.insert(fields[0].GetUInt32());
				} while (itemQR->NextRow());
			}
			delete itemQR;
			for (std::unordered_set<uint32>::iterator entryIt = availableItemEntrySet.begin(); entryIt != availableItemEntrySet.end(); entryIt++)
			{
				if (const ItemPrototype* pProto = sObjectMgr.GetItemPrototype(*entryIt))
				{
					bool hasStat = false;
					if (checkStat)
					{
						if (pProto->RandomProperty > 0 || pProto->RandomSuffix > 0)
						{
							hasStat = true;
						}
						else
						{
							for (uint32 statIndex = 0; statIndex < pProto->StatsCount; statIndex++)
							{
								if (pProto->ItemStat[statIndex].ItemStatType == pmModType)
								{
									hasStat = true;
									break;
								}
							}
						}
					}
					else
					{
						hasStat = true;
					}
					if (hasStat)
					{
						if (Item* pItem = Item::CreateItem(*entryIt, 1))
						{
							uint16 dest = 0;
							if (me->CanEquipItem(pmEquipSlot, dest, pItem, false) == InventoryResult::EQUIP_ERR_OK)
							{
								me->EquipItem(dest, pItem, true);
								std::ostringstream msgStream;
								msgStream << me->GetName() << " Equiped " << pItem->GetProto()->Name1;
								sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
								sLog.outBasic(msgStream.str().c_str());
								return;
							}
						}
					}
				}
			}
			activeQuality--;
		}
		activeReqLevel -= 4;
		minReqLevel = activeReqLevel - 3;
	}
}

void NingerAction_Base::PetAttack(Unit* pmTarget)
{
	if (me)
	{
		if (Pet* myPet = me->GetPet())
		{
			if (myPet->IsAlive())
			{
				if (UnitAI* uai = myPet->AI())
				{
					uai->AttackStart(pmTarget);
				}
			}
		}
	}
}

void NingerAction_Base::PetStop()
{
	if (me)
	{
		if (Pet* myPet = me->GetPet())
		{
			myPet->AttackStop();
			if (CharmInfo* pci = myPet->GetCharmInfo())
			{
				if (pci->HasCommandState(CommandStates::COMMAND_ATTACK))
				{
					pci->SetCommandState(CommandStates::COMMAND_FOLLOW);
				}
			}
		}
	}
}

bool NingerAction_Base::UseItem(Item* pmItem, Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}

	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return false;
	}

	if (const ItemPrototype* proto = pmItem->GetProto())
	{
		me->SetSelectionGuid(pmTarget->GetObjectGuid());
		me->SetTarget(pmTarget);
		SpellCastTargets targets;
		targets.Update(pmTarget);
		me->CastItemUseSpell(pmItem, targets, 1, 0, proto->Spells[0].SpellId);
		return true;
	}

	return false;
}

bool NingerAction_Base::UseItem(Item* pmItem, Item* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return false;
	}
	if (!pmTarget)
	{
		return false;
	}

	if (const ItemPrototype* proto = pmItem->GetProto())
	{
		SpellCastTargets targets;
		targets.setItemTarget(pmTarget);
		me->CastItemUseSpell(pmItem, targets, 1, 0, proto->Spells[0].SpellId);
		return true;
	}

	return false;
}

bool NingerAction_Base::CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura, bool pmOnlyMyAura, bool pmClearShapeShift, uint32 pmMaxAuraStack)
{
	if (!SpellValid(pmSpellId))
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return true;
	}
	if (pmClearShapeShift)
	{
		ClearShapeshift();
	}
	const SpellEntry* pS = sSpellTemplate.LookupEntry<SpellEntry>(pmSpellId);
	if (!pS)
	{
		return false;
	}
	if (pmTarget)
	{
		if (!me->IsWithinLOSInMap(pmTarget))
		{
			return false;
		}
		if (pmTarget->IsImmuneToSpell(pS, me->GetObjectGuid() == pmTarget->GetObjectGuid(), GetCheckCastEffectMask(pS)))
		{
			return false;
		}
		if (pmCheckAura)
		{
			if (pmOnlyMyAura)
			{
				if (pmTarget->HasAura(pmSpellId))
				{
					if (pmMaxAuraStack > 1)
					{
						if (Aura* activeAura = pmTarget->GetAura(pmSpellId, SpellEffectIndex::EFFECT_INDEX_0))
						{
							if (activeAura->GetStackAmount() >= pmMaxAuraStack)
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				if (pmTarget->HasAura(pmSpellId))
				{
					if (pmMaxAuraStack > 1)
					{
						if (Aura* activeAura = pmTarget->GetAura(pmSpellId, SpellEffectIndex::EFFECT_INDEX_0))
						{
							if (activeAura->GetStackAmount() >= pmMaxAuraStack)
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
			}
		}
		if (!me->isInFront(pmTarget, RANGE_MAX_DISTANCE, M_PI / 4))
		{
			me->SetFacingToObject(pmTarget);
		}
		if (me->GetSelectionGuid() != pmTarget->GetObjectGuid())
		{
			ChooseTarget(pmTarget);
		}
	}
	for (size_t i = 0; i < MAX_SPELL_REAGENTS; i++)
	{
		if (pS->Reagent[i] > 0)
		{
			if (!me->HasItemCount(pS->Reagent[i], pS->ReagentCount[i]))
			{
				me->StoreNewItemInBestSlots(pS->Reagent[i], pS->ReagentCount[i] * 10);
			}
		}
	}
	if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
	{
		me->SetStandState(UNIT_STAND_STATE_STAND);
	}
	me->CastSpell(pmTarget, pS, TriggerCastFlags::TRIGGERED_NONE);
	return true;

	//SpellCastResult scr = me->CastSpell(pmTarget, pS);
	//if (scr == SpellCastResult::SPELL_CAST_OK)
	//{
	//    return true;
	//}
	//return false;
}

void NingerAction_Base::ClearShapeshift()
{
	if (!me)
	{
		return;
	}
}

void NingerAction_Base::CancelAura(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return;
	}
	if (!me)
	{
		return;
	}
	me->RemoveAurasDueToSpell(pmSpellID);
}

bool NingerAction_Base::Eat()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	else if (me->IsInCombat())
	{
		return false;
	}
	uint32 foodEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 75)
	{
		foodEntry = 35950;
	}
	else if (myLevel >= 65)
	{
		foodEntry = 33449;
	}
	else if (myLevel >= 55)
	{
		foodEntry = 27855;
	}
	else if (myLevel >= 45)
	{
		foodEntry = 8950;
	}
	else if (myLevel >= 35)
	{
		foodEntry = 4601;
	}
	else if (myLevel >= 25)
	{
		foodEntry = 4544;
	}
	else if (myLevel >= 15)
	{
		foodEntry = 4542;
	}
	else if (myLevel >= 5)
	{
		foodEntry = 4541;
	}
	else
	{
		foodEntry = 4540;
	}
	if (!me->HasItemCount(foodEntry, 1))
	{
		me->StoreNewItemInBestSlots(foodEntry, 20);
	}
	me->CombatStop(true);
	me->StopMoving(true);
	me->GetMotionMaster()->Clear();
	ClearTarget();

	Item* pFood = GetItemInInventory(foodEntry);
	if (pFood && !pFood->IsInTrade())
	{
		if (UseItem(pFood, me))
		{
			me->ningerMovement->ResetMovement();
			return true;
		}
	}
	return false;
}

bool NingerAction_Base::Drink()
{
	if (!me)
	{
		return false;
	}
	if (!me->IsAlive())
	{
		return false;
	}
	if (me->IsInCombat())
	{
		return false;
	}
	uint32 drinkEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 75)
	{
		drinkEntry = 33445;
	}
	else if (myLevel >= 71)
	{
		drinkEntry = 33444;
	}
	else if (myLevel >= 65)
	{
		drinkEntry = 27860;
	}
	else if (myLevel >= 60)
	{
		drinkEntry = 28399;
	}
	else if (myLevel >= 55)
	{
		drinkEntry = 18300;
	}
	else if (myLevel >= 45)
	{
		drinkEntry = 8766;
	}
	else if (myLevel >= 35)
	{
		drinkEntry = 1645;
	}
	else if (myLevel >= 25)
	{
		drinkEntry = 1708;
	}
	else if (myLevel >= 15)
	{
		drinkEntry = 1205;
	}
	else if (myLevel >= 5)
	{
		drinkEntry = 1179;
	}
	else
	{
		drinkEntry = 159;
	}

	if (!me->HasItemCount(drinkEntry, 1))
	{
		me->StoreNewItemInBestSlots(drinkEntry, 20);
	}
	me->CombatStop(true);
	me->StopMoving(true);
	me->GetMotionMaster()->Clear();
	ClearTarget();
	Item* pDrink = GetItemInInventory(drinkEntry);
	if (pDrink && !pDrink->IsInTrade())
	{
		if (UseItem(pDrink, me))
		{
			me->ningerMovement->ResetMovement();
			return true;
		}
	}

	return false;
}

bool NingerAction_Base::HealthPotion()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	else if (!me->IsInCombat())
	{
		return false;
	}
	uint32 potionEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 70)
	{
		potionEntry = 33447;
	}
	else if (myLevel >= 55)
	{
		potionEntry = 22829;
	}
	else if (myLevel >= 45)
	{
		potionEntry = 13446;
	}
	else if (myLevel >= 35)
	{
		potionEntry = 3928;
	}
	else if (myLevel >= 21)
	{
		potionEntry = 1710;
	}
	else if (myLevel >= 12)
	{
		potionEntry = 929;
	}
	else if (myLevel >= 3)
	{
		potionEntry = 858;
	}
	else
	{
		potionEntry = 118;
	}
	if (!me->HasItemCount(potionEntry, 1))
	{
		me->StoreNewItemInBestSlots(potionEntry, 20);
	}
	Item* pPotion = GetItemInInventory(potionEntry);
	if (pPotion && !pPotion->IsInTrade())
	{
		if (UseItem(pPotion, me))
		{
			return true;
		}
	}
	return false;
}

bool NingerAction_Base::ManaPotion()
{
	if (!me)
	{
		return false;
	}
	else if (!me->IsAlive())
	{
		return false;
	}
	else if (!me->IsInCombat())
	{
		return false;
	}
	uint32 potionEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 70)
	{
		potionEntry = 33448;
	}
	else if (myLevel >= 55)
	{
		potionEntry = 22832;
	}
	else if (myLevel >= 49)
	{
		potionEntry = 13444;
	}
	else if (myLevel >= 41)
	{
		potionEntry = 13443;
	}
	else if (myLevel >= 31)
	{
		potionEntry = 6149;
	}
	else if (myLevel >= 22)
	{
		potionEntry = 3827;
	}
	else if (myLevel >= 14)
	{
		potionEntry = 3385;
	}
	else if (myLevel >= 5)
	{
		potionEntry = 2455;
	}
	if (potionEntry > 0)
	{
		if (!me->HasItemCount(potionEntry, 1))
		{
			me->StoreNewItemInBestSlots(potionEntry, 20);
		}
		Item* pPotion = GetItemInInventory(potionEntry);
		if (pPotion && !pPotion->IsInTrade())
		{
			if (UseItem(pPotion, me))
			{
				return true;
			}
		}
	}

	return false;
}

bool NingerAction_Base::RandomTeleport()
{
	int myLevel = me->GetLevel();
	std::unordered_map<uint32, ObjectGuid> samePlayersMap;
	std::unordered_map<uint32, WorldSession*> allSessions = sWorld.GetAllSessions();
	for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
	{
		if (WorldSession* eachWS = wsIT->second)
		{
			if (!eachWS->isNinger)
			{
				if (Player* eachPlayer = eachWS->GetPlayer())
				{
					int eachLevel = eachPlayer->GetLevel();
					if (myLevel > eachLevel - 5 && myLevel < eachLevel + 5)
					{
						//if (me->IsHostileTo(eachPlayer))
						//{
						//    samePlayersMap[samePlayersMap.size()] = eachPlayer->GetObjectGuid();
						//}
						samePlayersMap[samePlayersMap.size()] = eachPlayer->GetObjectGuid();
					}
				}
			}
		}
	}
	uint32 targetPlayerIndex = urand(0, samePlayersMap.size());
	if (Player* targetPlayer = ObjectAccessor::FindPlayer(samePlayersMap[targetPlayerIndex]))
	{
		float nearX = 0.0f;
		float nearY = 0.0f;
		float nearZ = 0.0f;
		float nearDistance = frand(200.0f, 400.0f);
		float nearAngle = frand(0.0f, M_PI * 2);
		targetPlayer->GetNearPoint(targetPlayer, nearX, nearY, nearZ, targetPlayer->GetCombatReach(), nearDistance, nearAngle);
		me->TeleportTo(me->GetMapId(), nearX, nearY, nearZ, 0);
	}
	return false;
}

void NingerAction_Base::ChooseTarget(Unit* pmTarget)
{
	if (pmTarget)
	{
		if (me)
		{
			me->SetSelectionGuid(pmTarget->GetObjectGuid());
			me->SetTarget(pmTarget);
		}
	}
}

void NingerAction_Base::ClearTarget()
{
	if (me)
	{
		me->SetSelectionGuid(ObjectGuid());
		me->SetTarget(nullptr);
		me->AttackStop();
		me->InterruptNonMeleeSpells(true);
	}
}

bool NingerAction_Base::SpellValid(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (me->IsSpellReady(pmSpellID))
	{
		return true;
	}
	return false;
}

Item* NingerAction_Base::GetItemInInventory(uint32 pmEntry)
{
	if (!me)
	{
		return NULL;
	}
	for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
		if (pItem)
		{
			if (pItem->GetEntry() == pmEntry)
			{
				return pItem;
			}
		}
	}

	for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		if (Bag* pBag = (Bag*)me->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		{
			for (uint32 j = 0; j < pBag->GetBagSize(); j++)
			{
				Item* pItem = me->GetItemByPos(i, j);
				if (pItem)
				{
					if (pItem->GetEntry() == pmEntry)
					{
						return pItem;
					}
				}
			}
		}
	}

	return NULL;
}

Player* NingerAction_Base::GetNearbyHostilePlayer(float pmRange)
{
	std::list<Player*> targets;
	MaNGOS::AnyPlayerInObjectRangeCheck check(me, pmRange);
	MaNGOS::PlayerListSearcher<MaNGOS::AnyPlayerInObjectRangeCheck> searcher(targets, check);
	Cell::VisitWorldObjects(me, searcher, pmRange);
	for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
	{
		if (Player* eachPlayer = *itr)
		{
			if (eachPlayer->IsAlive())
			{
				if (me->CanAttack(eachPlayer))
				{
					if (eachPlayer->IsVisibleForOrDetect(me, me, false))
					{
						return eachPlayer;
					}
				}
			}
		}
	}

	return nullptr;
}

Unit* NingerAction_Base::GetNearbyHostileUnit(float pmRange)
{
	std::list<Creature*> creatureList;
	MaNGOS::AnyUnitInObjectRangeCheck u_check(me, pmRange);
	MaNGOS::CreatureListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(creatureList, u_check);
	Cell::VisitAllObjects(me, searcher, pmRange);
	if (!creatureList.empty())
	{
		for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
		{
			if (Creature* hostileCreature = *itr)
			{
				if (hostileCreature->IsAlive())
				{
					if (!hostileCreature->IsCivilian())
					{
						if (me->CanAttack(hostileCreature))
						{
							return hostileCreature;
						}
					}
				}
			}
		}
	}

	return nullptr;
}