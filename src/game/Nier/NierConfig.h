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
        UpdateDelay = 0;
        UpdateCount = 0;
        NierCount = 0;
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
    uint32 UpdateDelay;
    uint32 UpdateCount;
    uint32 NierCount;
};

#define sNierConfig MaNGOS::Singleton<NierConfig>::Instance()

#endif