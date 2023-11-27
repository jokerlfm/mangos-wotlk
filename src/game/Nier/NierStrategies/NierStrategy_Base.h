#ifndef NINGER_STRATEGY_H
#define NINGER_STRATEGY_H

#ifndef ENTRY_HUNGARFEN
#define ENTRY_HUNGARFEN 17770
#endif

#ifndef ENTRY_HUNGARFEN_1
#define ENTRY_HUNGARFEN_1 20169
#endif

#ifndef ENTRY_UNDERBOG_MUSHROOM
#define ENTRY_UNDERBOG_MUSHROOM 17990
#endif

#ifndef ENTRY_UNDERBOG_MUSHROOM_1
#define ENTRY_UNDERBOG_MUSHROOM_1 20189
#endif

#include "Ninger/NingerConfig.h"

class NingerAction_Base;

enum GroupRole :uint32
{
	GroupRole_DPS = 0,
	GroupRole_Tank = 1,
	GroupRole_Healer = 2,
};

enum StrategyIndex :uint32
{
	StrategyIndex_Base = 0,
	StrategyIndex_The_Underbog = 546,
	StrategyIndex_The_Black_Morass = 269,	
};

enum BasicStrategyType :uint32
{
	BasicStrategyType_Normal = 0,
	BasicStrategyType_Freeze = 1,
	BasicStrategyType_Hold = 2,
	BasicStrategyType_Glue = 3,
};

enum ActionType :uint32
{
	ActionType_None = 0,
	ActionType_Engage = 1,
	ActionType_Revive = 2,
	ActionType_Move = 3,
	ActionType_ReadyTank = 4,
};

class NingerStrategy_Base
{
public:
	NingerStrategy_Base();

	virtual void Report();
	virtual void Reset();
	virtual void Update(uint32 pmDiff);
	virtual bool Engage(Unit* pmTarget);
	virtual bool TryTank();
	virtual bool DoTank(Unit* pmTarget);
	virtual bool TryDPS(bool pmDelay, bool pmForceInstantOnly, bool pmChasing);
	virtual bool DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing);
	virtual bool Follow();
	virtual bool TryHeal(bool pmForceInstantOnly);
	virtual bool DoHeal(Unit* pmTarget, bool pmForceInstantOnly);
	virtual bool Cure();
	virtual bool Buff();
	virtual bool Assist();
	virtual uint32 Caution();
	virtual bool Revive();
	virtual bool Revive(Unit* pmTarget);
	virtual bool Rest(bool pmForce = false);
	virtual bool Petting();
	virtual bool Wander();
	virtual std::string GetGroupRole();
	virtual void SetGroupRole(std::string pmRoleName);

	bool GroupInCombat();

public:
	Player* me;
	bool initialized;

	float dpsDistance;
	float dpsDistanceMin;
	float followDistance;

	int randomTeleportDelay;

	int corpseRunDelay;
	int dpsDelay;
	int rti;

	int restLimit;
	int drinkDelay;

	int combatDuration;
	int wanderDuration;
	int pvpDelay;

	uint32 basicStrategyType;
	int combatAngleINT;
	bool cure;
	bool aoe;
	bool rushing;
	bool petting;
	bool forceBack;
	bool instantOnly;

	uint32 actionType;
	int actionLimit;
	ObjectGuid ogActionTarget;
	ObjectGuid ogTank;
	ObjectGuid ogHealer;

	std::unordered_map<std::string, std::unordered_set<uint32>> cautionSpellMap;
	std::unordered_set<Position*> cautionPosSet;
};


class NingerStrategy_The_Underbog :public NingerStrategy_Base
{
public:
	NingerStrategy_The_Underbog();

public:
	bool hungarfen;
};

class NingerStrategy_The_Black_Morass :public NingerStrategy_Base
{
public:
	NingerStrategy_The_Black_Morass();

	bool DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing);
};

class NingerStrategy_Magisters_Terrace :public NingerStrategy_Base
{
public:
	NingerStrategy_Magisters_Terrace();

	void Update(uint32 pmDiff);
	bool TryTank();
	bool DoTank(Unit* pmTarget);
	bool TryDPS(bool pmDelay, bool pmForceInstantOnly, bool pmChasing);
	bool DoDPS(Unit* pmTarget, bool pmForceInstantOnly, bool pmChasing);
	bool DoHeal(Unit* pmTarget, bool pmForceInstantOnly);

	bool kael;
};
#endif
