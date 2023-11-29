#include "MingerManager.h"

MingerManager::MingerManager()
{
	herbGOEntrySet.clear();
	spawnedHerbSet.clear();
	instanceEncounterEntrySet.clear();
	heroicCreatureEntrySet.clear();
	worldCreatureEntrySet.clear();
}

void MingerManager::InitializeManager()
{
	sLog.outBasic("Initialize minger");

	//instanceEncounterEntrySet.clear();
	//QueryResult* icQR = WorldDatabase.Query("SELECT distinct creditEntry FROM instance_encounters");
	//if (icQR)
	//{
	//	do
	//	{
	//		Field* fields = icQR->Fetch();
	//		uint32 eachEntry = fields[0].GetUInt32();
	//		instanceEncounterEntrySet.insert(eachEntry);
	//	} while (icQR->NextRow());
	//}
	//delete icQR;
	//heroicCreatureEntrySet.clear();
	//QueryResult* hcQR = WorldDatabase.Query("SELECT DifficultyEntry1 FROM creature_template where DifficultyEntry1 > 0 and entry in (SELECT distinct id FROM creature where map in (269,540,542,543,545,546,547,552,553,554,555,556,557,558,560,574,575,576,578,585,595,599,600,601,602,604,608,619,631,632,649,650,658,668,724) and id > 0);");
	//if (hcQR)
	//{
	//	do
	//	{
	//		Field* fields = hcQR->Fetch();
	//		uint32 eachEntry = fields[0].GetUInt32();
	//		heroicCreatureEntrySet.insert(eachEntry);
	//	} while (hcQR->NextRow());
	//}
	//delete hcQR;
	heroicCreatureEntrySet.insert(18374); // focus fire 
	heroicCreatureEntrySet.insert(20535); // Lieutenant Drake
	heroicCreatureEntrySet.insert(20521); // Captain Skarloc
	heroicCreatureEntrySet.insert(20531); // Epoch Hunter
	heroicCreatureEntrySet.insert(20534); // Infinite Slayer
	heroicCreatureEntrySet.insert(20532); // Infinite Defiler
	heroicCreatureEntrySet.insert(20533); // Infinite Saboteur
	heroicCreatureEntrySet.insert(22164); // Infinite Assassin
	heroicCreatureEntrySet.insert(22172); // Rift Lord
	heroicCreatureEntrySet.insert(20738); // Chrono Lord Deja
	heroicCreatureEntrySet.insert(20745); // Temporus
	heroicCreatureEntrySet.insert(20737); // Aeonus
	heroicCreatureEntrySet.insert(20741); // Infinite Chronomancer
	heroicCreatureEntrySet.insert(22166); // Infinite Executioner
	heroicCreatureEntrySet.insert(22171); // Rift Keeper
	heroicCreatureEntrySet.insert(22165); // Infinite Chronomancer
	heroicCreatureEntrySet.insert(22169); // Infinite Whelp
	heroicCreatureEntrySet.insert(24674); // Phoenix
	heroicCreatureEntrySet.insert(24675); // Phoenix Egg
	heroicCreatureEntrySet.insert(25564); // Magister
	heroicCreatureEntrySet.insert(25541); // Magister
	heroicCreatureEntrySet.insert(25550); // Magister
	heroicCreatureEntrySet.insert(25555); // Magister
	heroicCreatureEntrySet.insert(25579); // Magister
	heroicCreatureEntrySet.insert(25556); // Magister
	heroicCreatureEntrySet.insert(25549); // Magister
	heroicCreatureEntrySet.insert(25574); // Magister
	heroicCreatureEntrySet.insert(25578); // Magister

	worldCreatureEntrySet.clear();
	worldCreatureEntrySet.insert(21838); // terrok 

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
	herbGOEntrySet.insert(190173);
	herbGOEntrySet.insert(190174);
	herbGOEntrySet.insert(190175);

	sLog.outBasic("minger initialized");
}

MingerManager* MingerManager::instance()
{
	static MingerManager instance;
	return &instance;
}

bool MingerManager::IsMingerExceptionEntry(uint32 pmEntry)
{
	if (instanceEncounterEntrySet.find(pmEntry) != instanceEncounterEntrySet.end())
	{
		return true;
	}
	if (heroicCreatureEntrySet.find(pmEntry) != heroicCreatureEntrySet.end())
	{
		return true;
	}
	if (worldCreatureEntrySet.find(pmEntry) != worldCreatureEntrySet.end())
	{
		return true;
	}
	return false;
}

bool MingerManager::IsHerb(uint32 pmEntry)
{
	if (herbGOEntrySet.find(pmEntry) != herbGOEntrySet.end())
	{
		return true;
	}

	return false;
}

bool MingerManager::NearHerb(uint32 pmMapId, Position pmPos, float pmDistance)
{
	for (std::unordered_set<SpawnedObject*>::iterator soIT = spawnedHerbSet.begin(); soIT != spawnedHerbSet.end(); soIT++)
	{
		if (SpawnedObject* eachSO = *soIT)
		{
			if (eachSO->mapId == pmMapId)
			{
				float distance = eachSO->pos.GetDistance(pmPos);
				distance = std::sqrt(distance);
				if (distance < pmDistance)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool MingerManager::AddHerb(uint32 pmGuid, uint32 pmMapId, Position pmPos, float pmDistance)
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

bool MingerManager::StringEndWith(const std::string& str, const std::string& tail)
{
	return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool MingerManager::StringStartWith(const std::string& str, const std::string& head)
{
	return str.compare(0, head.size(), head) == 0;
}
