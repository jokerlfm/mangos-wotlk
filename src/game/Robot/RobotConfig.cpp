#include "RobotConfig.h"
#include "Policies/Singleton.h"
#include <mutex>

#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <string>
#include <fstream>

#ifndef ROBOT_CONFIG_FILE_NAME
# define ROBOT_CONFIG_FILE_NAME  "robot.conf"
#endif

INSTANTIATE_SINGLETON_1(RobotConfig);

bool RobotConfig::SetSource(const std::string& file)
{
	m_filename = file;

	return Reload();
}

bool RobotConfig::Reload()
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

bool RobotConfig::IsSet(const std::string& name) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);
	return m_entries.find(nameLower) != m_entries.cend();
}

const std::string RobotConfig::GetStringDefault(const std::string& name, const std::string& def) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);

	auto const entry = m_entries.find(nameLower);

	return entry == m_entries.cend() ? def : entry->second;
}

bool RobotConfig::GetBoolDefault(const std::string& name, bool def) const
{
	auto const value = GetStringDefault(name, def ? "true" : "false");

	std::string valueLower;
	std::transform(value.cbegin(), value.cend(), std::back_inserter(valueLower), ::tolower);

	return valueLower == "true" || valueLower == "1" || valueLower == "yes";
}

int32 RobotConfig::GetIntDefault(const std::string& name, int32 def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stoi(value);
}

float RobotConfig::GetFloatDefault(const std::string& name, float def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stof(value);
}

bool RobotConfig::StartRobotSystem()
{
	SetSource(ROBOT_CONFIG_FILE_NAME);
	Reload();

	Enable = GetIntDefault("Enable", 0);
	ResetRobots = GetIntDefault("ResetRobots", 0);
	RobotAccountNamePrefix = GetStringDefault("RobotAccountNamePrefix", "ROBOT");
	OnlineLevel = GetIntDefault("OnlineLevel", 0);
	OnlineMinDelay = GetIntDefault("OnlineMinDelay", 60000);
	OnlineMaxDelay = GetIntDefault("OnlineMaxDelay", 300000);

	if (Enable == 0)
	{
		return false;
	}

	return true;
}