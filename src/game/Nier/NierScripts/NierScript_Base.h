#ifndef NIERSCRIPT_BASE_H
#define NIERSCRIPT_BASE_H

enum OrderType :uint32
{
    OrderType_None = 0,
    OrderType_Tank,
    OrderType_DPS,
    OrderType_Revive,
    OrderType_Move
};

class NierScript_Base
{
public:
    NierScript_Base(Player* pMe);
    virtual bool Prepare();
    virtual void Update(uint32 pDiff);
    virtual void InitializeCharacter();
    virtual void LearnTalents();
    virtual void InitializeEquipments(bool pReset = false);

    virtual bool Tank(Unit* pTarget);
    virtual bool Heal(Unit* pTarget);
    virtual bool DPS(Unit* pTarget, Unit* pTank, bool pRushing);
    virtual bool PVP(Unit* pTarget);
    virtual bool Interrupt(Unit* pTarget);
    virtual bool Buff();
	virtual bool Cure();
	virtual bool Revive();

    virtual bool Wander();

    bool Follow(Unit* pTarget);
    
    void MoveTo(Position pDestination, Unit* pTarget = nullptr, uint32 pMoveType = 1);
    void Relocate();    
    void LearnTalent(uint32 pmTalentId, uint32 pmMaxRank = 5);
    bool EuipRandom(uint32 pmEquipSlot, uint32 pmInventoryType, uint32 pmItemClass, uint32 pmItemSubClass, uint32 pmMaxLevel);

	bool UseItem(Item* pmItem, Unit* pmTarget);
	bool UseItem(Item* pmItem, Item* pmTarget);
	bool CastSpell(Unit* pmTarget, uint32 pmSpellId, bool pmCheckAura = false, bool pmOnlyMyAura = false, uint32 pmMaxAuraStack = 1,bool pmClearShapeShift = false);
	void CancelAura(uint32 pmSpellID);

    bool Rest(bool pForce = false);
    bool Drink();
	
	void ChooseTarget(Unit* pmTarget);
	void ClearTarget();
    void InterruptSpells();
	bool SpellValid(uint32 pmSpellID);

public:
	Player* me;
    uint32 masterId;
    uint32 specialty;

    uint32 orderType;

    int updateDelay;
    int actionDelay;
    int prepareDelay;
    int assembleDelay;
    int reviveDelay;
    int wanderDelay;
    int restDelay;
    int drinkDelay;    
    int helpDelay;
    int orderDelay;
    int interruptDelay;

    bool pvp;
    int relocateDelay;
    int repopDelay;

    int combatDuration;

    float dpsDistance;
    float followDistance;
    bool chasing;
    bool freezing;
    bool rushing;

    Position destination;
};
#endif
