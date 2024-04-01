#include "NierScript_Base.h"

#include "../NierConfig.h"
#include "../NierManager.h"
#include "Accounts/AccountMgr.h"
#include "Entities/ObjectGuid.h"
#include "World/World.h"
#include "MotionGenerators/PointMovementGenerator.h"
#include "Movement/MoveSpline.h"
#include "Movement/MoveSplineInit.h"
#include "Grids/GridNotifiers.h"

NierScript_Base::NierScript_Base(Player* pMe)
{
	me = pMe;
	masterId = 0;
	specialty = 0;

	orderType = OrderType::OrderType_None;
	actionDelay = 0;
	updateDelay = 5 * IN_MILLISECONDS;
	prepareDelay = 5 * IN_MILLISECONDS;
	assembleDelay = 0;
	reviveDelay = 0;
	wanderDelay = 0;
	restDelay = 0;
	drinkDelay = 0;
	interruptDelay = 0;
	helpDelay = 0;
	orderDelay = 0;

	pvp = true;
	relocateDelay = urand(10000, 60000);
	repopDelay = 0;

	combatDuration = 0;

	dpsDistance = CONTACT_DISTANCE;
	followDistance = INTERACTION_DISTANCE;
	chasing = true;
	freezing = false;
	rushing = false;
}

bool NierScript_Base::Prepare()
{
	if (me)
	{
		Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_HEAD);
		if (!currentEquip)
		{
			// no head reset 
			InitializeEquipments(true);
		}
		me->SetPvP(true);
		me->DurabilityRepairAll(false, 0, false);
		if (!me->GetGroup())
		{
			if (me->GetMap()->Instanceable())
			{
				uint32 hbId = 0;
				float hbX = 0.0f;
				float hbY = 0.0f;
				float hbZ = 0.0f;
				me->GetHomebindLocation(hbX, hbY, hbZ, hbId);
				me->TeleportTo(hbId, hbX, hbY, hbZ, 0.0f);
			}
		}
	}

	return true;
}

