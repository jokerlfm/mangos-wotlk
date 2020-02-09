#include "JokerConfig.h"
#include "Policies/Singleton.h"
#include <mutex>

#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <string>
#include <fstream>

#ifndef JOKER_CONFIG_FILE_NAME
# define JOKER_CONFIG_FILE_NAME  "joker.conf"
#endif

INSTANTIATE_SINGLETON_1(JokerConfig);

bool JokerConfig::SetSource(const std::string& file)
{
	m_filename = file;

	return Reload();
}

bool JokerConfig::Reload()
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

bool JokerConfig::IsSet(const std::string& name) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);
	return m_entries.find(nameLower) != m_entries.cend();
}

const std::string JokerConfig::GetStringDefault(const std::string& name, const std::string& def) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);

	auto const entry = m_entries.find(nameLower);

	return entry == m_entries.cend() ? def : entry->second;
}

bool JokerConfig::GetBoolDefault(const std::string& name, bool def) const
{
	auto const value = GetStringDefault(name, def ? "true" : "false");

	std::string valueLower;
	std::transform(value.cbegin(), value.cend(), std::back_inserter(valueLower), ::tolower);

	return valueLower == "true" || valueLower == "1" || valueLower == "yes";
}

int32 JokerConfig::GetIntDefault(const std::string& name, int32 def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stoi(value);
}

float JokerConfig::GetFloatDefault(const std::string& name, float def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stof(value);
}

bool JokerConfig::StartJokerSystem()
{
	SetSource(JOKER_CONFIG_FILE_NAME);
	Reload();

	Enable = GetIntDefault("Enable", 0);

	InstanceEncounterDamageMod_Warrior = GetFloatDefault("InstanceEncounterDamageMod_Warrior", 1);
	InstanceEncounterDamageMod_Paladin = GetFloatDefault("InstanceEncounterDamageMod_Paladin", 1);
	InstanceEncounterDamageMod_Mage = GetFloatDefault("InstanceEncounterDamageMod_Mage", 1);
	EliteDamageMod_Warrior = GetFloatDefault("EliteDamageMod_Warrior", 1);
	EliteDamageMod_Paladin = GetFloatDefault("EliteDamageMod_Paladin", 1);
	EliteDamageMod_Mage = GetFloatDefault("EliteDamageMod_Mage", 1);
	UniqueEliteDamageMod_Warrior = GetFloatDefault("UniqueEliteDamageMod_Warrior", 1);
	UniqueEliteDamageMod_Paladin = GetFloatDefault("UniqueEliteDamageMod_Paladin", 1);
	UniqueEliteDamageMod_Mage = GetFloatDefault("UniqueEliteDamageMod_Mage", 1);
	RareDamageMod_Warrior = GetFloatDefault("RareDamageMod_Warrior", 1);
	RareDamageMod_Paladin = GetFloatDefault("RareDamageMod_Paladin", 1);
	RareDamageMod_Mage = GetFloatDefault("RareDamageMod_Mage", 1);
	RareEliteDamageMod_Warrior = GetFloatDefault("RareEliteDamageMod_Warrior", 1);
	RareEliteDamageMod_Paladin = GetFloatDefault("RareEliteDamageMod_Paladin", 1);
	RareEliteDamageMod_Mage = GetFloatDefault("RareEliteDamageMod_Mage", 1);

	InstanceEncounterAPMod_Warrior = GetFloatDefault("InstanceEncounterAPMod_Warrior", 1);
	InstanceEncounterAPMod_Paladin = GetFloatDefault("InstanceEncounterAPMod_Paladin", 1);
	InstanceEncounterAPMod_Mage = GetFloatDefault("InstanceEncounterAPMod_Mage", 1);
	EliteAPMod_Warrior = GetFloatDefault("EliteAPMod_Warrior", 1);
	EliteAPMod_Paladin = GetFloatDefault("EliteAPMod_Paladin", 1);
	EliteAPMod_Mage = GetFloatDefault("EliteAPMod_Mage", 1);
	UniqueEliteAPMod_Warrior = GetFloatDefault("UniqueEliteAPMod_Warrior", 1);
	UniqueEliteAPMod_Paladin = GetFloatDefault("UniqueEliteAPMod_Paladin", 1);
	UniqueEliteAPMod_Mage = GetFloatDefault("UniqueEliteAPMod_Mage", 1);
	RareAPMod_Warrior = GetFloatDefault("RareAPMod_Warrior", 1);
	RareAPMod_Paladin = GetFloatDefault("RareAPMod_Paladin", 1);
	RareAPMod_Mage = GetFloatDefault("RareAPMod_Mage", 1);
	RareEliteAPMod_Warrior = GetFloatDefault("RareEliteAPMod_Warrior", 1);
	RareEliteAPMod_Paladin = GetFloatDefault("RareEliteAPMod_Paladin", 1);
	RareEliteAPMod_Mage = GetFloatDefault("RareEliteAPMod_Mage", 1);

	if (Enable == 0)
	{
		return false;
	}

	return true;
}
