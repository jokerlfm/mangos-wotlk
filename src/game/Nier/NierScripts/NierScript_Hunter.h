#ifndef NIERSCRIPT_HUNTER_H
#define NIERSCRIPT_HUNTER_H

#include "NierScript_Base.h"

class NierScript_Hunter :public NierScript_Base
{
public:
    NierScript_Hunter(Player* pMe);

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

    uint32 spell_RaptorStrike;
    uint32 spell_MongooseBite;
    uint32 spell_WingClip;
    
    std::unordered_set<uint32> growlSet;
    std::unordered_set<uint32> prowlSet;
};
#endif
