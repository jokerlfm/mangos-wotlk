#ifndef NIER_ACTION_BASE_H
#define NIER_ACTION_BASE_H

#ifndef DEFAULT_ACTION_LIMIT_DELAY
#define DEFAULT_ACTION_LIMIT_DELAY 5000
#endif

#ifndef DEFAULT_MOVEMENT_UPDATE_DELAY
#define DEFAULT_MOVEMENT_UPDATE_DELAY 500
#endif

#include "Nier/NierConfig.h"
#include "Nier/NierManager.h"

enum NierMovementType :uint32
{
	NierMovementType_None = 0,
	NierMovementType_Point,
	NierMovementType_Tank,
	NierMovementType_Chase,
	NierMovementType_Follow,
};

enum NierMovementDirection :uint32
{
	NierMovementDirection_Forward = 0,
	NierMovementDirection_Back,
	NierMovementDirection_Left,
	NierMovementDirection_Right,
};

class NierMovement
{
public:
	NierMovement(Player* pmMe);

public:
	Player* me;
};

class NierAction_Base
{
public:
	NierAction_Base(Player* pmMe);
	void InitializeCharacter(uint32 targetLevel, uint32 targetSpecialty);

	Player* me;
};
#endif
