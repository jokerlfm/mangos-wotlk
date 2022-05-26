#ifndef LINGER_MANAGER_H
#define LINGER_MANAGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>

#include "Common.h"

class LingerManager
{
	LingerManager();
	LingerManager(LingerManager const&) = delete;
	LingerManager& operator=(LingerManager const&) = delete;
	~LingerManager() = default;

public:
	void InitializeManager();
	void UpdateManager(uint32 pmDiff);
	void AddAuction(AuctionHouseType pmType, uint32 pmItemEntry);

	static LingerManager* instance();

public:
	std::unordered_set<uint32> materialEntrySet;

	int updateDelay;
};

#define sLingerManager LingerManager::instance()

#endif
