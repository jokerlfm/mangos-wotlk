#include "NierStrategy_Base.h"
#include "Nier/NierManager.h"
#include "Groups/Group.h"

NierStrategy_Base::NierStrategy_Base()
{
	rushing = false;
}

void NierStrategy_Base::Update(uint32 pDiff, Player* pMaster, std::unordered_set<Nier_Base*> pNierSet)
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
	Unit* skull = nullptr;
	if (Group* group = pMaster->GetGroup())
	{
		skull = ObjectAccessor::GetUnit(*pMaster, group->GetGuidByTargetIcon(7));
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
					if (!eachNierPlayer->IsAlive() || eachNierPlayer->hasUnitState(UnitState::UNIT_STAT_STUNNED))
					{
						continue;
					}
					if (!eachNierPlayer->IsInGroup(pMaster))
					{
						// solo update
						if (eachNier->Wander())
						{
							continue;
						}
						continue;
					}
					if (eachNier->freezing)
					{
						continue;
					}
					if (pMaster->IsInCombat())
					{
						if (eachNierPlayer->groupRole == GroupRole::GroupRole_Tank)
						{

						}
						else if (eachNierPlayer->groupRole == GroupRole::GroupRole_Healer)
						{
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
							if (eachNier->DPS(skull, tank, rushing))
							{
								continue;
							}
						}
					}
					else
					{
						rushing = false;
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
					}
					eachNier->Follow(pMaster);
				}
			}
		}
	}
}
