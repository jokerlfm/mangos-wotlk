#ifndef ROBOT_AI_H
#define ROBOT_AI_H

#ifndef MIN_DISTANCE_GAP
# define MIN_DISTANCE_GAP 0.5f
#endif

#ifndef FOLLOW_MIN_DISTANCE
# define FOLLOW_MIN_DISTANCE 1.0f
#endif

#ifndef FOLLOW_MAX_DISTANCE
# define FOLLOW_MAX_DISTANCE 30.0f
#endif

#ifndef MELEE_MIN_DISTANCE
# define MELEE_MIN_DISTANCE 1.0f
#endif

#ifndef MELEE_MAX_DISTANCE
# define MELEE_MAX_DISTANCE 3.0f
#endif

#ifndef RANGED_MIN_DISTANCE
# define RANGED_MIN_DISTANCE 15.0f
#endif

#ifndef RANGED_MAX_DISTANCE
# define RANGED_MAX_DISTANCE 30.0f
#endif

#include "Strategies/Strategy_Solo_Normal.h"
#include "Strategies/Strategy_Group_Normal.h"
#include "Entities/Player.h"
#include "WorldPacket.h"

class Script_Base;
class Strategy_Solo_Normal;
class Strategy_Group_Normal;

enum RobotState :uint32
{
	RobotState_None = 0,
	RobotState_OffLine,
	RobotState_CheckAccount,
	RobotState_CreateAccount,
	RobotState_CheckCharacter,
	RobotState_CreateCharacter,
	RobotState_CheckLogin,
	RobotState_DoLogin,
	RobotState_Online,
	RobotState_CheckLogoff,
	RobotState_DoLogoff,
};

class RobotAI
{
public:
	RobotAI(uint32 pmTargetLevel, uint32 pmTargetClass, uint32 pmTargetRace);
	~RobotAI();
	void Update();
	Item* GetItemInInventory(uint32 pmEntry);
	void InitializeCharacter();
	void SetStrategy(std::string pmStrategyName, bool pmEnable);
	void ResetStrategy();
	void Prepare();
	void Refresh();
	void RandomTeleport();
	void BaseMove(Unit* pmTarget, float pmDistance = MELEE_MIN_DISTANCE, bool pmAttack = true);
	bool CastSpell(Unit* pmTarget, std::string pmSpellName, float pmDistance = 100, bool pmCheckAura = false, bool pmOnlyMyAura = false, bool pmClearShapeshift = false);
	bool HasAura(Unit* pmTarget, std::string pmSpellName, bool pmOnlyMyAura = false);
	bool UseItem(Item* pmItem, Unit* pmTarget);
	bool EquipNewItem(uint32 pmEntry);
	void HandlePacket(WorldPacket const& packet);
	void HandleChatCommand(Player* pmSender, std::string pmCMD);
	uint32 FindSpellID(std::string pmSpellName);
	bool SpellValid(uint32 pmSpellID);
	bool CancelAura(std::string pmSpellName);
	void CancelAura(uint32 pmSpellID);
	void ClearShapeshift();
	void WhisperTo(std::string pmContent, Language pmLanguage, Player* pmTarget);
	void Logout();

public:
	time_t prevUpdate;
	std::unordered_map<std::string, bool> strategiesMap;
	uint8 characterTalentTab;
	// 0 dps, 1 tank, 2 healer
	uint32 characterType;
	std::string accountName;
	uint32 accountID;
	ObjectGuid characterGUID;
	ObjectGuid masterGUID;
	uint32 targetLevel;
	uint32 targetRace;
	uint32 targetClass;
	int32 allDelay;
	int32 checkDelay;
	int32 onlineDelay;
	int32 offlineDelay;
	uint32 robotState;

	uint32 realPrevTime;

	std::mutex robotPacketQueue_m;
	std::mutex robotChatCommandQueue_m;

	std::unordered_map<std::string, uint32> spellIDMap;
	std::unordered_map<std::string, uint8> spellLevelMap;
	std::unordered_map<uint64, bool> interestMap;


	Strategy_Solo_Normal* st_Solo_Normal;
	Strategy_Group_Normal* st_Group_Normal;

	Script_Base* s_base;
	//std::unordered_map<int, Script_Base*> scriptMap;

	Position targetingPos;

	bool staying;
	bool holding;
};
#endif
