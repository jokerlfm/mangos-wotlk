#include "MingManager.h"
#include "MingConfig.h"

MingManager::MingManager()
{
	checkDelay = 0;
}

void MingManager::InitializeManager()
{
	sLog.outBasic("Initialize ming");

	if (!sMingConfig.Initialize())
	{
		return;
	}

	herbGOEntrySet.clear();
	herbGOEntrySet.insert(1618);
	herbGOEntrySet.insert(3724);
	herbGOEntrySet.insert(1617);
	herbGOEntrySet.insert(3725);
	herbGOEntrySet.insert(1619);
	herbGOEntrySet.insert(3726);
	herbGOEntrySet.insert(1620);
	herbGOEntrySet.insert(3727);
	herbGOEntrySet.insert(1621);
	herbGOEntrySet.insert(3729);
	herbGOEntrySet.insert(1622);
	herbGOEntrySet.insert(3730);
	herbGOEntrySet.insert(2045);
	herbGOEntrySet.insert(1623);
	herbGOEntrySet.insert(1628);
	herbGOEntrySet.insert(1624);
	herbGOEntrySet.insert(2041);
	herbGOEntrySet.insert(2042);
	herbGOEntrySet.insert(2046);
	herbGOEntrySet.insert(2043);
	herbGOEntrySet.insert(2044);
	herbGOEntrySet.insert(2866);
	herbGOEntrySet.insert(142140);
	herbGOEntrySet.insert(180165);
	herbGOEntrySet.insert(142141);
	herbGOEntrySet.insert(176642);
	herbGOEntrySet.insert(142142);
	herbGOEntrySet.insert(176636);
	herbGOEntrySet.insert(180164);
	herbGOEntrySet.insert(142143);
	herbGOEntrySet.insert(183046);
	herbGOEntrySet.insert(142144);
	herbGOEntrySet.insert(142145);
	herbGOEntrySet.insert(176637);
	herbGOEntrySet.insert(176583);
	herbGOEntrySet.insert(176638);
	herbGOEntrySet.insert(180167);
	herbGOEntrySet.insert(176584);
	herbGOEntrySet.insert(176639);
	herbGOEntrySet.insert(180168);
	herbGOEntrySet.insert(176586);
	herbGOEntrySet.insert(176640);
	herbGOEntrySet.insert(180166);
	herbGOEntrySet.insert(176587);
	herbGOEntrySet.insert(176641);
	herbGOEntrySet.insert(176588);
	herbGOEntrySet.insert(176589);
	herbGOEntrySet.insert(181270);
	herbGOEntrySet.insert(183044);
	herbGOEntrySet.insert(181271);
	herbGOEntrySet.insert(183045);
	herbGOEntrySet.insert(181277);
	herbGOEntrySet.insert(181275);
	herbGOEntrySet.insert(183043);
	herbGOEntrySet.insert(181276);
	herbGOEntrySet.insert(181278);
	herbGOEntrySet.insert(181279);
	herbGOEntrySet.insert(181280);
	herbGOEntrySet.insert(181281);
	herbGOEntrySet.insert(189973);
	herbGOEntrySet.insert(190169);
	herbGOEntrySet.insert(190170);
	herbGOEntrySet.insert(191019);
	herbGOEntrySet.insert(190170);
	herbGOEntrySet.insert(190171);
	herbGOEntrySet.insert(190172);
	herbGOEntrySet.insert(190173);
	herbGOEntrySet.insert(190174);
	herbGOEntrySet.insert(190175);
	herbGOEntrySet.insert(190176);
	herbGOEntrySet.insert(191303);

	veinGOEntrySet.clear();
	veinGOEntrySet.insert(324);
	veinGOEntrySet.insert(1731);
	veinGOEntrySet.insert(1732);
	veinGOEntrySet.insert(1733);
	veinGOEntrySet.insert(1734);
	veinGOEntrySet.insert(3763);
	veinGOEntrySet.insert(3764);
	veinGOEntrySet.insert(73940);
	veinGOEntrySet.insert(73941);
	veinGOEntrySet.insert(103711);
	veinGOEntrySet.insert(123848);
	veinGOEntrySet.insert(150080);
	veinGOEntrySet.insert(175404);
	veinGOEntrySet.insert(177388);
	veinGOEntrySet.insert(181248);
	veinGOEntrySet.insert(181249);
	veinGOEntrySet.insert(181557);
	veinGOEntrySet.insert(2054);
	veinGOEntrySet.insert(2055);
	veinGOEntrySet.insert(103713);
	veinGOEntrySet.insert(105569);
	veinGOEntrySet.insert(150082);
	veinGOEntrySet.insert(176643);
	veinGOEntrySet.insert(181109);
	veinGOEntrySet.insert(185557);
	veinGOEntrySet.insert(1735);
	veinGOEntrySet.insert(2040);
	veinGOEntrySet.insert(2047);
	//veinGOEntrySet.insert(2653);
	veinGOEntrySet.insert(73939);
	veinGOEntrySet.insert(123309);
	veinGOEntrySet.insert(123310);
	veinGOEntrySet.insert(150079);
	veinGOEntrySet.insert(150081);
	veinGOEntrySet.insert(165658);
	veinGOEntrySet.insert(176645);
	veinGOEntrySet.insert(181108);
	veinGOEntrySet.insert(181555);
	veinGOEntrySet.insert(181556);
	veinGOEntrySet.insert(181569);
	veinGOEntrySet.insert(181570);
	veinGOEntrySet.insert(185877);
	veinGOEntrySet.insert(189978);
	veinGOEntrySet.insert(189979);
	veinGOEntrySet.insert(189980);
	veinGOEntrySet.insert(189981);
	veinGOEntrySet.insert(195036);

	checkDelay = 10000;

	sLog.outBasic("ming initialized");
}

MingManager* MingManager::instance()
{
	static MingManager instance;
	return &instance;
}

bool MingManager::StringEndWith(const std::string& str, const std::string& tail)
{
	return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool MingManager::StringStartWith(const std::string& str, const std::string& head)
{
	return str.compare(0, head.size(), head) == 0;
}

std::vector<std::string> MingManager::SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored)
{
	std::vector<std::string> resultStringVector;
	std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}, delimStr.at(0));
	size_t pos = srcStr.find(delimStr.at(0));
	std::string addedString = "";
	while (pos != std::string::npos) {
		addedString = srcStr.substr(0, pos);
		if (!addedString.empty() || !repeatedCharIgnored) {
			resultStringVector.push_back(addedString);
		}
		srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
		pos = srcStr.find(delimStr.at(0));
	}
	addedString = srcStr;
	if (!addedString.empty() || !repeatedCharIgnored) {
		resultStringVector.push_back(addedString);
	}
	return resultStringVector;
}

std::string MingManager::TrimString(std::string srcStr)
{
	std::string result = srcStr;
	if (!result.empty())
	{
		result.erase(0, result.find_first_not_of(" "));
		result.erase(result.find_last_not_of(" ") + 1);
	}

	return result;
}
