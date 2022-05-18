#ifndef NINGER_ACTION_MAGE_H
#define NINGER_ACTION_MAGE_H

#include "NingerAction_Base.h"

class NingerAction_Mage :public NingerAction_Base
{
public:    
    NingerAction_Mage(Player* pmMe);
    void InitializeCharacter(uint32 pmTargetLevel, uint32 pmSpecialtyTabIndex);
    void ResetTalent();
    bool InitializeEquipments(bool pmReset);
    void Prepare();
    bool DPS(Unit* pmTarget, bool pmRushing, float pmDistanceMax, float pmDistanceMin, bool pmHolding, bool pmInstantOnly, bool pmForceBack);
    bool AOE(Unit* pmTarget, bool pmRushing, float pmDistanceMax, float pmDistanceMin, bool pmHolding, bool pmInstantOnly);
    bool Cure(Unit* pmTarget);
    bool Buff(Unit* pmTarget);
    bool Mark(Unit* pmTarget, int pmRTI);
    bool Assist(int pmRTI);
    uint32 Caution();

public:
    uint32 spell_IcyVeins;
    uint32 spell_ColdSnap;
    uint32 spell_DeepFreeze;    
    uint32 spell_ArcaneIntellect;
    uint32 spell_ArcaneBrilliance;
    uint32 spell_Blizzard;
    uint32 spell_Polymorph;
    uint32 spell_IceArmor;
    uint32 spell_IceLance;
    uint32 spell_FrostNova;
    uint32 spell_Cone_Of_Cold;
    uint32 spell_MageArmor;
    uint32 spell_Frostbolt;
    uint32 spell_IceBlock;
    uint32 spell_Evocation;
    uint32 spell_Fireball;
    uint32 spell_RemoveCurse;
    uint32 spell_MirrorImage;
    uint32 spell_SummonWaterElemental;
    uint32 spell_Blink;
    uint32 spell_FrostfireBolt;
    uint32 spell_ConjureManaGem;

    uint32 spell_Portal_Exodar;
    uint32 spell_Portal_Dalaran;
    uint32 spell_Portal_Orgrimmar;
    uint32 spell_Portal_Stormwind;
    uint32 spell_Portal_Darnassus;
    uint32 spell_Portal_ThunderBluff;
    uint32 spell_Portal_Shattrath_A;
    uint32 spell_Portal_Shattrath_H;
    uint32 spell_Portal_Ironforge;
    uint32 spell_Portal_Silvermoon;
    uint32 spell_Portal_Undercity;

    uint32 aura_Frostbite;
    uint32 aura_Fingers_Of_Frost;
    uint32 aura_Fireball;

    uint32 item_ManaGem;
};
#endif
