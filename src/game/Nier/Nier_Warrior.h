#ifndef NIER_WARRIOR_H
#define NIER_WARRIOR_H

#include "Nier_Base.h"

class Nier_Warrior :public Nier_Base
{
public:
    Nier_Warrior();

    bool Prepare();
    void Update(uint32 pDiff);
    void Update_Online(uint32 pDiff);
    void InitializeCharacter();

    bool Threating(Unit* pTarget);
    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    bool Interrupt(Unit* pTarget);
    bool Buff();
    bool Cure();
    bool Revive();

public:
    uint32 spell_SunderArmor;
    uint32 spell_SunderArmorAura;
    uint32 spell_BattleShout;
    uint32 spell_Taunt;
    uint32 spell_ShieldBash;
    uint32 spell_DemoralizingShout;
    uint32 spell_Revenge;
    uint32 spell_ShieldBlock;
    uint32 spell_DefensiveStance;
    uint32 spell_Rend;
    uint32 spell_Disarm;
    uint32 spell_LastStand;
    uint32 spell_ShieldWall;
};
#endif
