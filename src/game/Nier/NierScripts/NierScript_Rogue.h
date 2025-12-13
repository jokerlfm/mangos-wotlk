#ifndef NIERSCRIPT_ROGUE_H
#define NIERSCRIPT_ROGUE_H

#include "NierScript_Base.h"

class NierScript_Rogue :public NierScript_Base
{
public:
    NierScript_Rogue(Player* pMe);

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
    uint32 spell_Dismantle;
    uint32 spell_Eviscerate;
    uint32 spell_FanofKnives;
    uint32 spell_Sprint;
    uint32 spell_TricksoftheTrade;
    uint32 spell_Kick;
    uint32 spell_Sap;
    uint32 spell_Stealth;
    uint32 spell_SliceandDice;
    uint32 spell_Evasion;
    uint32 spell_KidneyShot;
    uint32 spell_CheapShot;
    uint32 spell_Vanish;
    uint32 spell_Feint;
    uint32 spell_SinisterStrike;
    uint32 spell_DeadlyThrow;

    uint32 spell_BladeFlurry;
    uint32 spell_AdrenalineRush;
    uint32 spell_KillingSpree;
    uint32 spell_Cloak_Of_Shadows;

    uint32 item_InstantPoison;
    uint32 item_SlowPoison;
};
#endif
