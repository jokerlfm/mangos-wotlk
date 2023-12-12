#ifndef NIER_BASE_H
#define NIER_BASE_H

enum NierState :uint32
{
    NierState_None = 0,
    NierState_OffLine,
    NierState_Enter,
    NierState_CheckAccount,
    NierState_CreateAccount,
    NierState_CheckCharacter,
    NierState_CreateCharacter,
    NierState_CheckLogin,
    NierState_DoLogin,
    NierState_DoEnum,
    NierState_CheckEnum,
    NierState_Initialize,
    NierState_Equip,
    NierState_LevelUp,
    NierState_Upgrade,
    NierState_Online,
    NierState_Exit,
    NierState_CheckLogoff,
    NierState_DoLogoff,
    NierState_RedoLogin,
    NierState_CheckRedoLogin,
};

class Nier_Base
{
public:
    Nier_Base();    
    virtual void Prepare();
    virtual void Update(uint32 pDiff);
    virtual void Update_Online(uint32 pDiff);
    virtual void InitializeCharacter();

    virtual bool Tank(Unit* pTarget);
    virtual bool Heal(Unit* pTarget);
    virtual bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    virtual bool Buff();
	virtual bool Cure();
	virtual bool Revive();

    virtual bool Wander();

    bool Follow(Unit* pTarget);

    void WalkTo(Position dest);
    void RunTo(Position dest);

    void LearnTalent(uint32 pmTalentId, uint32 pmMaxRank = 5);
    bool InitializeEquipments(bool pReset = false);
    bool EuipRandom(uint32 pmEquipSlot, uint32 pmInventoryType, uint32 pmItemClass, uint32 pmItemSubClass, uint32 pmMaxLevel);

	bool UseItem(Item* pmItem, Unit* pmTarget);
	bool UseItem(Item* pmItem, Item* pmTarget);
	bool CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura = false, bool pmOnlyMyAura = false, uint32 pmMaxAuraStack = 1,bool pmClearShapeShift = false);
	void CancelAura(uint32 pmSpellID);

	bool Rest();
	
	void ChooseTarget(Unit* pmTarget);
	void ClearTarget();
	bool SpellValid(uint32 pmSpellID);

public:
	Player* me;
    std::string account_name;

    uint32 entry;
    uint32 master_id;
    uint32 account_id;
    uint32 character_id;
    uint32 target_level;
    uint32 target_race;
    uint32 target_class;
    uint32 target_specialty;

    uint32 entityState;

    int updateDelay;
    int movementDelay;
    int actionDelay;
    int prepareDelay;
    int assembleDelay;
    int reviveDelay;
    int wanderDelay;
    int restDelay;
    int helpDelay;
    int orderDelay;

    float dpsDistance;
    float followDistance;
    bool chasing;
    bool freezing;

    Position destination;
};
#endif
