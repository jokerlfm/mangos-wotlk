#include "Nier_Warlock.h"
#include "NierManager.h"

#include "Groups/Group.h"

Nier_Warlock::Nier_Warlock() :Nier_Base()
{
	spell_Shoot = 0;
	spell_Corruption = 0;
	spell_Curse_Of_Agony = 0;
	spell_SummonImp = 0;

	followDistance = 15.0f;
	dpsDistance = 15.0f;
}

void Nier_Warlock::Prepare()
{
	if (Pet* myPet = me->GetPet())
	{
		for (PetSpellMap::iterator itr = myPet->m_spells.begin(); itr != myPet->m_spells.end(); ++itr)
		{
			if (const SpellEntry* si = sSpellTemplate.LookupEntry<SpellEntry>(itr->first))
			{
				// phase shift exception
				if (itr->first == 4511)
				{
					myPet->ToggleAutocast(itr->first, false);
				}
				else
				{
					myPet->ToggleAutocast(itr->first, true);
				}
			}
		}
	}
}

void Nier_Warlock::InitializeCharacter()
{
	Nier_Base::InitializeCharacter();

	target_specialty = 0;
	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);

	spell_SummonImp = 688;
	spell_Shoot = 5019;

	if (myLevel >= 4)
	{
		spell_Corruption = 172;
	}
	if (myLevel >= 8)
	{
		spell_Curse_Of_Agony = 980;
	}
	if (myLevel >= 14)
	{
		spell_Corruption = 6222;
	}
	if (myLevel >= 18)
	{
		spell_Curse_Of_Agony = 1014;
	}
	if (myLevel >= 24)
	{
		spell_Corruption = 6223;
	}
	if (myLevel >= 28)
	{
		spell_Curse_Of_Agony = 6217;
	}
	if (myLevel >= 34)
	{
		spell_Corruption = 7648;
	}
	if (myLevel >= 38)
	{
		spell_Curse_Of_Agony = 11711;
	}
	if (myLevel >= 44)
	{
		spell_Corruption = 11671;
	}
	if (myLevel >= 48)
	{
		spell_Curse_Of_Agony = 11712;
	}
	if (myLevel >= 54)
	{
		spell_Corruption = 11672;
	}
	if (myLevel >= 58)
	{
		spell_Curse_Of_Agony = 11713;
	}
	if (myLevel >= 60)
	{
		spell_Corruption = 25311;
	}
	if (myLevel >= 65)
	{
		spell_Corruption = 27216;
	}
	if (myLevel >= 67)
	{
		spell_Curse_Of_Agony = 27218;
	}
	if (myLevel >= 71)
	{
		spell_Corruption = 47812;
	}
	if (myLevel >= 73)
	{
		spell_Curse_Of_Agony = 47863;
	}
	if (myLevel >= 77)
	{
		spell_Corruption = 47813;
	}
	if (myLevel >= 79)
	{
		spell_Curse_Of_Agony = 47864;
	}
}

bool Nier_Warlock::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (Nier_Base::DPS(pTarget, pTank, pRushing))
	{
		if (actionDelay > 0)
		{
			return true;
		}
		if (!pTarget)
		{
			ClearTarget();
			return false;
		}
		else if (!pTarget->IsAlive())
		{
			ClearTarget();
			return false;
		}
		else if (!me->CanAttack(pTarget))
		{
			ClearTarget();
			return false;
		}
		float targetDistance = me->GetDistance(pTarget);
		if (targetDistance < dpsDistance)
		{
			if (Pet* myPet = me->GetPet())
			{
				if (myPet->IsAlive())
				{
					if (!myPet->GetVictim())
					{
						if (UnitAI* pai = myPet->AI())
						{
							pai->AttackStart(pTarget);
						}
						if (CharmInfo* ci = myPet->GetCharmInfo())
						{
							if (ci->GetCommandState() != CommandStates::COMMAND_ATTACK)
							{
								ci->SetCommandState(CommandStates::COMMAND_ATTACK);
							}
						}
					}
				}
			}
			if (spell_Corruption > 0)
			{
				if (CastSpell(pTarget, spell_Corruption, true, true))
				{
					return true;
				}
			}
			if (spell_Curse_Of_Agony > 0)
			{
				if (CastSpell(pTarget, spell_Curse_Of_Agony, true, true))
				{
					return true;
				}
			}
			if (spell_Shoot > 0)
			{
				if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
				{
					return true;
				}
				else
				{
					if (CastSpell(pTarget, spell_Shoot))
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

bool Nier_Warlock::Buff()
{
	if (actionDelay > 0)
	{
		return true;
	}
	if (Group* group = me->GetGroup())
	{
		for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (member->GetObjectGuid() == me->GetObjectGuid())
				{
					if (Pet* myPet = me->GetPet())
					{
						return false;
					}
					else
					{
						if (CastSpell(me, spell_SummonImp))
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
		if (Pet* myPet = me->GetPet())
		{
			return false;
		}
		else
		{
			if (CastSpell(me, spell_SummonImp))
			{
				return true;
			}
		}
	}

	return false;
}