void NierScript_Base::Update(uint32 pDiff)
{
	if (actionDelay > 0)
	{
		actionDelay -= pDiff;
	}
	else
	{
		actionDelay = 200;
	}
	if (wanderDelay > 0)
	{
		wanderDelay -= pDiff;
	}
	if (helpDelay > 0)
	{
		helpDelay -= pDiff;
	}
	if (prepareDelay > 0)
	{
		prepareDelay -= pDiff;
		if (prepareDelay <= 0)
		{
			Prepare();
			prepareDelay = urand(60000, 120000);
		}
	}
	if (restDelay > 0)
	{
		restDelay -= pDiff;
		if (drinkDelay > 0)
		{
			drinkDelay -= pDiff;
			if (drinkDelay <= 0)
			{
				Drink();
			}
		}
		return;
	}
	if (me->IsInCombat())
	{
		combatDuration += pDiff;
		restDelay = 0;
		drinkDelay = 0;
	}
	else
	{
		if (combatDuration > 0)
		{
			combatDuration = 0;
			ClearTarget();
			//me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_SIT);
			//standDelay = 500;
		}
	}

	// group strategy
	if (Group* myGroup = me->GetGroup())
	{
		if (reviveDelay > 0)
		{
			reviveDelay -= pDiff;
			if (reviveDelay <= 0)
			{
				reviveDelay = 0;
				if (!me->IsAlive())
				{
					me->ResurrectPlayer(0.2f);
					me->SpawnCorpseBones();
				}
			}
		}
		if (assembleDelay > 0)
		{
			assembleDelay -= pDiff;
			if (assembleDelay <= 0)
			{
				assembleDelay = 0;
				if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid()))
				{
					if (!me->IsAlive())
					{
						reviveDelay = 5000;
						me->Whisper("Arrived. Revive in 5 seconds.", Language::LANG_UNIVERSAL, leader->GetObjectGuid());
					}
					else
					{
						me->Whisper("Arrived.", Language::LANG_UNIVERSAL, leader->GetObjectGuid());
					}
					me->ClearInCombat();
					me->TeleportTo(leader->GetMapId(), leader->GetPositionX(), leader->GetPositionY(), leader->GetPositionZ(), 0);
				}
				return;
			}
		}
		if (freezing)
		{
			return;
		}
		if (orderDelay > 0)
		{
			orderDelay -= pDiff;
			switch (orderType)
			{
			case OrderType::OrderType_None:
			{
				orderDelay = 0;
				break;
			}
			case OrderType::OrderType_Tank:
			{
				bool ordering = false;
				Unit* skull = ObjectAccessor::GetUnit(*me, myGroup->GetGuidByTargetIcon(7));
				if (Tank(skull))
				{
					ordering = true;
				}
				if (!ordering)
				{
					orderType = OrderType::OrderType_None;
					orderDelay = 0;
				}
				break;
			}
			case OrderType::OrderType_DPS:
			{
				break;
			}
			case OrderType::OrderType_Revive:
			{
				break;
			}
			case OrderType::OrderType_Move:
			{
				break;
			}
			default:
			{
				break;
			}
			}
			return;
		}
		if (interruptDelay > 0)
		{
			interruptDelay -= pDiff;
		}

		bool groupInCombat = false;
		Player* tank = nullptr;
		for (const auto& citr : myGroup->GetMemberSlots())
		{
			if (Player* member = sObjectMgr.GetPlayer(citr.guid))
			{
				if (member->groupRole == GroupRole::GroupRole_Tank)
				{
					tank = member;
				}
				if (member->IsInCombat())
				{
					if (member->GetDistance(me) < VISIBILITY_DISTANCE_NORMAL)
					{
						groupInCombat = true;
					}
				}
			}
		}
		Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGuid());
		if (groupInCombat)
		{
			Unit* skull = ObjectAccessor::GetUnit(*me, myGroup->GetGuidByTargetIcon(7));
			if (skull)
			{
				if (!skull->IsAlive())
				{
					myGroup->SetTargetIcon(7, me->GetObjectGuid(), ObjectGuid());
					skull = nullptr;
				}
			}
			if (me->groupRole == GroupRole::GroupRole_Tank)
			{
				if (Unit* eachTarget = me->GetTarget())
				{
					if (!eachTarget->IsAlive())
					{
						ClearTarget();
					}
					else if (!me->CanAttack(eachTarget))
					{
						ClearTarget();
					}
				}
				// get nearby enemies 
				std::list<Unit*> enemyList;
				enemyList.clear();
				if (tank)
				{
					MaNGOS::AnyUnitInObjectRangeCheck unitChecker(tank, ATTACK_DISTANCE);
					MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> unitSearcher(enemyList, unitChecker);
					Cell::VisitAllObjects(tank, unitSearcher, ATTACK_DISTANCE);
				}
				// set a skull 
				if (!skull)
				{
					for (Unit* eachEnemy : enemyList)
					{
						if (eachEnemy->IsAlive())
						{
							if (me->CanAttack(eachEnemy))
							{
								myGroup->SetTargetIcon(7, me->GetObjectGuid(), eachEnemy->GetObjectGuid());
								skull = eachEnemy;
								break;
							}
						}
					}
				}
				// skull ot 
				if (skull)
				{
					if (skull->GetSelectionGuid() != me->GetObjectGuid())
					{
						if (Tank(skull))
						{
							return;
						}
					}
				}
				// others ot 
				bool tanked = false;
				for (Unit* eachEnemy : enemyList)
				{
					if (eachEnemy->GetSelectionGuid() != me->GetObjectGuid())
					{
						if (Tank(eachEnemy))
						{
							return;
						}
					}
				}
				// tank skull 
				if (Tank(skull))
				{
					return;
				}
			}
			else if (me->groupRole == GroupRole::GroupRole_Healer)
			{
				if (Unit* eachTarget = me->GetTarget())
				{
					if (!eachTarget->IsAlive())
					{
						ClearTarget();
					}
				}
				if (Heal(tank))
				{
					return;
				}
				else
				{
					for (const auto& citr : myGroup->GetMemberSlots())
					{
						if (Player* member = sObjectMgr.GetPlayer(citr.guid))
						{
							if (member->IsAlive())
							{
								float targetHealthPct = member->GetHealthPercent();
								if (targetHealthPct < 50.0f)
								{
									if (Heal(member))
									{
										return;
									}
								}
							}
						}
					}
				}
				//if (eachNier->DPS(skull, tank, rushing))
				//{
				//    continue;
				//}
			}
			else if (me->groupRole == GroupRole::GroupRole_DPS)
			{
				if (Unit* eachTarget = me->GetTarget())
				{
					if (!eachTarget->IsAlive())
					{
						ClearTarget();
					}
					else if (!me->CanAttack(eachTarget))
					{
						ClearTarget();
					}
				}
				if (skull)
				{
					if (DPS(skull, tank, rushing))
					{
						return;
					}
				}
				if (tank)
				{
					if (Unit* victim = leader->GetTarget())
					{
						if (!victim->IsAlive())
						{
							return;
						}
						else if (!me->CanAttack(victim))
						{
							return;
						}
						if (tank->CanReachWithMeleeAttack(victim))
						{
							if (DPS(victim, tank, rushing))
							{
								return;
							}
						}
					}
				}
			}
		}
		else
		{
			rushing = false;
			if (Cure())
			{
				return;
			}
			if (Buff())
			{
				return;
			}
			if (Revive())
			{
				return;
			}
			if (Rest())
			{
				return;
			}
		}
		Follow(leader);
		return;
	}
	else
	{
		// solo strategy
		if (!me->IsAlive())
		{
			if (repopDelay > 0)
			{
				repopDelay -= pDiff;
				if (repopDelay <= 0)
				{
					if (!me->GetCorpse())
					{
						me->SetBotDeathTimer();
						me->BuildPlayerRepop();
						WorldLocation loc;
						Corpse* corpse = me->GetCorpse();
						corpse->GetPosition(loc);
						me->TeleportTo(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z, 0.0f);
					}
					reviveDelay = urand(600000, 1800000);
				}
			}
			else if (repopDelay == 0)
			{
				repopDelay = urand(120000, 180000);
			}
			else
			{
				if (reviveDelay > 0)
				{
					reviveDelay -= pDiff;
				}
				else
				{
					me->ResurrectPlayer(1.0f);
					me->SpawnCorpseBones();
					repopDelay = 0;
					relocateDelay = urand(2000, 5000);
				}
			}
			return;
		}
		if (relocateDelay > 0)
		{
			relocateDelay -= pDiff;
		}
		if (me->IsInCombat())
		{
			restDelay = 0;
			Unit* victim = nullptr;
			if (!victim)
			{
				victim = me->GetTarget();
			}
			if (victim)
			{
				if (!victim->IsAlive())
				{
					ClearTarget();
				}
				else if (!me->CanAttack(victim))
				{
					ClearTarget();
				}
			}
			// player attackers 
			bool checkAttackers = false;
			if (victim)
			{
				if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
				{
					checkAttackers = true;
				}
			}
			else
			{
				checkAttackers = true;
			}
			if (checkAttackers)
			{
				std::set<Unit*> attackers = me->getAttackers();
				for (auto itr = attackers.begin(); itr != attackers.end(); ++itr)
				{
					if (Unit* eachAttacker = *itr)
					{
						if (eachAttacker->GetTypeId() == TypeID::TYPEID_PLAYER)
						{
							if (eachAttacker->IsAlive())
							{
								if (me->GetDistance(eachAttacker) < VISIBILITY_DISTANCE_LARGE)
								{
									victim = eachAttacker;
									break;
								}
							}
						}
					}
				}
				if (!victim)
				{
					for (auto itr = attackers.begin(); itr != attackers.end(); ++itr)
					{
						if (Unit* eachAttacker = *itr)
						{
							if (eachAttacker->IsAlive())
							{
								if (me->GetDistance(eachAttacker) < VISIBILITY_DISTANCE_LARGE)
								{
									victim = eachAttacker;
									break;
								}
							}
						}
					}
				}
			}
			if (victim)
			{
				if (PVP(victim))
				{
					return;
				}
			}
		}
		else
		{
			if (Cure())
			{
				return;
			}
			if (Buff())
			{
				return;
			}
			if (Revive())
			{
				return;
			}
			if (Rest())
			{
				return;
			}
			if (relocateDelay <= 0)
			{
				relocateDelay = urand(600000, 1800000);
				Relocate();
				return;
			}
			if (wanderDelay > 0)
			{
				return;
			}
			Unit* victim = me->GetTarget();
			if (victim)
			{
				if (!me->CanAttack(victim))
				{
					ClearTarget();
					victim = nullptr;
				}
			}
			if (!victim)
			{
				std::list<Unit*> enemyList;
				MaNGOS::AnyUnitInObjectRangeCheck unitChecker(me, VISIBILITY_DISTANCE_GIGANTIC);
				MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> unitSearcher(enemyList, unitChecker);
				Cell::VisitAllObjects(me, unitSearcher, VISIBILITY_DISTANCE_GIGANTIC);

				for (Unit* eachEnemy : enemyList)
				{
					if (eachEnemy->IsAlive())
					{
						if (eachEnemy->GetTypeId() == TypeID::TYPEID_PLAYER)
						{
							if (me->CanAttack(eachEnemy))
							{
								victim = eachEnemy;
								break;
							}
						}
					}
				}
			}

			if (PVP(victim))
			{
				return;
			}
			if (Wander())
			{
				return;
			}
		}
	}
}

