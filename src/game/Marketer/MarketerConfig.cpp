#include "MarketerConfig.h"
#include "Policies/Singleton.h"
#include <mutex>

#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <string>
#include <fstream>

#ifndef MARKETER_CONFIG_FILE_NAME
# define MARKETER_CONFIG_FILE_NAME  "marketer.conf"
#endif

INSTANTIATE_SINGLETON_1(MarketerConfig);

bool MarketerConfig::SetSource(const std::string& file)
{
	m_filename = file;

	return Reload();
}

bool MarketerConfig::Reload()
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

bool MarketerConfig::IsSet(const std::string& name) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);
	return m_entries.find(nameLower) != m_entries.cend();
}

const std::string MarketerConfig::GetStringDefault(const std::string& name, const std::string& def) const
{
	auto const nameLower = boost::algorithm::to_lower_copy(name);

	auto const entry = m_entries.find(nameLower);

	return entry == m_entries.cend() ? def : entry->second;
}

bool MarketerConfig::GetBoolDefault(const std::string& name, bool def) const
{
	auto const value = GetStringDefault(name, def ? "true" : "false");

	std::string valueLower;
	std::transform(value.cbegin(), value.cend(), std::back_inserter(valueLower), ::tolower);

	return valueLower == "true" || valueLower == "1" || valueLower == "yes";
}

int32 MarketerConfig::GetIntDefault(const std::string& name, int32 def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stoi(value);
}

float MarketerConfig::GetFloatDefault(const std::string& name, float def) const
{
	auto const value = GetStringDefault(name, std::to_string(def));

	return std::stof(value);
}

bool MarketerConfig::StartMarketerSystem()
{
	SetSource(MARKETER_CONFIG_FILE_NAME);
	Reload();

	Enable = GetIntDefault("Enable", 0);
	Reset = GetIntDefault("Reset", 0);

	if (Enable == 0)
	{
		return false;
	}

	return true;
}