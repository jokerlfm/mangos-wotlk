#ifndef NINGER_CONFIG_H
#define NINGER_CONFIG_H

#define NIER_CONFIG_FILE_NAME "nier.conf"

#ifndef NIER_MARK
# define NIER_MARK "NIER"
#endif

#include "Common.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include <mutex>

#include <string>
#include <unordered_map>

class NierConfig
{
private:
    std::string m_filename;
    std::unordered_map<std::string, std::string> m_entries; // keys are converted to lower case.  values cannot be.

public:
    NierConfig()
    {
        Enable = 0;
        Reset = 0;
        UpdateCount = 0;
        NierCount_Horde_Warrior = 0;
        NierCount_Horde_Paladin = 0;
        NierCount_Horde_Hunter = 0;
        NierCount_Horde_Rogue = 0;
        NierCount_Horde_Priest = 0;
        NierCount_Horde_DeathKnight = 0;
        NierCount_Horde_Shaman = 0;
        NierCount_Horde_Mage = 0;
        NierCount_Horde_Warlock = 0;
        NierCount_Horde_Druid = 0;
        NierCount_Alliance_Warrior = 0;
        NierCount_Alliance_Paladin = 0;
        NierCount_Alliancee_Hunter = 0;
        NierCount_Alliance_Rogue = 0;
        NierCount_Alliance_Priest = 0;
        NierCount_Alliance_DeathKnight = 0;
        NierCount_Alliance_Shaman = 0;
        NierCount_Alliance_Mage = 0;
        NierCount_Alliance_Warlock = 0;
        NierCount_Alliance_Druid = 0;
    }

    bool SetSource(const std::string& file);
    bool Reload();

    bool IsSet(const std::string& name) const;

    const std::string GetStringDefault(const std::string& name, const std::string& def = "") const;
    bool GetBoolDefault(const std::string& name, bool def) const;
    int32 GetIntDefault(const std::string& name, int32 def) const;
    float GetFloatDefault(const std::string& name, float def) const;

    const std::string& GetFilename() const { return m_filename; }
    std::mutex m_configLock;

public:
    bool Initialize();
    uint32 Enable;
    uint32 Reset;
    uint32 UpdateCount;
    uint32 NierCount_Horde_Warrior;
    uint32 NierCount_Horde_Paladin;
    uint32 NierCount_Horde_Hunter;
    uint32 NierCount_Horde_Rogue;
    uint32 NierCount_Horde_Priest;
    uint32 NierCount_Horde_DeathKnight;
    uint32 NierCount_Horde_Shaman;
    uint32 NierCount_Horde_Mage;
    uint32 NierCount_Horde_Warlock;
    uint32 NierCount_Horde_Druid;
    uint32 NierCount_Alliance_Warrior;
    uint32 NierCount_Alliance_Paladin;
    uint32 NierCount_Alliancee_Hunter;
    uint32 NierCount_Alliance_Rogue;
    uint32 NierCount_Alliance_Priest;
    uint32 NierCount_Alliance_DeathKnight;
    uint32 NierCount_Alliance_Shaman;
    uint32 NierCount_Alliance_Mage;
    uint32 NierCount_Alliance_Warlock;
    uint32 NierCount_Alliance_Druid;
};

#define sNierConfig MaNGOS::Singleton<NierConfig>::Instance()

#endif