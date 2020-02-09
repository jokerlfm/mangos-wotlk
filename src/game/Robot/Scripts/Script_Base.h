#ifndef ROBOT_STRATEGIES_SCRIPT_BASE_H
#define ROBOT_STRATEGIES_SCRIPT_BASE_H

#ifndef AOE_TARGETS_RANGE
# define AOE_TARGETS_RANGE 5
#endif

#include "Robot/RobotAI.h"
#include "Robot/RobotManager.h"
#include "Entities/Player.h"
#include "Spells/SpellMgr.h"
#include "Spells/SpellAuras.h"
#include "Globals/ObjectMgr.h"

class RobotAI;

class Script_Base
{
public:
	Script_Base(RobotAI* pmSourceAI);
	virtual bool DPS(Unit* pmTarget) = 0;
	virtual bool Tank(Unit* pmTarget) = 0;
	virtual bool Healer(Unit* pmTarget) = 0;
	virtual bool Attack(Unit* pmTarget) = 0;
	virtual bool Buff(Unit* pmTarget) = 0;
	virtual bool HealMe() = 0;

	RobotAI* sourceAI;
};
#endif
