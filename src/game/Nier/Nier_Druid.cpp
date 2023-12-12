#include "Nier_Druid.h"
#include "NierManager.h"

Nier_Druid::Nier_Druid() :Nier_Base()
{

}

void Nier_Druid::Prepare()
{
    Nier_Base::Prepare();
}

void Nier_Druid::Update(uint32 pDiff)
{
    Nier_Base::Update(pDiff);
}

void Nier_Druid::Update_Online(uint32 pDiff)
{
    Nier_Base::Update_Online(pDiff);
}

void Nier_Druid::InitializeCharacter()
{
    Nier_Base::InitializeCharacter();
}

bool Nier_Druid::Tank(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Druid::Heal(Unit* pTarget)
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

bool Nier_Druid::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
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

bool Nier_Druid::Buff()
{
    if (actionDelay > 0)
    {
        return true;
    }
    return Nier_Base::Buff();
}

bool Nier_Druid::Cure()
{
    return Nier_Base::Cure();
}

bool Nier_Druid::Revive()
{
    return Nier_Base::Revive();
}
