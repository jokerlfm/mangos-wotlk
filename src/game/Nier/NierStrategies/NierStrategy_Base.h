#ifndef NIER_STRATEGY_H
#define NIER_STRATEGY_H

#include "Nier/NierConfig.h"

enum GroupRole :uint32
{
	GroupRole_DPS = 0,
	GroupRole_Tank = 1,
	GroupRole_Healer = 2,
};

class NierStrategy_Base
{
public:
	NierStrategy_Base(Player* pmMe);
	void Update(uint32 diff);

public:
	Player* me;
};
#endif
