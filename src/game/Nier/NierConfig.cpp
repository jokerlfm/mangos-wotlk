#include "NierConfig.h"
#include "Policies/Singleton.h"
#include <mutex>

#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <string>
#include <fstream>

INSTANTIATE_SINGLETON_1(NierConfig);

bool NierConfig::SetSource(const std::string& file)
{
	m_filename = file;

	return Reload();
}

bool NierConfig::Reload()
{
	std::ifstream in(m_filename, std::ifstream::in);

	if (in.fail())
		return false;

	std::unordered_map<std::string, std::string> newEntries;
	std::lock_guard<std::mutex> guard(m_configLock);

	do
	{
		std::string line;
		std::getline(in, line);

		boost::algorithm::trim_left(line);

		if (!line.length())
			continue;

		if (line[0] == '#' || line[0] == '[')
			continue;

		auto const equals = line.find('=');
		if (equals == std::string::npos)
			return false;

		auto const entry = boost::algorithm::trim_copy(boost::algorithm::to_lower_copy(line.substr(0, equals)));
		auto const value = boost::algorithm::trim_copy_if(boost::algorithm::trim_copy(line.substr(equals + 1)), boost::algorithm::is_any_of("\""));

		newEntries[entry] = value;
	} while (in.good());

	m_entries = std::move(newEntries);

	return true;
}

bool NierConfig::IsSet(const std::string& name) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);
	return m_entries.find(nameLower) != m_entries.cend();
}

const std::string NierConfig::GetStringDefault(const std::string& name, const std::string& def) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);

	auto const entry = m_entries.find(nameLower);

	return entry == m_entries.cend() ? def : entry->second;
}

bool NierConfig::GetBoolDefault(const std::string& name, bool def) const
{
	auto const value = GetStringDefault(name, def ? "true" : "false");

	std::string valueLower;
	std::transform(value.cbegin(), value.cend(), std::back_inserter(valueLower), ::tolower);

	return valueLower == "true" || valueLower == "1" || valueLower == "yes";
}

int32 NierConfig::GetIntDefault(const std::string& name, int32 def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stoi(value);
}

float NierConfig::GetFloatDefault(const std::string& name, float def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stof(value);
}

bool NierConfig::Initialize()
{
	SetSource(NIER_CONFIG_FILE_NAME);
	if (!Reload())
	{
		sLog.outError("Error in nier config file : %s.", NIER_CONFIG_FILE_NAME);
		return false;
	}

	Enable = GetIntDefault("Enable", 0);
	Reset = GetIntDefault("Reset", 0);
	UpdateCount = GetIntDefault("UpdateCount", 20);
	NierCount_Horde_Warrior = GetIntDefault("NierCount_Horde_Warrior", 0);
	NierCount_Horde_Paladin = GetIntDefault("NierCount_Horde_Paladin", 0);
	NierCount_Horde_Hunter = GetIntDefault("NierCount_Horde_Hunter", 0);
	NierCount_Horde_Rogue = GetIntDefault("NierCount_Horde_Rogue", 0);
	NierCount_Horde_Priest = GetIntDefault("NierCount_Horde_Priest", 0);
	NierCount_Horde_DeathKnight = GetIntDefault("NierCount_Horde_DeathKnight", 0);
	NierCount_Horde_Shaman = GetIntDefault("NierCount_Horde_Shaman", 0);
	NierCount_Horde_Mage = GetIntDefault("NierCount_Horde_Mage", 0);
	NierCount_Horde_Warlock = GetIntDefault("NierCount_Horde_Warlock", 0);
	NierCount_Horde_Druid = GetIntDefault("NierCount_Horde_Druid", 0);
	NierCount_Alliance_Warrior = GetIntDefault("NierCount_Alliance_Warrior", 0);
	NierCount_Alliance_Paladin = GetIntDefault("NierCount_Alliance_Paladin", 0);
	NierCount_Alliancee_Hunter = GetIntDefault("NierCount_Alliancee_Hunter", 0);
	NierCount_Alliance_Rogue = GetIntDefault("NierCount_Alliance_Rogue", 0);
	NierCount_Alliance_Priest = GetIntDefault("NierCount_Alliance_Priest", 0);
	NierCount_Alliance_DeathKnight = GetIntDefault("NierCount_Alliance_DeathKnight", 0);
	NierCount_Alliance_Shaman = GetIntDefault("NierCount_Alliance_Shaman", 0);
	NierCount_Alliance_Mage = GetIntDefault("NierCount_Alliance_Mage", 0);
	NierCount_Alliance_Warlock = GetIntDefault("NierCount_Alliance_Warlock", 0);
	NierCount_Alliance_Druid = GetIntDefault("NierCount_Alliance_Druid", 0);

	if (Enable == 0)
	{
		sLog.outBasic("nier is disabled.");
		return false;
	}
	sLog.outBasic("nier started.");
	return true;
}