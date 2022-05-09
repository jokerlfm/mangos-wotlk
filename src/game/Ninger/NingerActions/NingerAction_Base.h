#ifndef NINGER_ACTION_BASE_H
#define NINGER_ACTION_BASE_H

#ifndef DEFAULT_ACTION_LIMIT_DELAY
#define DEFAULT_ACTION_LIMIT_DELAY 5000
#endif

#ifndef DEFAULT_MOVEMENT_UPDATE_DELAY
#define DEFAULT_MOVEMENT_UPDATE_DELAY 500
#endif

#include "Ninger/NingerConfig.h"
#include "Ninger/NingerManager.h"

enum NingerMovementType :uint32
{
    NingerMovementType_None = 0,
    NingerMovementType_Point,
    NingerMovementType_Tank,
    NingerMovementType_Chase,
    NingerMovementType_Follow,
};

enum NingerMovementDirection :uint32
{
    NingerMovementDirection_Forward = 0,
    NingerMovementDirection_Back,
    NingerMovementDirection_Left,
    NingerMovementDirection_Right,
};

class NingerMovement
{
public:
    NingerMovement(Player* pmMe);
    void ResetMovement();
    void Update(uint32 pmDiff);

    void Run();
    bool Tank(Unit* pmTankTarget);
    bool Chase(Unit* pmChaseTarget);
    bool Follow(Unit* pmFollowTarget);
    void Point(Position pmPosTarget, uint32 pmLimit = DEFAULT_ACTION_LIMIT_DELAY);
    bool Direction(Unit* pmCommander, uint32 pmDirection, uint32 pmLimit = DEFAULT_ACTION_LIMIT_DELAY, float pmDistance = INTERACTION_DISTANCE);
    bool Direction(float pmAngle, uint32 pmLimit = DEFAULT_ACTION_LIMIT_DELAY, float pmDistance = RANGE_MIN_DISTANCE);    

public:
    Player* me;
    ObjectGuid ogTankTarget;
    ObjectGuid ogChaseTarget;
    ObjectGuid ogFollowTarget;
    Position positionTarget;    
    uint32 activeMovementType;
    int moveCheckDelay;
};

class NingerAction_Base
{
public:
    NingerAction_Base();
    virtual void Reset();
    virtual void Prepare();
    virtual void Update(uint32 pmDiff);
    virtual bool DPS(Unit* pmTarget, bool pmAOE, bool pmRush = false);
    virtual bool Tank(Unit* pmTarget, bool pmAOE);
    virtual bool Heal(Unit* pmTarget);
    virtual bool GroupHeal(Unit* pmTarget);
    virtual bool SimpleHeal(Unit* pmTarget);
    virtual bool Cure(Unit* pmTarget);
    virtual bool Buff(Unit* pmTarget);
    virtual bool Assist(Unit* pmTarget);
    virtual bool Revive(Player* pmTarget);
    virtual bool Petting(bool pmSummon = true, bool pmReset = false);
    virtual void InitializeCharacter(uint32 pmTargetLevel, uint32 pmSpecialtyTabIndex);
    virtual void ResetTalent();
    virtual bool InitializeEquipments(bool pmReset = false);

    void RemoveEquipments();
    void LearnTalent(uint32 pmTalentId, uint32 pmMaxRank = MAX_TALENT_RANK);
    void TrainSpells(uint32 pmTrainerEntry);
    void EquipRandomItem(uint32 pmEquipSlot, uint32 pmClass, uint32 pmSubclass, uint32 pmMinQuality, int pmModType, std::unordered_set<uint32> pmInventoryTypeSet = std::unordered_set<uint32>());
    void PetAttack(Unit* pmTarget);
    void PetStop();
    bool UseItem(Item* pmItem, Unit* pmTarget);
    bool UseItem(Item* pmItem, Item* pmTarget);
    bool CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura = false, bool pmOnlyMyAura = false, bool pmClearShapeShift = false, uint32 pmMaxAuraStack = 1);
    void ClearShapeshift();
    void CancelAura(uint32 pmSpellID);
    bool Eat();
    bool Drink();
    bool HealthPotion();
    bool ManaPotion();
    bool RandomTeleport();

    void ChooseTarget(Unit* pmTarget);
    void ClearTarget();
    bool SpellValid(uint32 pmSpellID);
    Item* GetItemInInventory(uint32 pmEntry);
    Player* GetNearbyHostilePlayer(float pmRange = RANGE_MAX_DISTANCE);
    Unit* GetNearbyHostileUnit(float pmRange = RANGE_MAX_DISTANCE);    

    Player* me;

    uint32 specialty;
    int rti;
};
#endif
