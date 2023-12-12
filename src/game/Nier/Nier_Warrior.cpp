#include "Nier_Warrior.h"
#include "NierManager.h"

Nier_Warrior::Nier_Warrior() :Nier_Base()
{

}

void Nier_Warrior::Prepare()
{
    Nier_Base::Prepare();
}

void Nier_Warrior::Update(uint32 pDiff)
{
    Nier_Base::Update(pDiff);
}

void Nier_Warrior::Update_Online(uint32 pDiff)
{
    Nier_Base::Update_Online(pDiff);
}

void Nier_Warrior::InitializeCharacter()
{
    Nier_Base::InitializeCharacter();
}

bool Nier_Warrior::Tank(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Warrior::Heal(Unit* pTarget)
{
    return Nier_Base::Tank(pTarget);
}

bool Nier_Warrior::DPS(Unit* pTarget, Unit* pTank, bool pRushing)
{
    return Nier_Base::DPS(pTarget, pTank, pRushing);
}

bool Nier_Warrior::Buff()
{
    return Nier_Base::Buff();
}

bool Nier_Warrior::Cure()
{
    return Nier_Base::Cure();
}

bool Nier_Warrior::Revive()
{
    return Nier_Base::Revive();
}
