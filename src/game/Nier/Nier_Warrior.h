#ifndef NIER_WARRIOR_H
#define NIER_WARRIOR_H

#include "Nier_Base.h"

class Nier_Warrior :public Nier_Base
{
public:
    Nier_Warrior();

    void Prepare();
    void Update(uint32 pDiff);
    void Update_Online(uint32 pDiff);
    void InitializeCharacter();

    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    bool Buff();
    bool Cure();
    bool Revive();
};
#endif
