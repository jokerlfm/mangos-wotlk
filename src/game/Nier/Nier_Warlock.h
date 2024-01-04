#ifndef NIER_WARLOCK_H
#define NIER_WARLOCK_H

#include "Nier_Base.h"

class Nier_Warlock :public Nier_Base
{
public:
    Nier_Warlock();
    bool Prepare();
    void InitializeCharacter();
    bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    bool PVP(Unit* pTarget);
    bool Buff();

public:
    uint32 spell_Shoot;
    uint32 spell_SummonImp;
    uint32 spell_Corruption;
    uint32 spell_Curse_Of_Agony;
    uint32 spell_Immolate;
    uint32 spell_Shadowbolt;
    uint32 spell_RainOfFire;
    uint32 spell_FireShield;
};
#endif
