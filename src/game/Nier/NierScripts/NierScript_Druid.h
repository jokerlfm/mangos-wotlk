#ifndef NIERSCRIPT_DRUID_H
#define NIERSCRIPT_DRUID_H

#include "NierScript_Base.h"

class NierScript_Druid :public NierScript_Base
{
public:
    NierScript_Druid(Player* pMe);

    bool Prepare();
    void Update(uint32 pDiff);
    void InitializeCharacter();
    void LearnTalents();
    void InitializeEquipments(bool pReset);

    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget);
    bool Buff();
    bool Cure();
    bool Revive();    
};
#endif
