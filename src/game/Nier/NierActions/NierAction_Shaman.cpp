#include "NingerAction_Shaman.h"

NingerAction_Shaman::NingerAction_Shaman(Player* pmMe) :NingerAction_Base(pmMe)
{
	earthTotemType = ShamanEarthTotemType::ShamanEarthTotemType_StoneskinTotem;
}

void NingerAction_Shaman::Prepare()
{
	if (!me->HasItemCount(5175, 1))
	{
		me->StoreNewItemInBestSlots(5175, 1);
	}
	if (!me->HasItemCount(5176, 1))
	{
		me->StoreNewItemInBestSlots(5176, 1);
	}

	me->Say("Prepared", Language::LANG_UNIVERSAL);
}
