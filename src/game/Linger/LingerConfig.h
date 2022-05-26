#ifndef LINGER_CONFIG_H
#define LINGER_CONFIG_H

#define LINGER_CONFIG_FILE_NAME "linger.conf"

#ifndef LINGER_UPDATE_DELAY
# define LINGER_UPDATE_DELAY 3600000
#endif

#include "Common.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include <mutex>

#include <string>
#include <unordered_map>

class LingerConfig
{
private:
	std::string m_filename;
	std::unordered_map<std::string, std::string> m_entries; // keys are converted to lower case.  values cannot be.

public:
	LingerConfig()
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
	bool StartLinger();
	uint32 Enable;	
};

#define sLingerConfig MaNGOS::Singleton<LingerConfig>::Instance()

#endif