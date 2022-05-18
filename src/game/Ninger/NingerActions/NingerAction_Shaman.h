#ifndef NINGER_ACTION_SHAMAN_H
#define NINGER_ACTION_SHAMAN_H

#include "NingerAction_Base.h"

enum ShamanEarthTotemType :uint32
{
    ShamanEarthTotemType_EarthbindTotem = 0,
    ShamanEarthTotemType_StoneskinTotem,
    ShamanEarthTotemType_StoneclawTotem,
    ShamanEarthTotemType_StrengthOfEarthTotem
};

class NingerAction_Shaman :public NingerAction_Base
{
public:
    NingerAction_Shaman(Player* pmMe);
    void Prepare();

    uint32 earthTotemType;
};
#endif
