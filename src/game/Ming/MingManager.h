#ifndef MING_MANAGER_H
#define MING_MANAGER_H

#define enum_to_string(x) #x

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>

#include "Common.h"

class MingManager
{
    MingManager();
    MingManager(MingManager const&) = delete;
    MingManager& operator=(MingManager const&) = delete;
    ~MingManager() = default;

public:
    void InitializeManager();
    bool StringEndWith(const std::string& str, const std::string& tail);
    bool StringStartWith(const std::string& str, const std::string& head);
    std::vector<std::string> SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored);
    std::string TrimString(std::string srcStr);
    static MingManager* instance();    

public:
    std::unordered_set<uint32> herbGOEntrySet;
    std::unordered_set<uint32> veinGOEntrySet;

    /// <summary>
    /// class, subclass, level, entry set
    /// </summary>
    std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_set<uint32>>>> vendorEquipsMap;

    int checkDelay;
};

#define sMingManager MingManager::instance()

#endif
