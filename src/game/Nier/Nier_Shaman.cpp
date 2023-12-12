#include "Nier_Shaman.h"
#include "NierManager.h"

Nier_Shaman::Nier_Shaman() :Nier_Base()
{

}

void Nier_Shaman::Prepare()
{
    Nier_Base::Prepare();
}

void Nier_Shaman::Update(uint32 pDiff)
{
    Nier_Base::Update(pDiff);
}

void Nier_Shaman::Update_Online(uint32 pDiff)
{
    Nier_Base::Update_Online(pDiff);
}

void Nier_Shaman::InitializeCharacter()
{
    Nier_Base::InitializeCharacter();
}

bool Nier_Shaman::Tank(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Shaman::Heal(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Shaman::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
    return Nier_Base::DPS(pTarget, pTank, pRushing);
}

bool Nier_Shaman::Buff()
{
    return Nier_Base::Buff();
}

bool Nier_Shaman::Cure()
{
    return Nier_Base::Cure();
}

bool Nier_Shaman::Revive()
{
    return Nier_Base::Revive();
}
