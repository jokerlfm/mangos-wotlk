#ifndef NIER_DRUID_H
#define NIER_DRUID_H

#include "Nier_Base.h"

class Nier_Druid :public Nier_Base
{
public:
    Nier_Druid();

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
