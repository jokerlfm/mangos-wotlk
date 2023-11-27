#include "NingerAction_Paladin.h"

NingerAction_Paladin::NingerAction_Paladin(Player* pmMe) :NingerAction_Base(pmMe)
{
    blessingType = PaladinBlessingType::PaladinBlessingType_Might;
    auraType = PaladinAuraType::PaladinAuraType_Retribution;
    sealType = PaladinSealType::PaladinSealType_Righteousness;

    judgementDelay = 0;
    hammerOfJusticeDelay = 0;
    sealDelay = 0;
}
