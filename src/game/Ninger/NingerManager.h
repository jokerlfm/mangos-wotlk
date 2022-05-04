#ifndef NINGER_MANAGER_H
#define NINGER_MANAGER_H

#define enum_to_string(x) #x

#include "NingerEntity.h"

#include <string>
#include <iostream>
#include <sstream>

#include "NingerConfig.h"

class pvpZonePosition
{
public:
    pvpZonePosition()
    {
        mapID = 0;
        minLevel = 0;
        maxLevel = 0;
        spawnDistance = 5.0f;
        flagPointMap.clear();
    }

    uint32 mapID;
    uint32 minLevel;
    uint32 maxLevel;
    float spawnDistance;
    std::unordered_map<uint32, Position> flagPointMap;
};

enum ShapeshiftSpell :uint32
{
    CatForm = 768,
    DireBearForm = 9634,
    BearForm = 5487,
    MoonkinForm = 24858
};

class NingerManager
{
    NingerManager();
    NingerManager(NingerManager const&) = delete;
    NingerManager& operator=(NingerManager const&) = delete;
    ~NingerManager() = default;

public:
    void InitializeManager();
    void UpdateNingerManager(uint32 pmDiff);
    void UpdateNingerEntities(uint32 pmDiff);
    void LogoutNingers(bool pmInstant=false);
    void DeleteNingers();
    bool LoginNinger(uint32 pmLevel, uint32 pmCount);
    void HandleChatCommand(Player* pmCommander, std::string pmContent, Player* pmTargetPlayer = nullptr, Group* pmTargetGroup = nullptr);
    bool StringEndWith(const std::string& str, const std::string& tail);
    bool StringStartWith(const std::string& str, const std::string& head);
    bool IsInstanceEncounter(uint32 pmEntry);
    std::vector<std::string> SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored);
    std::string TrimString(std::string srcStr);
    static NingerManager* instance();    

    void HandleNingerPacket(const WorldSession* pmSession, WorldPacket pmPacket);

public:
    std::unordered_map<uint32, std::unordered_map<uint32, uint32>> allianceRaces;
    std::unordered_map<uint32, std::unordered_map<uint32, uint32>> hordeRaces;
    uint32 nameIndex;
    std::unordered_map<uint32, std::string> ningerNameMap;
    std::unordered_set<NingerEntity*> ningerEntitySet;
    std::unordered_map<uint32, std::unordered_map<uint32, std::string>> characterTalentTabNameMap;
    std::unordered_set<uint32> instanceEncounterEntrySet;

private:
    void CreateNinger(uint32 pmLevel, bool pmAlliance, uint32 pmGroupRole);
    int checkDelay;
};

#define sNingerManager NingerManager::instance()

#endif
