#include "NierStrategy_Base.h"
#include "Nier/NierManager.h"
#include "Groups/Group.h"
#include "Grids/GridNotifiers.h"

NierStrategy_Base::NierStrategy_Base()
{
	rushing = false;
}

void NierStrategy_Base::UpdatePartners(uint32 pDiff, Player* pMaster, std::unordered_set<Nier_Base*> pNierSet)
{
	Player* tank = nullptr;
	if (pMaster->groupRole == GroupRole::GroupRole_Tank)
	{
		tank = pMaster;
	}
	for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
	{
		if (Nier_Base* eachNier = *nit)
		{
			if (eachNier->entityState == NierState::NierState_Online)
			{
				if (Player* eachNierPlayer = eachNier->me)
				{
					if (!eachNierPlayer->isNier)
					{
						continue;
					}
					if (!eachNierPlayer->IsInGroup(pMaster))
					{
						continue;
					}
					else
					{
						if (eachNierPlayer->groupRole == GroupRole::GroupRole_Tank) {
							tank = eachNierPlayer;
						}
					}
				}
			}
		}
	}
	if (Group* group = pMaster->GetGroup())
	{
		bool groupInCombat = false;
		for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
		{
			if (Nier_Base* eachNier = *nit)
			{
				if (eachNier->entityState == NierState::NierState_Online)
				{
					if (Player* eachNierPlayer = eachNier->me)
					{
						if (!eachNierPlayer->isNier)
						{
							continue;
						}
						if (!eachNierPlayer->IsInWorld())
						{
							continue;
						}
						if (!eachNierPlayer->IsAlive())
						{
							continue;
						}
						if (!eachNierPlayer->IsInGroup(pMaster))
						{
							continue;
						}
						if (!eachNierPlayer->IsInCombat())
						{
							continue;
						}
						if (eachNierPlayer->GetDistance(pMaster) < VISIBILITY_DISTANCE_NORMAL)
						{
							groupInCombat = true;
							break;
						}
					}
				}
			}
		}
		if (groupInCombat)
		{
			// get nearby enemies 
			std::list<Unit*> enemyList;
			enemyList.clear();
			if (tank)
			{
				MaNGOS::AnyUnitInObjectRangeCheck unitChecker(tank, ATTACK_DISTANCE);
				MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> unitSearcher(enemyList, unitChecker);
				Cell::VisitAllObjects(tank, unitSearcher, ATTACK_DISTANCE);
			}
			Unit* skull = ObjectAccessor::GetUnit(*pMaster, group->GetGuidByTargetIcon(7));
			if (skull)
			{
				if (!skull->IsAlive())
				{
					group->SetTargetIcon(7, pMaster->GetObjectGuid(), ObjectGuid());
					skull = nullptr;
				}
			}
			// skull casting 
			if (skull)
			{
				bool casting = false;
				if (Spell* spell = skull->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
				{
					if (spell->IsInterruptible())
					{
						casting = true;
					}
				}
				if (Spell* spell = skull->GetCurrentSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL))
				{
					if (spell->IsInterruptible())
					{
						casting = true;
					}
				}
				if (casting)
				{
					Unit* interrupter = nullptr;
					for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
					{
						if (Nier_Base* eachNier = *nit)
						{
							if (eachNier->entityState == NierState::NierState_Online)
							{
								if (eachNier->orderDelay > 0)
								{
									continue;
								}
								if (eachNier->actionDelay > 0)
								{
									continue;
								}
								if (eachNier->interruptDelay > 0)
								{
									continue;
								}
								if (eachNier->freezing)
								{
									continue;
								}
								if (Player* eachNierPlayer = eachNier->me)
								{
									if (!eachNierPlayer->isNier)
									{
										continue;
									}
									if (!eachNierPlayer->IsInWorld())
									{
										continue;
									}
									if (!eachNierPlayer->IsAlive() || eachNierPlayer->hasUnitState(UnitState::UNIT_STAT_STUNNED))
									{
										continue;
									}
									if (sNierManager->SpellCasting(eachNierPlayer))
									{
										continue;
									}
									if (eachNierPlayer->IsInGroup(pMaster))
									{
										if (eachNier->Interrupt(skull))
										{
											eachNier->actionDelay = 200;
											interrupter = eachNierPlayer;
											break;
										}
									}
								}
							}
						}
					}
					if (interrupter)
					{
						for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
						{
							if (Nier_Base* eachNier = *nit)
							{
								if (eachNier->entityState == NierState::NierState_Online)
								{
									if (Player* eachNierPlayer = eachNier->me)
									{
										if (eachNierPlayer->GetObjectGuid() != interrupter->GetObjectGuid())
										{
											eachNier->interruptDelay = 250;
										}
									}
								}
							}
						}
					}
				}
			}
			for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
			{
				if (Nier_Base* eachNier = *nit)
				{
					if (eachNier->entityState == NierState::NierState_Online)
					{
						if (eachNier->orderDelay > 0)
						{
							continue;
						}
						if (eachNier->actionDelay > 0)
						{
							continue;
						}
						if (eachNier->freezing)
						{
							continue;
						}
						if (Player* eachNierPlayer = eachNier->me)
						{
							if (!eachNierPlayer->isNier)
							{
								continue;
							}
							if (!eachNierPlayer->IsInWorld())
							{
								continue;
							}
							if (!eachNierPlayer->IsAlive() || eachNierPlayer->hasUnitState(UnitState::UNIT_STAT_STUNNED))
							{
								continue;
							}
							if (sNierManager->SpellCasting(eachNierPlayer))
							{
								continue;
							}
							if (eachNierPlayer->IsInGroup(pMaster))
							{
								if (eachNierPlayer->groupRole == GroupRole::GroupRole_Tank)
								{
									if (Unit* eachTarget = eachNierPlayer->GetTarget())
									{
										if (!eachTarget->IsAlive())
										{
											eachNier->ClearTarget();
											continue;
										}
										else if (!eachNierPlayer->CanAttack(eachTarget))
										{
											eachNier->ClearTarget();
											continue;
										}
									}
									// set a skull 
									if (!skull)
									{
										for (Unit* eachEnemy : enemyList)
										{
											if (eachEnemy->IsAlive())
											{
												if (eachNierPlayer->CanAttack(eachEnemy))
												{
													group->SetTargetIcon(7, eachNierPlayer->GetObjectGuid(), eachEnemy->GetObjectGuid());
													skull = eachEnemy;
													break;
												}
											}
										}
									}
									// skull ot 
									if (skull)
									{
										if (skull->GetSelectionGuid() != eachNierPlayer->GetObjectGuid())
										{
											if (eachNier->Tank(skull))
											{
												continue;
											}
										}
									}
									// others ot 
									bool tanked = false;
									for (Unit* eachEnemy : enemyList)
									{
										if (eachEnemy->GetSelectionGuid() != eachNierPlayer->GetObjectGuid())
										{
											if (eachNier->Tank(eachEnemy))
											{
												tanked = true;
												break;
											}
										}
									}
									if (tanked)
									{
										continue;
									}
									// skull threat 
									if (skull)
									{
										if (eachNier->Threating(skull))
										{
											if (eachNier->Tank(skull))
											{
												continue;
											}
										}
									}
									// others threat 
									for (Unit* eachEnemy : enemyList)
									{
										if (eachNier->Threating(eachEnemy))
										{
											if (eachNier->Tank(eachEnemy))
											{
												tanked = true;
												break;
											}
										}
									}
									if (tanked)
									{
										continue;
									}
									// tank skull 
									if (eachNier->Tank(skull))
									{
										continue;
									}
								}
								else if (eachNierPlayer->groupRole == GroupRole::GroupRole_Healer)
								{
									if (Unit* eachTarget = eachNierPlayer->GetTarget())
									{
										if (!eachTarget->IsAlive())
										{
											eachNier->ClearTarget();
											continue;
										}
									}
									if (eachNier->Heal(tank))
									{
										continue;
									}
									else if (eachNier->Heal(pMaster))
									{
										continue;
									}
									else
									{
										for (std::unordered_set<Nier_Base*>::iterator tit = pNierSet.begin(); tit != pNierSet.end(); tit++)
										{
											if (Nier_Base* eachTargetNier = *tit)
											{
												if (eachNier->Heal(eachTargetNier->me))
												{
													continue;
												}
											}
										}
									}
									//if (eachNier->DPS(skull, tank, rushing))
									//{
									//    continue;
									//}
								}
								else if (eachNierPlayer->groupRole == GroupRole::GroupRole_DPS)
								{
									if (Unit* eachTarget = eachNierPlayer->GetTarget())
									{
										if (!eachTarget->IsAlive())
										{
											eachNier->ClearTarget();
											continue;
										}
										else if (!eachNierPlayer->CanAttack(eachTarget))
										{
											eachNier->ClearTarget();
											continue;
										}
									}
									if (eachNier->DPS(skull, tank, rushing))
									{
										continue;
									}
								}
								eachNier->Follow(pMaster);
							}
						}
					}
				}
			}
		}
		else
		{
			rushing = false;
			for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
			{
				if (Nier_Base* eachNier = *nit)
				{
					if (eachNier->entityState == NierState::NierState_Online)
					{
						if (eachNier->orderDelay > 0)
						{
							continue;
						}
						if (eachNier->actionDelay > 0)
						{
							continue;
						}
						if (eachNier->freezing)
						{
							continue;
						}
						if (Player* eachNierPlayer = eachNier->me)
						{
							if (!eachNierPlayer->isNier)
							{
								continue;
							}
							if (!eachNierPlayer->IsInWorld())
							{
								continue;
							}
							if (!eachNierPlayer->IsAlive() || eachNierPlayer->hasUnitState(UnitState::UNIT_STAT_STUNNED))
							{
								continue;
							}
							if (sNierManager->SpellCasting(eachNierPlayer))
							{
								continue;
							}

							if (eachNierPlayer->IsInGroup(pMaster))
							{
								if (eachNier->restDelay > 0)
								{
									continue;
								}
								if (eachNier->Cure())
								{
									continue;
								}
								if (eachNier->Buff())
								{
									continue;
								}
								if (eachNier->Revive())
								{
									continue;
								}
								if (eachNier->Rest())
								{
									continue;
								}
								eachNier->Follow(pMaster);
							}
						}
					}
				}
			}
		}
	}
	// solo update 
	for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
	{
		if (Nier_Base* eachNier = *nit)
		{
			if (eachNier->entityState == NierState::NierState_Online)
			{
				if (eachNier->orderDelay > 0)
				{
					continue;
				}
				if (Player* eachNierPlayer = eachNier->me)
				{
					if (!eachNierPlayer->isNier)
					{
						continue;
					}
					if (!eachNierPlayer->IsInWorld())
					{
						continue;
					}
					if (!eachNierPlayer->IsAlive() || eachNierPlayer->hasUnitState(UnitState::UNIT_STAT_STUNNED))
					{
						continue;
					}
					if (!eachNierPlayer->IsInGroup(pMaster))
					{
						if (eachNier->Wander())
						{
							continue;
						}
					}
				}
			}
		}
	}
}

