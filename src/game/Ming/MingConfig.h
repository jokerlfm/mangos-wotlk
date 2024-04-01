#ifndef MING_CONFIG_H
#define MING_CONFIG_H

#define MING_CONFIG_FILE_NAME "ming.conf"

#include "Common.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include <mutex>

#include <string>
#include <unordered_map>

class MingConfig
{
private:
    std::string m_filename;
    std::unordered_map<std::string, std::string> m_entries; // keys are converted to lower case.  values cannot be.

public:
    MingConfig()
    {
        Enable = 0;
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
};

#define sMingConfig MaNGOS::Singleton<MingConfig>::Instance()

#endif
