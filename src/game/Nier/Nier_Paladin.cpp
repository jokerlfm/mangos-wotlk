#include "Nier_Paladin.h"
#include "NierManager.h"

Nier_Paladin::Nier_Paladin() :Nier_Base()
{

}

void Nier_Paladin::Prepare()
{
    Nier_Base::Prepare();
}

void Nier_Paladin::Update(uint32 pDiff)
{
    Nier_Base::Update(pDiff);
}

void Nier_Paladin::Update_Online(uint32 pDiff)
{
    Nier_Base::Update_Online(pDiff);
}

void Nier_Paladin::InitializeCharacter()
{
    Nier_Base::InitializeCharacter();
}

bool Nier_Paladin::Tank(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Paladin::Heal(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Paladin::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
    return Nier_Base::DPS(pTarget, pTank, pRushing);
}

bool Nier_Paladin::Buff()
{
    return Nier_Base::Buff();
}

bool Nier_Paladin::Cure()
{
    return Nier_Base::Cure();
}

bool Nier_Paladin::Revive()
{
    return Nier_Base::Revive();
}