void NierStrategy_Base::UpdateRivals(uint32 pDiff, Player* pMaster, std::unordered_set<Nier_Base*> pNierSet)
{
	for (std::unordered_set<Nier_Base*>::iterator nit = pNierSet.begin(); nit != pNierSet.end(); nit++)
	{
		if (Nier_Base* eachNier = *nit)
		{
			if (eachNier->entityState == NierState::NierState_Online)
			{
				if (Player* eachNierPlayer = eachNier->me)
				{
					if (!eachNierPlayer->isNier)
					{
						continue;
					}
					if (!eachNierPlayer->IsAlive())
					{
						if (eachNier->reviveDelay > 0)
						{

						}
						else
						{
							eachNier->reviveDelay = urand(900000, 1800000);
						}
						continue;
					}
					if (eachNierPlayer->IsInCombat())
					{
						eachNier->restDelay = 0;
						Unit* victim = nullptr;
						if (!victim)
						{
							victim = eachNierPlayer->GetTarget();
						}
						if (victim)
						{
							if (!victim->IsAlive())
							{
								eachNier->ClearTarget();
								continue;
							}
							else if (!eachNierPlayer->CanAttack(victim))
							{
								eachNier->ClearTarget();
								continue;
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
							std::set<Unit*> attackers = eachNierPlayer->getAttackers();
							for (auto itr = attackers.begin(); itr != attackers.end(); ++itr)
							{
								if (Unit* eachAttacker = *itr)
								{
									if (eachAttacker->GetTypeId() == TypeID::TYPEID_PLAYER)
									{
										if (eachAttacker->IsAlive())
										{
											if (eachNierPlayer->GetDistance(eachAttacker) < VISIBILITY_DISTANCE_LARGE)
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
											if (eachNierPlayer->GetDistance(eachAttacker) < VISIBILITY_DISTANCE_LARGE)
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
							if (eachNier->PVP(victim))
							{
								continue;
							}
						}
					}
					else
					{
						if (eachNier->restDelay > 0)
						{
							continue;
						}
						if (eachNier->Cure())
						{
							continue;
						}
						if (eachNier->Buff())
						{
							continue;
						}
						if (eachNier->Revive())
						{
							continue;
						}
						if (eachNier->Rest())
						{
							continue;
						}
						if (eachNier->pvp)
						{
							bool relocate = false;
							if (pMaster->GetMapId() == 0 || pMaster->GetMapId() == 1)
							{
								if (!eachNierPlayer->IsInMap(pMaster))
								{
									relocate = true;
								}
								if (eachNierPlayer->GetDistance(pMaster) > VISIBILITY_DISTANCE_GIGANTIC)
								{
									relocate = true;
								}
							}
							if (relocate)
							{
								Position dest;
								float dist = frand(VISIBILITY_DISTANCE_NORMAL, VISIBILITY_DISTANCE_LARGE);
								float angle = frand(0.0f, M_PI * 2);
								pMaster->GetNearPoint(pMaster, dest.x, dest.y, dest.z, 0.0f, dist, angle);
								eachNierPlayer->TeleportTo(pMaster->GetMapId(), dest.x, dest.y, dest.z, 0.0f);
								eachNier->restDelay = 10000;
								continue;
							}

							Unit* victim = nullptr;
							if (!victim)
							{
								victim = eachNierPlayer->GetTarget();
							}
							if (!victim)
							{
								std::list<Unit*> enemyList;
								MaNGOS::AnyUnitInObjectRangeCheck unitChecker(eachNierPlayer, VISIBILITY_DISTANCE_GIGANTIC);
								MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> unitSearcher(enemyList, unitChecker);
								Cell::VisitAllObjects(eachNierPlayer, unitSearcher, VISIBILITY_DISTANCE_GIGANTIC);

								for (Unit* eachEnemy : enemyList)
								{
									if (eachEnemy->IsAlive())
									{
										if (eachEnemy->GetTypeId() == TypeID::TYPEID_PLAYER)
										{
											if (eachNierPlayer->CanAttack(eachEnemy))
											{
												victim = eachEnemy;
												break;
											}
										}
									}
								}
							}

							if (!victim)
							{
								if (eachNier->PVP(victim))
								{
									continue;
								}
							}
						}
						else
						{
							// solo update
							if (eachNier->Wander())
							{
								continue;
							}
						}
					}
				}
			}
		}
	}
}