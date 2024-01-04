#ifndef NIER_HUNTER_H
#define NIER_HUNTER_H

#include "Nier_Base.h"

class Nier_Hunter :public Nier_Base
{
public:
    Nier_Hunter();

    bool Prepare();
    void Update(uint32 pDiff);
    void Update_Online(uint32 pDiff);
    void InitializeCharacter();

    bool Tank(Unit* pTarget);
    bool Heal(Unit* pTarget);
    bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    bool PVP(Unit* pTarget);
    bool Buff();
    bool Cure();
    bool Revive();

public:
    uint32 spell_CallPet;
    uint32 spell_RevivePet;
    uint32 spell_TameBeast;
    uint32 spell_DismissPet;
    uint32 spell_MendPet;
    uint32 spell_AutoShot;
    uint32 spell_SerpentSting;
    uint32 spell_ArcaneShot;
    uint32 spell_HuntersMark;
    uint32 spell_ConcussiveShot;
    uint32 spell_AspectOfTheHawk;
    
    std::unordered_set<uint32> growlSet;
    std::unordered_set<uint32> prowlSet;
};
#endif
