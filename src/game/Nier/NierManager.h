#ifndef NIER_MANAGER_H
#define NIER_MANAGER_H

#include "NierEntity.h"

enum GroupRole :uint32
{
    GroupRole_DPS = 0,
    GroupRole_Tank = 1,
    GroupRole_Healer = 2,
};

enum ShapeshiftSpell :uint32
{
    CatForm = 768,
    DireBearForm = 9634,
    BearForm = 5487,
    MoonkinForm = 24858
};

class NierManager
{
    NierManager();
    NierManager(NierManager const&) = delete;
    NierManager& operator=(NierManager const&) = delete;
    ~NierManager() = default;

public:
    void InitializeManager();
    void LogoutNiers(bool pmInstant = false);
    void LoginNiers(uint32 pMasterId = 0);
    void DeleteNiers();
    void CheckNiers(uint32 pMasterId, uint32 pCareer, bool pAlliance, uint32 pNierCount, uint32 pNierType = 0);
    bool IsPolymorphed(Unit* pmTarget);

    void RandomTeleport(Player* me, Player* target);

    bool SpellCasting(Player* me);
    float GetExactDistance(Position p1, Position p2);
    Position PredictPosition(Unit* target);

    void HandleChatCommand(Player* pCommander, std::string pContent, Player* pTargetPlayer = nullptr, Group* pTargetGroup = nullptr);
    void HandlePacket(const WorldSession* pmSession, WorldPacket pmPacket);

    void UpdateNierManager();

    static NierManager* instance();

public:
    std::unordered_map<uint32, std::unordered_map<uint32, uint32>> allianceRaces;
    std::unordered_map<uint32, std::unordered_map<uint32, uint32>> hordeRaces;
    std::unordered_map<uint32, std::string> nierNameMap;
    std::unordered_map<uint32, std::unordered_map<uint32, std::string>> characterTalentTabNameMap;
    std::unordered_map<uint32, std::string> groupRoleNameMap;

    std::unordered_map<uint32, uint32> trainerMap;
    std::unordered_map<uint32, uint32> tamableBeastMap;

    std::unordered_map<uint32, NierEntity*> nierMap;
    uint32 updateIndex;
};

#define sNierManager NierManager::instance()

#endif
