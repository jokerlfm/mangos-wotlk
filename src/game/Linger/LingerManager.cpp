#include "LingerManager.h"
#include "LingerConfig.h"

LingerManager::LingerManager()
{
	materialEntrySet.clear();
	updateDelay = 0;
}

void LingerManager::InitializeManager()
{
	if (sLingerConfig.Enable == 0)
	{
		return;
	}

	sLog.outBasic("Initialize linger");

	materialEntrySet.clear();
	materialEntrySet.insert(39682);

	sLog.outBasic("linger initialized");
}

LingerManager* LingerManager::instance()
{
	static LingerManager instance;
	return &instance;
}

void LingerManager::UpdateManager(uint32 pmDiff)
{
	if (sLingerConfig.Enable == 0)
	{
		return;
	}

	if (updateDelay > 0)
	{
		updateDelay -= pmDiff;
		return;
	}
	else
	{
		updateDelay = LINGER_UPDATE_DELAY;
	}
	for (std::unordered_set<uint32>::iterator entryIt = materialEntrySet.begin(); entryIt != materialEntrySet.end(); entryIt++)
	{
		AddAuction(AuctionHouseType::AUCTION_HOUSE_ALLIANCE, *entryIt);
		AddAuction(AuctionHouseType::AUCTION_HOUSE_HORDE, *entryIt);
		AddAuction(AuctionHouseType::AUCTION_HOUSE_NEUTRAL, *entryIt);
	}
}

void LingerManager::AddAuction(AuctionHouseType pmType, uint32 pmItemEntry)
{
	if (AuctionHouseObject* ah = sAuctionMgr.GetAuctionsMap(pmType))
	{
		if (Item* item = Item::CreateItem(pmItemEntry, 1))
		{
			if (const ItemPrototype* proto = item->GetProto())
			{
				uint32 basePrice = proto->BuyPrice;
				if (basePrice == 0)
				{
					basePrice = proto->SellPrice * 5;
				}
				if (basePrice == 0)
				{
					basePrice = proto->ItemLevel * proto->Quality * 500;
				}
				uint32 ahEntry = 1;
				if (pmType == AuctionHouseType::AUCTION_HOUSE_HORDE)
				{
					ahEntry = 6;
				}
				else if (pmType == AuctionHouseType::AUCTION_HOUSE_NEUTRAL)
				{
					ahEntry = 7;
				}
				ah->AddAuction(sAuctionHouseStore.LookupEntry(ahEntry), item, urand(1, 2) * HOUR, urand(basePrice * 4, basePrice * 8), urand(basePrice * 10, basePrice * 20));
			}
		}
	}
}