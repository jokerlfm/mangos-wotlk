#include "Nier_Warlock.h"
#include "NierManager.h"

#include "Groups/Group.h"

Nier_Warlock::Nier_Warlock() :Nier_Base()
{
	spell_Shoot = 0;
	spell_Corruption = 0;
	spell_Curse_Of_Agony = 0;
	spell_SummonImp = 0;
	spell_Immolate = 0;
	spell_Shadowbolt = 0;
	spell_RainOfFire = 0;
	spell_FireShield = 0;

	followDistance = 15.0f;
	dpsDistance = 15.0f;
}

bool Nier_Warlock::Prepare()
{
	if (Nier_Base::Prepare())
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
		return true;
	}

	return false;
}

void Nier_Warlock::InitializeCharacter()
{
	target_specialty = 2;
	Nier_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);

	spell_SummonImp = 688;
	spell_Shoot = 5019;
	spell_Immolate = 348;
	spell_Shadowbolt = 686;

	if (myLevel >= 4)
	{
		spell_Corruption = 172;
	}
	if (myLevel >= 6)
	{
		spell_Shadowbolt = 695;
	}
	if (myLevel >= 8)
	{
		spell_Curse_Of_Agony = 980;
	}
	if (myLevel >= 10)
	{
		spell_Immolate = 707;
	}
	if (myLevel >= 12)
	{
		spell_Shadowbolt = 705;
	}
	if (myLevel >= 14)
	{
		spell_Corruption = 6222;
		spell_FireShield = 2947;
	}
	if (myLevel >= 18)
	{
		spell_Curse_Of_Agony = 1014;
	}
	if (myLevel >= 20)
	{
		spell_Immolate = 1094;
		spell_Shadowbolt = 1088;
		spell_RainOfFire = 5740;
	}
	if (myLevel >= 24)
	{
		spell_Corruption = 6223;
		spell_FireShield = 8316;
	}
	if (myLevel >= 28)
	{
		spell_Curse_Of_Agony = 6217;
		spell_Shadowbolt = 1106;
	}
	if (myLevel >= 30)
	{
		spell_Immolate = 2941;
	}
	if (myLevel >= 34)
	{
		spell_Corruption = 7648;
		spell_RainOfFire = 6219;
		spell_FireShield = 8317;
	}
	if (myLevel >= 36)
	{
		spell_Shadowbolt = 7641;
	}
	if (myLevel >= 38)
	{
		spell_Curse_Of_Agony = 11711;
	}
	if (myLevel >= 40)
	{
		spell_Immolate = 11665;
	}
	if (myLevel >= 44)
	{
		spell_Corruption = 11671;
		spell_Shadowbolt = 11659;
		spell_FireShield = 11770;
	}
	if (myLevel >= 46)
	{
		spell_RainOfFire = 11677;
	}
	if (myLevel >= 48)
	{
		spell_Curse_Of_Agony = 11712;
	}
	if (myLevel >= 50)
	{
		spell_Immolate = 11667;
	}
	if (myLevel >= 52)
	{
		spell_Shadowbolt = 11660;
	}
	if (myLevel >= 54)
	{
		spell_Corruption = 11672;
		spell_FireShield = 11771;
	}
	if (myLevel >= 58)
	{
		spell_Curse_Of_Agony = 11713;
		spell_RainOfFire = 11678;
	}
	if (myLevel >= 60)
	{
		spell_Corruption = 25311;
		spell_Shadowbolt = 11661;
		spell_Immolate = 11668;
	}
	if (myLevel >= 61)
	{
		spell_Shadowbolt = 25307;
		spell_Immolate = 25309;
	}
	if (myLevel >= 64)
	{
		spell_FireShield = 27269;
	}
	if (myLevel >= 65)
	{
		spell_Corruption = 27216;
	}
	if (myLevel >= 67)
	{
		spell_Curse_Of_Agony = 27218;
	}
	if (myLevel >= 69)
	{
		spell_Immolate = 27215;
		spell_Shadowbolt = 27209;
		spell_RainOfFire = 27212;
	}
	if (myLevel >= 71)
	{
		spell_Corruption = 47812;
	}
	if (myLevel >= 72)
	{
		spell_RainOfFire = 47819;
	}
	if (myLevel >= 73)
	{
		spell_Curse_Of_Agony = 47863;
	}
	if (myLevel >= 74)
	{
		spell_Shadowbolt = 47808;
	}
	if (myLevel >= 75)
	{
		spell_Immolate = 47810;
	}
	if (myLevel >= 76)
	{
		spell_FireShield = 47983;
	}
	if (myLevel >= 77)
	{
		spell_Corruption = 47813;
	}
	if (myLevel >= 79)
	{
		spell_Curse_Of_Agony = 47864;
		spell_Shadowbolt = 47809;
		spell_RainOfFire = 47820;
	}
	if (myLevel >= 80)
	{
		spell_Immolate = 47811;
	}
	me->UpdateSkillsForLevel(true);
}

bool Nier_Warlock::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (Nier_Base::DPS(pTarget, pTank, pRushing))
	{
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
			if (spell_Immolate > 0)
			{
				if (CastSpell(pTarget, spell_Immolate, true, true))
				{
					return true;
				}
			}
			uint32 healthTimes = pTarget->GetMaxHealth() / me->GetMaxHealth();
			if (healthTimes > 2)
			{
				if (spell_Shadowbolt > 0)
				{
					if (CastSpell(pTarget, spell_Shadowbolt))
					{
						return true;
					}
				}
			}
			else
			{
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
			//if (spell_Corruption > 0)
			//{
			//	if (CastSpell(pTarget, spell_Corruption, true, true))
			//	{
			//		return true;
			//	}
			//}
			//if (spell_Curse_Of_Agony > 0)
			//{
			//	if (CastSpell(pTarget, spell_Curse_Of_Agony, true, true))
			//	{
			//		return true;
			//	}
			//}
		}
		return true;
	}

	return false;
}

bool Nier_Warlock::PVP(Unit* pTarget)
{
	if (Nier_Base::PVP(pTarget))
	{
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
			if (spell_Immolate > 0)
			{
				if (CastSpell(pTarget, spell_Immolate, true, true))
				{
					return true;
				}
			}
			uint32 healthTimes = pTarget->GetMaxHealth() / me->GetMaxHealth();
			if (healthTimes > 2)
			{
				if (spell_Shadowbolt > 0)
				{
					if (CastSpell(pTarget, spell_Shadowbolt))
					{
						return true;
					}
				}
			}
			else
			{
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
			//if (spell_Corruption > 0)
			//{
			//	if (CastSpell(pTarget, spell_Corruption, true, true))
			//	{
			//		return true;
			//	}
			//}
			//if (spell_Curse_Of_Agony > 0)
			//{
			//	if (CastSpell(pTarget, spell_Curse_Of_Agony, true, true))
			//	{
			//		return true;
			//	}
			//}
		}
		return true;
	}

	return false;
}

bool Nier_Warlock::Buff()
{
	if (Pet* myPet = me->GetPet())
	{

	}
	else
	{
		if (CastSpell(me, spell_SummonImp))
		{
			return true;
		}
	}

	return false;
}