void NierScript_Base::Relocate()
{
	if (!me->IsAlive())
	{
		return;
	}
	me->ClearInCombat();
	ObjectGuid masterGuid = ObjectGuid(HIGHGUID_PLAYER, masterId);
	if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
	{
		uint32 mapId = 0;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		me->GetHomebindLocation(x, y, z, mapId);
		if (Map* masterMap = master->GetMap())
		{
			if (!masterMap->Instanceable())
			{
				mapId = masterMap->GetId();
				float distance = frand(100.0f, 300.0f);
				float angle = frand(0.0f, M_PI * 2.0f);
				me->GetNearPoint(nullptr, x, y, z, 0.0f, distance, angle);
				master->GetNearPoint(nullptr, x, y, z, 0.0f, distance, angle);
			}
		}
		me->TeleportTo(mapId, x, y, z, 0.0f);
	}
}

void NierScript_Base::InitializeCharacter()
{
	if (!me)
	{
		return;
	}
	ObjectGuid masterGuid = ObjectGuid(HIGHGUID_PLAYER, masterId);
	if (Player* master = ObjectAccessor::FindPlayer(masterGuid))
	{
		if (me->GetLevel() != master->GetLevel())
		{
			me->GiveLevel(master->GetLevel());
		}
	}
	LearnTalents();

	// spells 
	me->LearnDefaultSkills();
	me->learnDefaultSpells();
	me->learnQuestRewardedSpells();
	me->UpdateSkillsForLevel(true);

	if (TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerSpells(sNierManager->trainerMap[me->getClass()]))
	{
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
	if (const CreatureInfo* ci = sObjectMgr.GetCreatureTemplate(sNierManager->trainerMap[me->getClass()]))
	{
		uint32 trainerTemplateId = ci->TrainerTemplateId;

		if (TrainerSpellData const* trainer_template_spells = sObjectMgr.GetNpcTrainerTemplateSpells(trainerTemplateId))
		{
			bool hasNew = false;
			while (true)
			{
				hasNew = false;
				for (TrainerSpellMap::const_iterator itr = trainer_template_spells->spellList.begin(); itr != trainer_template_spells->spellList.end(); ++itr)
				{
					TrainerSpell const* tSpell = &itr->second;
					if (me->HasSpell(tSpell->spell))
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
}

void NierScript_Base::LearnTalents()
{
	me->resetTalents(true);
	me->SendTalentsInfoData(false);
}

void NierScript_Base::InitializeEquipments(bool pReset)
{
	if (pReset)
	{
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

	int requiredLevel = me->GetLevel();
	EuipRandom(EquipmentSlots::EQUIPMENT_SLOT_BACK, InventoryType::INVTYPE_CLOAK, ItemClass::ITEM_CLASS_ARMOR, ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH, requiredLevel);
	EuipRandom(EquipmentSlots::EQUIPMENT_SLOT_NECK, InventoryType::INVTYPE_NECK, ItemClass::ITEM_CLASS_ARMOR, ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC, requiredLevel);
	EuipRandom(EquipmentSlots::EQUIPMENT_SLOT_FINGER1, InventoryType::INVTYPE_NECK, ItemClass::ITEM_CLASS_ARMOR, ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC, requiredLevel);
	EuipRandom(EquipmentSlots::EQUIPMENT_SLOT_FINGER2, InventoryType::INVTYPE_NECK, ItemClass::ITEM_CLASS_ARMOR, ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISC, requiredLevel);
}

bool NierScript_Base::EuipRandom(uint32 pmEquipSlot, uint32 pmInventoryType, uint32 pmItemClass, uint32 pmItemSubClass, uint32 pmMaxLevel)
{
	if (Item* currentEquip = me->GetItemByPos(INVENTORY_SLOT_BAG_0, pmEquipSlot))
	{
		return false;
	}
	int minLevel = pmMaxLevel - 5;
	if (minLevel < 1)
	{
		minLevel = 1;
	}
	std::ostringstream itemQueryStream;
	itemQueryStream << "SELECT entry FROM item_template where class = " << pmItemClass << " and subclass = " << pmItemSubClass << " and InventoryType = " << pmInventoryType << " and RequiredSkill = 0 and RequiredReputationFaction = 0 and RequiredLevel >= " << minLevel << " and RequiredLevel <= " << pmMaxLevel << " order by rand() ";
	auto itemQR = WorldDatabase.Query(itemQueryStream.str().c_str());
	if (itemQR)
	{
		do
		{
			Field* fields = itemQR->Fetch();
			uint32 itemEntry = fields[0].GetUInt32();

			if (Item* pItem = Item::CreateItem(itemEntry, 1))
			{
				uint16 dest = 0;
				me->CombatStopWithPets(true, true);
				if (me->CanEquipItem(pmEquipSlot, dest, pItem, false) == InventoryResult::EQUIP_ERR_OK)
				{
					me->EquipItem(dest, pItem, true);
					std::ostringstream msgStream;
					msgStream << me->GetName() << " Equiped " << pItem->GetProto()->Name1;
					//sWorld.SendServerMessage(ServerMessageType::SERVER_MSG_CUSTOM, msgStream.str().c_str());
					sLog.outBasic(msgStream.str().c_str());
					return true;
				}
			}
		} while (itemQR->NextRow());
	}
	return false;
}

bool NierScript_Base::Tank(Unit* pTarget)
{
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
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool NierScript_Base::Heal(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	else if (!pTarget->IsAlive())
	{
		return false;
	}

	return true;
}

bool NierScript_Base::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
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
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}
	else
	{
		// ranged 
		if (targetDistance < dpsDistance)
		{
			if (pTarget->IsWithinLOS(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
			{
				if (!me->isInFront(pTarget, dpsDistance, M_PI / 4))
				{
					me->GetMotionMaster()->Clear(true);
					me->SetFacingToObject(pTarget);
					return true;
				}
				else if (me->IsMoving())
				{
					me->UpdateSplinePosition();
					me->movespline->_Finalize();
					me->GetMotionMaster()->Clear(true);
				}
				return true;
			}
		}

		bool adjust = false;
		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance > dpsDistance)
		{
			adjust = true;
		}
		else if (!pTarget->IsWithinLOS(destination.x, destination.y, destination.z))
		{
			adjust = true;
		}

		if (adjust)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, pTarget->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool NierScript_Base::PVP(Unit* pTarget)
{
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
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		ClearTarget();
		return false;
	}
	ChooseTarget(pTarget);
	if (dpsDistance < ATTACK_DISTANCE)
	{
		// melee 
		float meReach = me->GetCombatReach() + MELEE_LEEWAY / 2.0f;
		float minDistance = meReach / 5.0f;
		if (minDistance < 0.5f)
		{
			minDistance = 0.5f;
		}

		if (targetDistance > minDistance && targetDistance < meReach)
		{
			if (!me->isInFront(pTarget, INTERACTION_DISTANCE, M_PI / 4))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pTarget);
				return true;
			}
			else if (me->IsMoving())
			{
				me->UpdateSplinePosition();
				me->movespline->_Finalize();
				me->GetMotionMaster()->Clear(true);
			}
			return true;
		}

		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance < minDistance || destDistance > meReach)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, me->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}
	else
	{
		// ranged 
		if (targetDistance < dpsDistance)
		{
			if (pTarget->IsWithinLOS(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
			{
				if (!me->isInFront(pTarget, dpsDistance, M_PI / 4))
				{
					me->GetMotionMaster()->Clear(true);
					me->SetFacingToObject(pTarget);
					return true;
				}
				else if (me->IsMoving())
				{
					me->UpdateSplinePosition();
					me->movespline->_Finalize();
					me->GetMotionMaster()->Clear(true);
				}
				return true;
			}
		}

		bool adjust = false;
		// destination check 
		float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
		if (destDistance > dpsDistance)
		{
			adjust = true;
		}
		else if (!pTarget->IsWithinLOS(destination.x, destination.y, destination.z))
		{
			adjust = true;
		}

		if (adjust)
		{
			float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
			angle = frand(angle - 0.1f, angle + 0.1f);
			pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle, pTarget->IsInWater());
			MoveTo(destination, pTarget);
			return true;
		}

		if (!me->IsMoving())
		{
			MoveTo(destination, pTarget);
			return true;
		}
	}

	return true;
}

bool NierScript_Base::Interrupt(Unit* pTarget)
{
	return false;
}

bool NierScript_Base::Buff()
{
	return false;
}

bool NierScript_Base::Cure()
{
	return false;
}

bool NierScript_Base::Revive()
{
	return false;
}

void NierScript_Base::MoveTo(Position pDestination, Unit* pTarget, uint32 pMoveType)
{
	if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
	{
		me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
	}
	me->m_movementInfo.RemoveMovementFlag(MovementFlags::MOVEFLAG_SPLINE_ELEVATION);
	float runSpeed = me->GetSpeed((UnitMoveType)pMoveType);

	Movement::MoveSplineInit init(*me);
	init.SetWalk(false);
	init.SetVelocity(runSpeed);
	if (pTarget)
	{
		init.SetFacing(pTarget);
	}
	init.MoveTo(pDestination.x, pDestination.y, pDestination.z, true);
	init.Launch();

	//me->GetMotionMaster()->MovePoint(0, pDestination, ForcedMovement::FORCED_MOVEMENT_RUN, runSpeed);
}

bool NierScript_Base::Follow(Unit* pTarget)
{
	if (!pTarget)
	{
		return false;
	}
	float targetDistance = sNierManager->GetExactDistance(me->GetPosition(), pTarget->GetPosition());
	targetDistance = targetDistance - pTarget->GetObjectBoundingRadius();
	if (targetDistance > VISIBILITY_DISTANCE_NORMAL)
	{
		return false;
	}
	//ChooseTarget(pTarget);
	if (targetDistance < followDistance)
	{
		if (me->IsMoving())
		{
			me->UpdateSplinePosition();
			me->movespline->_Finalize();
			me->GetMotionMaster()->Clear(true);
			return true;
		}
		return false;
	}

	// destination check 
	float destDistance = sNierManager->GetExactDistance(pTarget->GetPosition(), destination);
	if (destDistance > followDistance)
	{
		float angle = pTarget->GetAngle(me->GetPositionX(), me->GetPositionY());
		angle = frand(angle - 0.1f, angle + 0.1f);
		pTarget->GetNearPoint(pTarget, destination.x, destination.y, destination.z, 0.0f, CONTACT_DISTANCE, angle);
		MoveTo(destination, pTarget);
		return true;
	}

	if (!me->IsMoving())
	{
		MoveTo(destination, pTarget);
	}

	return true;
}

bool NierScript_Base::Wander()
{
	if (wanderDelay > 0)
	{
		return false;
	}
	else
	{
		float distance = frand(5.0f, 30.0f);
		float angle = frand(0.0f, M_PI * 2.0f);
		me->GetNearPoint(me, destination.x, destination.y, destination.z, 0.0f, distance, angle);
		MoveTo(destination, nullptr, UnitMoveType::MOVE_WALK);
		wanderDelay = urand(10000, 30000);
	}
	return true;
}

bool NierScript_Base::UseItem(Item* pmItem, Unit* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}

	if (sNierManager->SpellCasting(me))
	{
		return false;
	}

	if (const ItemPrototype* proto = pmItem->GetProto())
	{
		//ChooseTarget(pmTarget);
		SpellCastTargets targets;
		targets.Update(pmTarget);
		uint32 spellId = 0;
		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			if (proto->Spells[i].SpellId > 0)
			{
				spellId = proto->Spells[i].SpellId;
				break;
			}
		}
		me->CastItemUseSpell(pmItem, targets, 1, 0, spellId);
		return true;
	}

	return false;
}

bool NierScript_Base::UseItem(Item* pmItem, Item* pmTarget)
{
	if (!me)
	{
		return false;
	}
	if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
	{
		return false;
	}
	if (sNierManager->SpellCasting(me))
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
		uint32 spellId = 0;
		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			if (proto->Spells[i].SpellId > 0)
			{
				spellId = proto->Spells[i].SpellId;
				break;
			}
		}
		me->CastItemUseSpell(pmItem, targets, 1, 0, spellId);
		return true;
	}

	return false;
}

bool NierScript_Base::CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura, bool pmOnlyMyAura, uint32 pmMaxAuraStack, bool pmClearShapeShift)
{
	if (!SpellValid(pmSpellId))
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (sNierManager->SpellCasting(me))
	{
		return true;
	}
	if (pmClearShapeShift)
	{
		me->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);
	}
	if (const SpellEntry* pS = sSpellTemplate.LookupEntry<SpellEntry>(pmSpellId))
	{
		if (pmTarget)
		{
			if (!me->IsWithinLOSInMap(pmTarget))
			{
				return false;
			}
			if (pmTarget->IsImmuneToSpell(pS, false, 0, me))
			{
				return false;
			}
			bool valid = true;
			if (pmCheckAura)
			{
				for (uint8 effIndex = 0; effIndex < MAX_EFFECT_INDEX; ++effIndex)
				{
					Unit::AuraList const& auras = pmTarget->GetAurasByType(AuraType(pS->EffectApplyAuraName[effIndex]));
					for (Unit::AuraList::const_iterator it = auras.begin(); it != auras.end(); ++it)
					{
						if (Aura* eachAura = *it)
						{
							if (eachAura->GetSpellProto()->Id == pmSpellId)
							{
								if (pmOnlyMyAura)
								{
									if (eachAura->GetCasterGuid() != me->GetObjectGuid())
									{
										continue;
									}
								}
								if (eachAura->GetStackAmount() >= pmMaxAuraStack)
								{
									valid = false;
								}
								break;
							}
						}
					}
					if (!valid)
					{
						break;
					}
				}
				if (!valid)
				{
					return false;
				}
			}
			if (!me->isInFront(pmTarget, VISIBILITY_DISTANCE_NORMAL, M_PI / 2))
			{
				me->GetMotionMaster()->Clear(true);
				me->SetFacingToObject(pmTarget);
				return true;
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
		if (SpellRangeEntry const* spellRange = sSpellRangeStore.LookupEntry(pS->rangeIndex))
		{
			float maxRange = GetSpellMaxRange(spellRange);
			float targetDistance = me->GetDistance(pmTarget);
			if (targetDistance < maxRange)
			{
				if (me->IsMoving())
				{
					me->StopMoving(true);
					me->GetMotionMaster()->Clear(true);
				}
			}
		}
		if (me->getStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
		{
			me->SetStandState(UNIT_STAND_STATE_STAND);
		}

		SpellCastResult scr = me->CastSpell(pmTarget, pS->Id, TriggerCastFlags::TRIGGERED_NONE);
		if (scr == SpellCastResult::SPELL_CAST_OK)
		{
			return true;
		}
	}

	return false;
}

void NierScript_Base::CancelAura(uint32 pmSpellID)
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

void NierScript_Base::LearnTalent(uint32 pmTalentId, uint32 pmMaxRank)
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

bool NierScript_Base::Rest(bool pForce)
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
	float hpPercent = me->GetHealthPercent();
	float manaPercent = me->GetPower(Powers::POWER_MANA) * 100.0f / me->GetMaxPower(Powers::POWER_MANA);
	if (hpPercent < 60.0f || manaPercent < 60.0f || pForce)
	{
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

		Item* pFood = me->GetItemByEntry(foodEntry);
		if (pFood && !pFood->IsInTrade())
		{
			if (UseItem(pFood, me))
			{
				restDelay = 20000;
			}
		}
	}

	if (restDelay > 0)
	{
		drinkDelay = 1000;
		return true;
	}

	return false;
}

bool NierScript_Base::Drink()
{
	uint32 drinkEntry = 0;
	uint32 myLevel = me->GetLevel();
	if (myLevel >= 75)
	{
		drinkEntry = 33445;
	}
	else if (myLevel >= 70)
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
	Item* pDrink = me->GetItemByEntry(drinkEntry);
	if (pDrink && !pDrink->IsInTrade())
	{
		if (UseItem(pDrink, me))
		{
			return true;
		}
	}

	return false;
}

void NierScript_Base::ChooseTarget(Unit* pmTarget)
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

void NierScript_Base::ClearTarget()
{
	if (me)
	{
		me->AttackStop(true, true, true, true);
		//me->CastStop();
		InterruptSpells();
		me->SetSelectionGuid(ObjectGuid());
		me->SetTarget(nullptr);
		if (Pet* myPet = me->GetPet())
		{
			myPet->AttackStop(true, true, true, true);
			if (CharmInfo* pci = myPet->GetCharmInfo())
			{
				pci->SetCommandState(COMMAND_FOLLOW);
			}
		}
		me->GetMotionMaster()->Clear(true);
	}
}

void NierScript_Base::InterruptSpells()
{
	if (me)
	{
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		if (Spell* spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL))
		{
			WorldPacket wp(CMSG_CANCEL_CAST, 5);
			wp << uint8(0);
			wp << spell->m_spellInfo->Id;
			me->GetSession()->HandleCancelCastOpcode(wp);
		}
		me->SetStandState(MAX_UNIT_STAND_STATE);
	}
}

bool NierScript_Base::SpellValid(uint32 pmSpellID)
{
	if (pmSpellID == 0)
	{
		return false;
	}
	if (!me)
	{
		return false;
	}
	if (!me->IsSpellReady(pmSpellID))
	{
		return false;
	}

	return true;
}
