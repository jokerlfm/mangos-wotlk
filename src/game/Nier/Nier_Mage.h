#ifndef NIER_MAGE_H
#define NIER_MAGE_H

#include "Nier_Base.h"

class Nier_Mage :public Nier_Base
{
public:
    Nier_Mage();

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
    uint32 spell_Shoot;
    uint32 spell_FireBlast;
    uint32 spell_ArcaneIntellect;
};
#endif
