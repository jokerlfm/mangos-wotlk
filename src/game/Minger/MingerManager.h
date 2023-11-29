#ifndef MINGER_MANAGER_H
#define MINGER_MANAGER_H

#define enum_to_string(x) #x

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>

#include "Common.h"

class SpawnedObject
{
public:
    SpawnedObject()
    {
        guid = 0;
        mapId = 0;
    }

public:
    uint32 guid;
    uint32 mapId;
    Position pos;
};

class MingerManager
{
    MingerManager();
    MingerManager(MingerManager const&) = delete;
    MingerManager& operator=(MingerManager const&) = delete;
    ~MingerManager() = default;

public:
    void InitializeManager();
    bool StringEndWith(const std::string& str, const std::string& tail);
    bool StringStartWith(const std::string& str, const std::string& head);
    bool IsMingerExceptionEntry(uint32 pmEntry);
    bool IsHerb(uint32 pmEntry);
    bool NearHerb(uint32 pmMapId, Position pmPos, float pmDistance);
    bool AddHerb(uint32 pmGuid, uint32 pmMapId, Position pmPos, float pmDistance);
    static MingerManager* instance();    

public:
    std::unordered_set<uint32> herbGOEntrySet;
    std::unordered_set<uint32> instanceEncounterEntrySet;
    std::unordered_set<uint32> heroicCreatureEntrySet;
    std::unordered_set<uint32> worldCreatureEntrySet;
    std::unordered_set<SpawnedObject*> spawnedHerbSet;

    /// <summary>
    /// class, subclass, level, entry set
    /// </summary>
    std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_set<uint32>>>> vendorEquipsMap;
};

#define sMingerManager MingerManager::instance()

#endif
