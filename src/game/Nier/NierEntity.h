#ifndef NIER_ENTITY_H
#define NIER_ENTITY_H

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

class NierEntity
{
public:
    NierEntity();
    virtual void Update();

public:
    int updateDelay;

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

    uint32 gametime;
};
#endif
