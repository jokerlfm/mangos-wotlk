#ifndef NIERSCRIPT_MAGE_H
#define NIERSCRIPT_MAGE_H

#include "NierScript_Base.h"

class NierScript_Mage :public NierScript_Base
{
public:
    NierScript_Mage(Player* pMe);

    bool Prepare();
    void Update(uint32 pDiff);
    void InitializeCharacter();
    void LearnTalents();
    void InitializeEquipments(bool pReset);

    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget);
    bool PVP(Unit* pTarget);
    bool Buff();
    bool Cure();
    bool Revive();

public:
    uint32 spell_Shoot;
    uint32 spell_FireBlast;
    uint32 spell_ArcaneIntellect;
};
#endif
