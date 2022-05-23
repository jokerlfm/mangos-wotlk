#ifndef NINGER_ACTION_HUNTER_H
#define NINGER_ACTION_HUNTER_H

#include "NingerAction_Base.h"

enum HunterAspectType :uint32
{
    HunterAspectType_Hawk = 0,
    HunterAspectType_Monkey,
    HunterAspectType_Wild,
    HunterAspectType_Pack,
};

class NingerAction_Hunter :public NingerAction_Base
{
public:
    NingerAction_Hunter(Player* me);
    void InitializeCharacter(uint32 pmTargetLevel, uint32 pmSpecialtyTabIndex);
    void ResetTalent();
    bool InitializeEquipments(bool pmReset);
    void Prepare();
    bool DPS(Unit* pmTarget, bool pmRushing, float pmDistanceMax, float pmDistanceMin, bool pmHolding, bool pmInstantOnly, bool pmChasing, bool pmForceBack);
    bool AOE(Unit* pmTarget, bool pmRushing, float pmDistanceMax, float pmDistanceMin, bool pmHolding, bool pmInstantOnly, bool pmChasing);
    bool Buff(Unit* pmTarget);
    bool Petting(bool pmSummon = true, bool pmReset = false);
    void PetStop();
    void PetAttack(Unit* pmTarget);

    uint32 aspectType;

private:
    uint32 spell_ArcaneShot;
    uint32 spell_SerpentSting;
    uint32 spell_ScorpidSting;
    uint32 spell_RapidFire;
    uint32 spell_FeignDeath;
    uint32 spell_HuntersMark;
    uint32 spell_Volley;
    uint32 spell_SteadyShot;
    uint32 spell_AspectOfTheHawk;
    uint32 spell_AspectOfTheViper;
    uint32 spell_CallPet;
    uint32 spell_DismissPet;
    uint32 spell_RevivePet;
    uint32 spell_TameBeast;
    uint32 spell_MendPet;
    uint32 spell_AspectOfTheDragonhawk;
    uint32 spell_ConcussiveShot;
    uint32 spell_AutoShot;
    uint32 spell_Readiness;

    uint32 spell_ScatterShot;
    uint32 spell_TrueshotAura;
    uint32 spell_SilencingShot;
    uint32 spell_ChimeraShot;

    uint32 ammoEntry;
};
#endif
