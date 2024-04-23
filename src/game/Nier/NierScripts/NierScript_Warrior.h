#ifndef NIERSCRIPT_WARRIOR_H
#define NIERSCRIPT_WARRIOR_H

#include "NierScript_Base.h"

class NierScript_Warrior :public NierScript_Base
{
public:
    NierScript_Warrior(Player* pMe);

    bool Prepare();
    void Update(uint32 pDiff);
    void InitializeCharacter();
    void LearnTalents();
    void InitializeEquipments(bool pReset);

    uint32 Chase(Unit* pTarget);
    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget);
    bool PVP(Unit* pTarget);
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
    uint32 spell_BattleStance;
    uint32 spell_HeroicStrike;
    uint32 spell_Rend;
    uint32 spell_Disarm;
    uint32 spell_LastStand;
    uint32 spell_ShieldWall;
};
#endif
