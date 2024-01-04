#include "Nier_Mage.h"
#include "NierManager.h"

#include "Groups/Group.h"

Nier_Mage::Nier_Mage() :Nier_Base()
{
	spell_Shoot = 0;
	spell_FireBlast = 0;
	spell_ArcaneIntellect = 0;

	followDistance = 15.0f;
	dpsDistance = 15.0f;
}

bool Nier_Mage::Prepare()
{
	if (Nier_Base::Prepare())
	{

	}

	return false;
}

void Nier_Mage::Update(uint32 pDiff)
{
	Nier_Base::Update(pDiff);
}

void Nier_Mage::Update_Online(uint32 pDiff)
{
	Nier_Base::Update_Online(pDiff);
}

void Nier_Mage::InitializeCharacter()
{
	Nier_Base::InitializeCharacter();

	target_specialty = 2;
	me->groupRole = GroupRole::GroupRole_DPS;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);

	spell_Shoot = 5019;
	spell_ArcaneIntellect = 1459;

	if (myLevel >= 6)
	{
		spell_FireBlast = 2136;
	}
	if (myLevel >= 8)
	{

	}
	if (myLevel >= 14)
	{
		spell_FireBlast = 2137;
		spell_ArcaneIntellect = 1460;
	}
	if (myLevel >= 22)
	{
		spell_FireBlast = 2138;
	}
	if (myLevel >= 28)
	{
		spell_ArcaneIntellect = 1461;
	}
	if (myLevel >= 30)
	{
		spell_FireBlast = 8412;
	}
	if (myLevel >= 38)
	{
		spell_FireBlast = 8413;
	}
	if (myLevel >= 42)
	{
		spell_ArcaneIntellect = 10156;
	}
	if (myLevel >= 46)
	{
		spell_FireBlast = 10197;
	}
	if (myLevel >= 54)
	{
		spell_FireBlast = 10199;
	}
	if (myLevel >= 56)
	{
		spell_ArcaneIntellect = 10157;
	}
	if (myLevel >= 61)
	{
		spell_FireBlast = 27078;
	}
	if (myLevel >= 70)
	{
		spell_FireBlast = 27079;
		spell_ArcaneIntellect = 27126;
	}
	if (myLevel >= 74)
	{
		spell_FireBlast = 42872;
	}
	if (myLevel >= 80)
	{
		spell_FireBlast = 42873;
		spell_ArcaneIntellect = 42995;
	}
	me->UpdateSkillsForLevel(true);
}

bool Nier_Mage::Tank(Unit* pTarget)
{
	return Nier_Base::Tank(pTarget);
}

bool Nier_Mage::Heal(Unit* pTarget)
{
	return false;
}

bool Nier_Mage::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
	if (Nier_Base::DPS(pTarget, pTank, pRushing))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (targetDistance < dpsDistance)
		{
			if (spell_FireBlast > 0)
			{
				if (CastSpell(pTarget, spell_FireBlast))
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

bool Nier_Mage::PVP(Unit* pTarget)
{
	if (Nier_Base::PVP(pTarget))
	{
		float targetDistance = me->GetDistance(pTarget);
		if (targetDistance < dpsDistance)
		{
			if (spell_FireBlast > 0)
			{
				if (CastSpell(pTarget, spell_FireBlast))
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

bool Nier_Mage::Buff()
{
	if (Group* group = me->GetGroup())
	{
		for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
		{
			if (Player* member = groupRef->getSource())
			{
				if (CastSpell(member, spell_ArcaneIntellect, true))
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (CastSpell(me, spell_ArcaneIntellect, true))
		{
			return true;
		}
	}

	return false;
}

bool Nier_Mage::Cure()
{
	return Nier_Base::Cure();
}

bool Nier_Mage::Revive()
{
	return Nier_Base::Revive();
}
