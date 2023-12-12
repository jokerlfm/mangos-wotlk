#ifndef NIER_PALADIN_H
#define NIER_PALADIN_H

#include "Nier_Base.h"

class Nier_Paladin :public Nier_Base
{
public:
    Nier_Paladin();

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

public:
};
#endif
