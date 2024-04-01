#include "MingManager.h"
#include "MingConfig.h"

MingManager::MingManager()
{
	dkItemExceptionSet.clear();
	dkSpellsExceptionSet.clear();
	herbGOEntrySet.clear();
	spawnedHerbSet.clear();
	veinGOEntrySet.clear();
	spawnedVeinSet.clear();
	instanceEncounterEntrySet.clear();
	veinsMap.clear();
	vendorEquipsMap.clear();
	flyingCreatureEntrySet.clear();

	checkDelay = 0;
}

void MingManager::InitializeManager()
{
	sLog.outBasic("Initialize ming");

	if (!sMingConfig.Initialize())
	{
		return;
	}

	dkSpellsExceptionSet.clear();
	dkSpellsExceptionSet.insert(48266);
	dkSpellsExceptionSet.insert(49410);
	dkSpellsExceptionSet.insert(45477);
	dkSpellsExceptionSet.insert(3274);
	dkSpellsExceptionSet.insert(7924);
	dkSpellsExceptionSet.insert(10846);
	dkSpellsExceptionSet.insert(19902);
	dkSpellsExceptionSet.insert(10847);
	dkSpellsExceptionSet.insert(3280);
	dkSpellsExceptionSet.insert(19903);
	dkSpellsExceptionSet.insert(54254);
	dkSpellsExceptionSet.insert(7925);
	dkSpellsExceptionSet.insert(61455);
	dkSpellsExceptionSet.insert(59921);
	dkSpellsExceptionSet.insert(59879);
	dkSpellsExceptionSet.insert(49576);
	dkSpellsExceptionSet.insert(47541);
	dkSpellsExceptionSet.insert(45462);

	dkSpellsExceptionSet.insert(3276);
	dkSpellsExceptionSet.insert(3277);
	dkSpellsExceptionSet.insert(3278);
	dkSpellsExceptionSet.insert(7928);
	dkSpellsExceptionSet.insert(7929);
	dkSpellsExceptionSet.insert(10840);
	dkSpellsExceptionSet.insert(10841);
	dkSpellsExceptionSet.insert(18629);
	dkSpellsExceptionSet.insert(18630);

	dkSpellsExceptionSet.insert(33391);
	dkSpellsExceptionSet.insert(33392);

	dkItemExceptionSet.clear();
	dkItemExceptionSet.insert(34658);
	dkItemExceptionSet.insert(38147);
	dkItemExceptionSet.insert(34657);
	dkItemExceptionSet.insert(38145);
	dkItemExceptionSet.insert(41751);
	dkItemExceptionSet.insert(34652);

	noRewardQuestExceptions.clear();
	noRewardQuestExceptions.insert(1682);
	noRewardQuestExceptions.insert(1667);
	noRewardQuestExceptions.insert(9560);
	noRewardQuestExceptions.insert(9571);
	noRewardQuestExceptions.insert(9667);
	noRewardQuestExceptions.insert(9689);

	instanceEncounterEntrySet.clear();
	auto icQR = WorldDatabase.Query("SELECT distinct creditEntry FROM instance_encounters");
	if (icQR)
	{
		do
		{
			Field* fields = icQR->Fetch();
			uint32 eachEntry = fields[0].GetUInt32();
			instanceEncounterEntrySet.insert(eachEntry);
		} while (icQR->NextRow());
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

	flyingCreatureEntrySet.insert(2657);
	flyingCreatureEntrySet.insert(2474);
	flyingCreatureEntrySet.insert(2473);
	flyingCreatureEntrySet.insert(5300);
	flyingCreatureEntrySet.insert(5304);
	flyingCreatureEntrySet.insert(5347);
	flyingCreatureEntrySet.insert(5305);
	flyingCreatureEntrySet.insert(5306);
	flyingCreatureEntrySet.insert(4012);
	flyingCreatureEntrySet.insert(4013);
	flyingCreatureEntrySet.insert(6141);
	flyingCreatureEntrySet.insert(10807);
	flyingCreatureEntrySet.insert(3869);
	flyingCreatureEntrySet.insert(8534);
	flyingCreatureEntrySet.insert(10408);
	flyingCreatureEntrySet.insert(16168);
	flyingCreatureEntrySet.insert(16446);
	flyingCreatureEntrySet.insert(17906);
	flyingCreatureEntrySet.insert(24440);
	flyingCreatureEntrySet.insert(24624);
	flyingCreatureEntrySet.insert(25387);
	flyingCreatureEntrySet.insert(26491);
	flyingCreatureEntrySet.insert(26517);
	flyingCreatureEntrySet.insert(27691);
	flyingCreatureEntrySet.insert(27829);
	flyingCreatureEntrySet.insert(28749);
	flyingCreatureEntrySet.insert(28874);
	flyingCreatureEntrySet.insert(29239);
	flyingCreatureEntrySet.insert(29576);
	flyingCreatureEntrySet.insert(30482);
	flyingCreatureEntrySet.insert(30545);
	flyingCreatureEntrySet.insert(31040);
	flyingCreatureEntrySet.insert(32188);
	flyingCreatureEntrySet.insert(32472);
	flyingCreatureEntrySet.insert(32769);
	flyingCreatureEntrySet.insert(36896);
	flyingCreatureEntrySet.insert(37544);
	flyingCreatureEntrySet.insert(37636);
	flyingCreatureEntrySet.insert(38481);
	flyingCreatureEntrySet.insert(351059);
	flyingCreatureEntrySet.insert(351077);
	flyingCreatureEntrySet.insert(8600);
	flyingCreatureEntrySet.insert(8601);
	flyingCreatureEntrySet.insert(8602);
	flyingCreatureEntrySet.insert(16036);
	flyingCreatureEntrySet.insert(16175);
	flyingCreatureEntrySet.insert(27490);
	flyingCreatureEntrySet.insert(29608);
	flyingCreatureEntrySet.insert(351031);
	flyingCreatureEntrySet.insert(8536);
	flyingCreatureEntrySet.insert(10809);
	flyingCreatureEntrySet.insert(10825);
	flyingCreatureEntrySet.insert(8535);
	flyingCreatureEntrySet.insert(1995);
	flyingCreatureEntrySet.insert(1996);
	flyingCreatureEntrySet.insert(1997);
	flyingCreatureEntrySet.insert(7097);
	flyingCreatureEntrySet.insert(7098);
	flyingCreatureEntrySet.insert(7099);
	flyingCreatureEntrySet.insert(7455);
	flyingCreatureEntrySet.insert(7456);
	flyingCreatureEntrySet.insert(14283);
	flyingCreatureEntrySet.insert(14343);
	flyingCreatureEntrySet.insert(17034);
	flyingCreatureEntrySet.insert(17035);
	flyingCreatureEntrySet.insert(17039);
	flyingCreatureEntrySet.insert(17053);
	flyingCreatureEntrySet.insert(17128);
	flyingCreatureEntrySet.insert(17129);
	flyingCreatureEntrySet.insert(18220);
	flyingCreatureEntrySet.insert(18257);
	flyingCreatureEntrySet.insert(18963);
	flyingCreatureEntrySet.insert(18996);
	flyingCreatureEntrySet.insert(19055);
	flyingCreatureEntrySet.insert(19429);
	flyingCreatureEntrySet.insert(20109);
	flyingCreatureEntrySet.insert(20127);
	flyingCreatureEntrySet.insert(20686);
	flyingCreatureEntrySet.insert(21324);
	flyingCreatureEntrySet.insert(21468);
	flyingCreatureEntrySet.insert(21470);
	flyingCreatureEntrySet.insert(21495);
	flyingCreatureEntrySet.insert(21804);
	flyingCreatureEntrySet.insert(21891);
	flyingCreatureEntrySet.insert(21904);
	flyingCreatureEntrySet.insert(21989);
	flyingCreatureEntrySet.insert(21990);
	flyingCreatureEntrySet.insert(22265);
	flyingCreatureEntrySet.insert(22767);
	flyingCreatureEntrySet.insert(23051);
	flyingCreatureEntrySet.insert(23134);
	flyingCreatureEntrySet.insert(23135);
	flyingCreatureEntrySet.insert(23136);
	flyingCreatureEntrySet.insert(23693);
	flyingCreatureEntrySet.insert(23985);
	flyingCreatureEntrySet.insert(24747);
	flyingCreatureEntrySet.insert(24787);
	flyingCreatureEntrySet.insert(25748);
	flyingCreatureEntrySet.insert(25817);
	flyingCreatureEntrySet.insert(26028);
	flyingCreatureEntrySet.insert(26369);
	flyingCreatureEntrySet.insert(27244);
	flyingCreatureEntrySet.insert(27249);
	flyingCreatureEntrySet.insert(28004);
	flyingCreatureEntrySet.insert(28060);
	flyingCreatureEntrySet.insert(29044);
	flyingCreatureEntrySet.insert(29736);
	flyingCreatureEntrySet.insert(29854);
	flyingCreatureEntrySet.insert(30013);
	flyingCreatureEntrySet.insert(30108);
	flyingCreatureEntrySet.insert(30165);
	flyingCreatureEntrySet.insert(30506);
	flyingCreatureEntrySet.insert(32098);
	flyingCreatureEntrySet.insert(32481);
	flyingCreatureEntrySet.insert(35293);
	flyingCreatureEntrySet.insert(37419);
	flyingCreatureEntrySet.insert(38154);
	flyingCreatureEntrySet.insert(4031);
	flyingCreatureEntrySet.insert(4032);
	flyingCreatureEntrySet.insert(6167);
	flyingCreatureEntrySet.insert(7447);
	flyingCreatureEntrySet.insert(7448);
	flyingCreatureEntrySet.insert(7449);
	flyingCreatureEntrySet.insert(8660);
	flyingCreatureEntrySet.insert(8763);
	flyingCreatureEntrySet.insert(8764);
	flyingCreatureEntrySet.insert(10807);
	flyingCreatureEntrySet.insert(11497);
	flyingCreatureEntrySet.insert(12800);
	flyingCreatureEntrySet.insert(12801);
	flyingCreatureEntrySet.insert(12802);
	flyingCreatureEntrySet.insert(12803);
	flyingCreatureEntrySet.insert(20932);
	flyingCreatureEntrySet.insert(21033);
	flyingCreatureEntrySet.insert(21879);
	flyingCreatureEntrySet.insert(24673);
	flyingCreatureEntrySet.insert(33504);
	flyingCreatureEntrySet.insert(3239);
	flyingCreatureEntrySet.insert(6375);
	flyingCreatureEntrySet.insert(6377);
	flyingCreatureEntrySet.insert(6378);
	flyingCreatureEntrySet.insert(6379);
	flyingCreatureEntrySet.insert(6380);

	checkDelay = 10000;

	sLog.outBasic("ming initialized");
}

MingManager* MingManager::instance()
{
	static MingManager instance;
	return &instance;
}

bool MingManager::IsDKSpellsException(uint32 pmSpellId)
{
	if (dkSpellsExceptionSet.find(pmSpellId) == dkSpellsExceptionSet.end())
	{
		return false;
	}
	return true;
}

bool MingManager::IsDKItemException(uint32 pmItemId)
{
	if (dkItemExceptionSet.find(pmItemId) == dkItemExceptionSet.end())
	{
		return false;
	}
	return true;
}

bool MingManager::IsMingerExceptionEntry(uint32 pmEntry)
{
	if (instanceEncounterEntrySet.find(pmEntry) != instanceEncounterEntrySet.end())
	{
		return true;
	}
	return false;
}

bool MingManager::IsHerb(uint32 pmEntry)
{
	if (herbGOEntrySet.find(pmEntry) != herbGOEntrySet.end())
	{
		return true;
	}

	return false;
}

bool MingManager::NearHerb(uint32 pmMapId, Position pmPos, float pmDistance)
{
	for (std::unordered_set<SpawnedObject*>::iterator soIT = spawnedHerbSet.begin(); soIT != spawnedHerbSet.end(); soIT++)
	{
		if (SpawnedObject* eachSO = *soIT)
		{
			if (eachSO->mapId == pmMapId)
			{
				float distance = sqrt(eachSO->pos.GetDistance(pmPos));
				if (distance < pmDistance)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool MingManager::AddHerb(uint32 pmGuid, uint32 pmMapId, Position pmPos, float pmDistance)
{
	if (!NearHerb(pmMapId, pmPos, pmDistance))
	{
		SpawnedObject* so = new SpawnedObject();
		so->guid = pmGuid;
		so->mapId = pmMapId;
		so->pos = pmPos;
		spawnedHerbSet.insert(so);
		return true;
	}

	return false;
}

bool MingManager::IsVein(uint32 pmEntry)
{
	if (veinGOEntrySet.find(pmEntry) != veinGOEntrySet.end())
	{
		return true;
	}

	return false;
}

bool MingManager::NearVein(uint32 pmMapId, Position pmPos, float pmDistance)
{
	for (std::unordered_set<SpawnedObject*>::iterator soIT = spawnedVeinSet.begin(); soIT != spawnedVeinSet.end(); soIT++)
	{
		if (SpawnedObject* eachSO = *soIT)
		{
			if (eachSO->mapId == pmMapId)
			{
				float distance = sqrt(eachSO->pos.GetDistance(pmPos));
				if (distance < pmDistance)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool MingManager::AddVein(uint32 pmGuid, uint32 pmMapId, Position pmPos, float pmDistance)
{
	if (!NearVein(pmMapId, pmPos, pmDistance))
	{
		SpawnedObject* so = new SpawnedObject();
		so->guid = pmGuid;
		so->mapId = pmMapId;
		so->pos = pmPos;
		spawnedVeinSet.insert(so);
		return true;
	}

	return false;
}

void MingManager::UpdateMingManager(uint32 pDiff)
{
	if (sMingConfig.Enable == 0)
	{
		return;
	}

	if (checkDelay > 0)
	{
		checkDelay -= pDiff;
	}
	else
	{
		checkDelay = 60000;
	}
}

void MingManager::ReplaceVendor(Unit* pmVendor)
{

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
