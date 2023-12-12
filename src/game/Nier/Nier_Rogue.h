#ifndef NIER_ROGUE_H
#define NIER_ROGUE_H

#include "Nier_Base.h"

class Nier_Rogue :public Nier_Base
{
public:
    Nier_Rogue();

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
