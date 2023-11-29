#include "NierStrategy_Base.h"

NierStrategy_Base::NierStrategy_Base(Player* pmMe)
{
	me = pmMe;
}

void NierStrategy_Base::Update(uint32 diff)
{
	if (Group* myGroup = me->GetGroup())
	{
		// group nier updates
	}
	else
	{
		// solo nier updates

	}
}
