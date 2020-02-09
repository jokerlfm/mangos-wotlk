#ifndef JOKER_CONFIG_H
#define JOKER_CONFIG_H

#include "Common.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include <mutex>

#include <string>
#include <unordered_map>

class JokerConfig
{
private:
	std::string m_filename;
	std::unordered_map<std::string, std::string> m_entries; // keys are converted to lower case.  values cannot be.

public:
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

	bool StartJokerSystem();

	uint32 Enable;

	float InstanceEncounterDamageMod_Warrior;
	float InstanceEncounterDamageMod_Paladin;
	float InstanceEncounterDamageMod_Mage;
	float EliteDamageMod_Warrior;
	float EliteDamageMod_Paladin;
	float EliteDamageMod_Mage;
	float UniqueEliteDamageMod_Warrior;
	float UniqueEliteDamageMod_Paladin;
	float UniqueEliteDamageMod_Mage;
	float RareDamageMod_Warrior;
	float RareDamageMod_Paladin;
	float RareDamageMod_Mage;
	float RareEliteDamageMod_Warrior;
	float RareEliteDamageMod_Paladin;
	float RareEliteDamageMod_Mage;

	float InstanceEncounterAPMod_Warrior;
	float InstanceEncounterAPMod_Paladin;
	float InstanceEncounterAPMod_Mage;
	float EliteAPMod_Warrior;
	float EliteAPMod_Paladin;
	float EliteAPMod_Mage;
	float UniqueEliteAPMod_Warrior;
	float UniqueEliteAPMod_Paladin;
	float UniqueEliteAPMod_Mage;
	float RareAPMod_Warrior;
	float RareAPMod_Paladin;
	float RareAPMod_Mage;
	float RareEliteAPMod_Warrior;
	float RareEliteAPMod_Paladin;
	float RareEliteAPMod_Mage;
};

#define sJokerConfig MaNGOS::Singleton<JokerConfig>::Instance()

#endif