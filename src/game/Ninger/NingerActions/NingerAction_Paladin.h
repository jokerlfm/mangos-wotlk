#ifndef NINGER_ACTION_PALADIN_H
#define NINGER_ACTION_PALADIN_H

#include "NingerAction_Base.h"

enum PaladinAuraType :uint32
{
    PaladinAuraType_Concentration = 0,
    PaladinAuraType_Devotion,
    PaladinAuraType_Retribution,
    PaladinAuraType_FireResistant,
    PaladinAuraType_FrostResistant,
    PaladinAuraType_ShadowResistant,
};

enum PaladinBlessingType :uint32
{
    PaladinBlessingType_Kings = 0,
    PaladinBlessingType_Might = 1,
    PaladinBlessingType_Wisdom = 2,
    PaladinBlessingType_Salvation = 3,
};

enum PaladinSealType :uint32
{
    PaladinSealType_Righteousness = 0,
    PaladinSealType_Justice = 1,
    PaladinSealType_Command = 2,
};

class NingerAction_Paladin :public NingerAction_Base
{
public:
    NingerAction_Paladin();

    uint32 auraType;
    uint32 blessingType;
    uint32 sealType;

    int judgementDelay;
    int hammerOfJusticeDelay;
    int sealDelay;
};
#endif
