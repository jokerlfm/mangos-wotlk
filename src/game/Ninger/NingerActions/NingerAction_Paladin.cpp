#include "NingerAction_Paladin.h"

NingerAction_Paladin::NingerAction_Paladin() :NingerAction_Base()
{
    blessingType = PaladinBlessingType::PaladinBlessingType_Might;
    auraType = PaladinAuraType::PaladinAuraType_Retribution;
    sealType = PaladinSealType::PaladinSealType_Righteousness;

    judgementDelay = 0;
    hammerOfJusticeDelay = 0;
    sealDelay = 0;
}
