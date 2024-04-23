#ifndef NIERSCRIPT_WARLOCK_H
#define NIERSCRIPT_WARLOCK_H

#include "NierScript_Base.h"

class NierScript_Warlock :public NierScript_Base
{
public:
    NierScript_Warlock(Player* pMe);

    bool Prepare();
    void InitializeCharacter();
    void LearnTalents();
    void InitializeEquipments(bool pReset);

    bool DPS(Unit* pTarget);
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
