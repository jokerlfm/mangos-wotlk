#include "Nier_Hunter.h"
#include "NierManager.h"

Nier_Hunter::Nier_Hunter() :Nier_Base()
{

}

void Nier_Hunter::Prepare()
{
    Nier_Base::Prepare();
}

void Nier_Hunter::Update(uint32 pDiff)
{
    Nier_Base::Update(pDiff);
}

void Nier_Hunter::Update_Online(uint32 pDiff)
{
    Nier_Base::Update_Online(pDiff);
}

void Nier_Hunter::InitializeCharacter()
{
    Nier_Base::InitializeCharacter();
}

bool Nier_Hunter::Tank(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Hunter::Heal(Unit* pTarget)
{
    if (Nier_Base::Heal(pTarget))
    {
        if (actionDelay > 0)
        {
            return true;
        }
    }

    return false;
}

bool Nier_Hunter::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
    if (Nier_Base::DPS(pTarget, pTank, pRushing))
    {
        if (actionDelay > 0)
        {
            return true;
        }
    }

    return false;
}

bool Nier_Hunter::Buff()
{
    if (actionDelay > 0)
    {
        return true;
    }

    return false;
}

bool Nier_Hunter::Cure()
{
    return Nier_Base::Cure();
}

bool Nier_Hunter::Revive()
{
    return Nier_Base::Revive();
}
