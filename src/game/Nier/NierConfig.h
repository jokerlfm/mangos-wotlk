#ifndef NIER_CONFIG_H
#define NIER_CONFIG_H

#define NIER_CONFIG_FILE_NAME "nier.conf"

#ifndef NIER_MARK
# define NIER_MARK "nier"
#endif

#ifndef AOE_TARGETS_RANGE
# define AOE_TARGETS_RANGE 4.0f
#endif

#ifndef MID_RANGE
# define MID_RANGE 8.0f
#endif

#ifndef MIN_DISTANCE_GAP
# define MIN_DISTANCE_GAP 0.25f
#endif

#ifndef MAX_DISTANCE_GAP
# define MAX_DISTANCE_GAP 1.0f
#endif

#ifndef MIN_ANGLE_GAP
# define MIN_ANGLE_GAP 0.2f
#endif

#ifndef FOLLOW_MIN_DISTANCE
# define FOLLOW_MIN_DISTANCE 0.5f
#endif

#ifndef FOLLOW_NEAR_DISTANCE
# define FOLLOW_NEAR_DISTANCE 8.0f
#endif

#ifndef FOLLOW_NORMAL_DISTANCE
# define FOLLOW_NORMAL_DISTANCE 20.0f
#endif

#ifndef FOLLOW_FAR_DISTANCE
# define FOLLOW_FAR_DISTANCE 25.0f
#endif

#ifndef FOLLOW_MAX_DISTANCE
# define FOLLOW_MAX_DISTANCE 35.0f
#endif

#ifndef MELEE_MIN_DISTANCE
# define MELEE_MIN_DISTANCE 0.5f
#endif

#ifndef MELEE_MAX_DISTANCE
# define MELEE_MAX_DISTANCE 2.0f
#endif

#ifndef RANGE_MIN_DISTANCE
# define RANGE_MIN_DISTANCE 8.0f
#endif

#ifndef RANGE_NEAR_DISTANCE
# define RANGE_NEAR_DISTANCE 10.0f
#endif

#ifndef RANGE_NORMAL_DISTANCE
# define RANGE_NORMAL_DISTANCE 25.0f
#endif

#ifndef RANGE_FAR_DISTANCE
# define RANGE_FAR_DISTANCE 30.0f
#endif

#ifndef RANGE_MAX_DISTANCE
# define RANGE_MAX_DISTANCE 38.0f
#endif

#ifndef CAUTION_DISTANCE
# define CAUTION_DISTANCE 15.0f
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
	bool StartNier();
	uint32 Enable;
	uint32 Reset;
};

#define sNierConfig MaNGOS::Singleton<NierConfig>::Instance()

#endif