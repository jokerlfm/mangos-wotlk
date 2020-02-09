#include "MarketerManager.h"

#include "Log.h"
#include "MarketerConfig.h"
#include "AuctionHouse/AuctionHouseMgr.h"
#include "Entities/Item.h"
#include "Server/DBCStores.h"
#include "Database/DatabaseEnv.h"
#include "Globals/ObjectMgr.h"
#include "Timer.h"

MarketerManager::MarketerManager()
{
	realPrevTime = 0;

	buyerCheckDelay = 120000;
	sellerCheckDelay = 60000;

	auctionHouseTypeSet.clear();
	auctionHouseTypeSet.insert(AuctionHouseType::AUCTION_HOUSE_ALLIANCE);
	auctionHouseTypeSet.insert(AuctionHouseType::AUCTION_HOUSE_HORDE);
	auctionHouseTypeSet.insert(AuctionHouseType::AUCTION_HOUSE_NEUTRAL);

	vendorUnlimitItemSet.clear();
	QueryResult* vendorItemQR = WorldDatabase.Query("SELECT distinct item FROM npc_vendor where maxcount = 0");
	if (vendorItemQR)
	{
		do
		{
			Field* fields = vendorItemQR->Fetch();
			uint32 eachItemEntry = fields[0].GetUInt32();
			vendorUnlimitItemSet.insert(eachItemEntry);
		} while (vendorItemQR->NextRow());
		delete vendorItemQR;
	}

	sellingItemIDMap.clear();
	for (uint32 itemID = 0; itemID < sItemStorage.GetMaxEntry(); ++itemID)
	{
		ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemID);

		if (!proto)
		{
			continue;
		}
		if (proto->ItemLevel < 1)
		{
			continue;
		}
		if (proto->Quality < 1)
		{
			continue;
		}
		if (proto->Quality > 4)
		{
			continue;
		}
		if (proto->Bonding != ItemBondingType::NO_BIND && proto->Bonding != ItemBondingType::BIND_WHEN_EQUIPPED && proto->Bonding != ItemBondingType::BIND_WHEN_USE)
		{
			continue;
		}
		if (proto->SellPrice == 0 && proto->BuyPrice == 0)
		{
			continue;
		}
		if (vendorUnlimitItemSet.find(proto->ItemId) != vendorUnlimitItemSet.end())
		{
			continue;
		}
		bool sellThis = false;
		switch (proto->Class)
		{
		case ItemClass::ITEM_CLASS_CONSUMABLE:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_CONTAINER:
		{
			if (proto->Quality >= 2)
			{
				sellThis = true;
			}
			break;
		}
		case ItemClass::ITEM_CLASS_WEAPON:
		{
			if (proto->Quality >= 2)
			{
				sellThis = true;
			}
			break;
		}
		case ItemClass::ITEM_CLASS_GEM:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_ARMOR:
		{
			if (proto->Quality >= 2)
			{
				sellThis = true;
			}
			break;
		}
		case ItemClass::ITEM_CLASS_REAGENT:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_PROJECTILE:
		{
			break;
		}
		case ItemClass::ITEM_CLASS_TRADE_GOODS:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_GENERIC:
		{
			break;
		}
		case ItemClass::ITEM_CLASS_RECIPE:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_MONEY:
		{
			break;
		}
		case ItemClass::ITEM_CLASS_QUIVER:
		{
			if (proto->Quality >= 2)
			{
				sellThis = true;
			}
			break;
		}
		case ItemClass::ITEM_CLASS_QUEST:
		{
			sellThis = true;
			break;
		}
		case ItemClass::ITEM_CLASS_KEY:
		{
			break;
		}
		case ItemClass::ITEM_CLASS_PERMANENT:
		{
			break;
		}
		case ItemClass::ITEM_CLASS_MISC:
		{
			if (proto->Quality > 1)
			{
				sellThis = true;
			}
			break;
		}
		case ItemClass::ITEM_CLASS_GLYPH:
		{
			sellThis = true;
			break;
		}
		default:
		{
			break;
		}
		}
		if (sellThis)
		{
			sellingItemIDMap[sellingItemIDMap.size()] = proto->ItemId;
		}
	}
	sellingIndex = 0;
	selling = false;
}

MarketerManager* MarketerManager::instance()
{
	static MarketerManager instance;
	return &instance;
}

void MarketerManager::ResetMarketer()
{
	if (!sMarketerConfig.Reset)
	{
		return;
	}
	sLog.outBasic("Ready to reset marketer seller");

	for (std::set<uint32>::iterator ahTypeIT = auctionHouseTypeSet.begin(); ahTypeIT != auctionHouseTypeSet.end(); ahTypeIT++)
	{
		uint32 ahType = *ahTypeIT;
		uint32 ahID = 0;
		switch (ahType)
		{
		case AUCTION_HOUSE_ALLIANCE: ahID = 1; break;
		case AUCTION_HOUSE_HORDE:    ahID = 6; break;
		default:                     ahID = 7; break;
		}
		AuctionHouseEntry const* ahEntry = sAuctionHouseStore.LookupEntry(ahID);
		AuctionHouseObject* aho = sAuctionMgr.GetAuctionsMap(ahEntry);
		if (!aho)
		{
			sLog.outError("AuctionHouseObject is null");
			return;
		}
		std::set<uint32> auctionIDSet;
		auctionIDSet.clear();
		std::map<uint32, AuctionEntry*> allAuctionsMap = aho->GetAuctions();
		for (std::map<uint32, AuctionEntry*>::iterator aeIT = allAuctionsMap.begin(); aeIT != allAuctionsMap.end(); aeIT++)
		{
			auctionIDSet.insert(aeIT->first);
		}
		for (std::set<uint32>::iterator auctionIDIT = auctionIDSet.begin(); auctionIDIT != auctionIDSet.end(); auctionIDIT++)
		{
			AuctionEntry* eachAE = aho->GetAuction(*auctionIDIT);
			if (eachAE)
			{
				if (eachAE->owner == 0)
				{					
					Item* pItem = sAuctionMgr.GetAItem(eachAE->itemGuidLow);
					if (pItem)
					{
						delete pItem;
					}
					sAuctionMgr.RemoveAItem(eachAE->itemGuidLow);
					eachAE->DeleteFromDB();
					CharacterDatabase.PExecute("DELETE FROM item_instance WHERE guid='%u'", eachAE->itemGuidLow);
					sLog.outBasic("Auction %d removed for auctionhouse %d", eachAE->itemGuidLow, ahID);
				}
			}
		}
	}

	sLog.outBasic("Marketer seller reset");
}

bool MarketerManager::UpdateMarketer()
{
	if (!sMarketerConfig.Enable)
	{
		return false;
	}

	uint32 realCurrTime = WorldTimer::getMSTime();
	uint32 diff = WorldTimer::getMSTimeDiff(realPrevTime, realCurrTime);

	if (diff < MARKETER_MANAGER_UPDATE_GAP)
	{
		return false;
	}

	UpdateSeller(diff);
	UpdateBuyer(diff);
	realPrevTime = realCurrTime;

	return true;
}

bool MarketerManager::UpdateSeller(uint32 pmDiff)
{
	if (sellerCheckDelay > 0)
	{
		sellerCheckDelay -= pmDiff;
		return true;
	}
	if (selling)
	{
		int checkTotalSellCount = 0;
		int maxSellCount = 100;
		while (checkTotalSellCount < maxSellCount)
		{
			if (sellingIndex < sellingItemIDMap.size())
			{
				int itemEntry = sellingItemIDMap[sellingIndex];
				const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itemEntry);
				if (!proto)
				{
					sellingIndex++;
					continue;
				}

				bool sellThis = false;
				uint32 stackCount = 1;
				switch (proto->Class)
				{
				case ItemClass::ITEM_CLASS_CONSUMABLE:
				{
					//sellThis = true;
					//stackCount = proto->Stackable;
					break;
				}
				case ItemClass::ITEM_CLASS_CONTAINER:
				{
					if (proto->Quality >= 2)
					{
						sellThis = true;
						stackCount = proto->Stackable;
					}
					break;
				}
				case ItemClass::ITEM_CLASS_WEAPON:
				{
					if (proto->Quality > 2)
					{
						sellThis = true;
						stackCount = 1;
					}
					break;
				}
				case ItemClass::ITEM_CLASS_GEM:
				{
					sellThis = true;
					stackCount = 1;
					break;
				}
				case ItemClass::ITEM_CLASS_ARMOR:
				{
					if (proto->Quality > 2)
					{
						sellThis = true;
						stackCount = 1;
					}
					break;
				}
				case ItemClass::ITEM_CLASS_REAGENT:
				{
					//sellThis = true;
					//stackCount = proto->Stackable;
					break;
				}
				case ItemClass::ITEM_CLASS_PROJECTILE:
				{
					break;
				}
				case ItemClass::ITEM_CLASS_TRADE_GOODS:
				{
					sellThis = true;
					stackCount = proto->Stackable;
					break;
				}
				case ItemClass::ITEM_CLASS_GENERIC:
				{
					break;
				}
				case ItemClass::ITEM_CLASS_RECIPE:
				{
					sellThis = true;
					stackCount = 1;
					break;
				}
				case ItemClass::ITEM_CLASS_MONEY:
				{
					break;
				}
				case ItemClass::ITEM_CLASS_QUIVER:
				{
					if (proto->Quality > 2)
					{
						sellThis = true;
						stackCount = 1;
					}
					break;
				}
				case ItemClass::ITEM_CLASS_QUEST:
				{
					sellThis = true;
					stackCount = 1;
					break;
				}
				case ItemClass::ITEM_CLASS_KEY:
				{
					break;
				}
				case ItemClass::ITEM_CLASS_PERMANENT:
				{
					break;
				}
				case ItemClass::ITEM_CLASS_MISC:
				{
					//if (proto->Quality > 0)
					//{
					//	sellThis = true;
					//	stackCount = 1;
					//}
					break;
				}
				case ItemClass::ITEM_CLASS_GLYPH:
				{
					sellThis = true;
					stackCount = 1;
					break;
				}
				default:
				{
					break;
				}
				}
				if (!sellThis)
				{
					sellingIndex++;
					continue;
				}
				for (std::set<uint32>::iterator ahTypeIT = auctionHouseTypeSet.begin(); ahTypeIT != auctionHouseTypeSet.end(); ahTypeIT++)
				{
					uint32 ahType = *ahTypeIT;
					uint32 ahID = 0;
					switch (ahType)
					{
					case AUCTION_HOUSE_ALLIANCE: ahID = 1; break;
					case AUCTION_HOUSE_HORDE:    ahID = 6; break;
					default:                     ahID = 7; break;
					}
					AuctionHouseEntry const* ahEntry = sAuctionHouseStore.LookupEntry(ahID);
					AuctionHouseObject* aho = sAuctionMgr.GetAuctionsMap(ahEntry);
					if (!aho)
					{
						sLog.outError("AuctionHouseObject is null");
						return false;
					}

					Item* item = Item::CreateItem(proto->ItemId, stackCount, NULL);
					if (item)
					{
						if (int32 randomPropertyId = item->GetItemRandomPropertyId())
						{
							item->SetItemRandomProperties(randomPropertyId);
						}
						uint32 finalPrice = 0;
						uint8 qualityMuliplier = 1;
						if (proto->Quality > 2)
						{
							qualityMuliplier = proto->Quality - 1;
						}
						finalPrice = proto->SellPrice * stackCount * urand(10, 20);
						if (finalPrice == 0)
						{
							finalPrice = proto->BuyPrice * stackCount * urand(2, 4);
						}
						if (finalPrice == 0)
						{
							break;
						}
						finalPrice = finalPrice * qualityMuliplier;
						if (finalPrice > 100)
						{
							aho->AddAuction(ahEntry, item, 48 * HOUR, finalPrice / 2, finalPrice);
							sLog.outBasic("Auction %s added for auctionhouse %d", proto->Name1, ahID);
						}
					}
				}

				sellingIndex++;
			}
			else
			{
				sellingIndex = 0;
				selling = false;
				sLog.outBasic("Marketer selling finished");
				break;
			}
			checkTotalSellCount++;
		}
		sellerCheckDelay = IN_MILLISECONDS;
	}
	else if (MarketEmpty())
	{
		sellingIndex = 0;
		selling = true;
		sLog.outBasic("Marketer start selling");
	}
	else
	{
		sLog.outBasic("Marketer is not empty");
		sellerCheckDelay = 2 * HOUR * IN_MILLISECONDS;
	}

	return true;
}

bool MarketerManager::MarketEmpty()
{
	for (std::set<uint32>::iterator ahTypeIT = auctionHouseTypeSet.begin(); ahTypeIT != auctionHouseTypeSet.end(); ahTypeIT++)
	{
		uint32 ahType = *ahTypeIT;
		uint32 ahID = 0;
		switch (ahType)
		{
		case AUCTION_HOUSE_ALLIANCE: ahID = 1; break;
		case AUCTION_HOUSE_HORDE:    ahID = 6; break;
		default:                     ahID = 7; break;
		}
		AuctionHouseEntry const* ahEntry = sAuctionHouseStore.LookupEntry(ahID);
		AuctionHouseObject* aho = sAuctionMgr.GetAuctionsMap(ahEntry);
		if (!aho)
		{
			sLog.outError("AuctionHouseObject is null");
			return false;
		}
		std::map<uint32, AuctionEntry*> allAuctionsMap = aho->GetAuctions();
		for (std::map<uint32, AuctionEntry*>::iterator aeIT = allAuctionsMap.begin(); aeIT != allAuctionsMap.end(); aeIT++)
		{
			Item* checkItem = sAuctionMgr.GetAItem(aeIT->second->itemGuidLow);
			if (!checkItem)
			{
				continue;
			}
			if (aeIT->second->owner == 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool MarketerManager::UpdateBuyer(uint32 pmDiff)
{
	if (buyerCheckDelay > 0)
	{
		buyerCheckDelay -= pmDiff;
		return true;
	}
	buyerCheckDelay = HOUR * IN_MILLISECONDS;

	// EJ debug 
	//buyerCheckDelay = 60000;

	sLog.outBasic("Ready to update marketer buyer");
	std::set<uint32> toBuyAuctionIDSet;
	for (std::set<uint32>::iterator ahTypeIT = auctionHouseTypeSet.begin(); ahTypeIT != auctionHouseTypeSet.end(); ahTypeIT++)
	{
		uint32 ahType = *ahTypeIT;
		uint32 ahID = 0;
		switch (ahType)
		{
		case AUCTION_HOUSE_ALLIANCE: ahID = 1; break;
		case AUCTION_HOUSE_HORDE:    ahID = 6; break;
		default:                     ahID = 7; break;
		}
		AuctionHouseEntry const* ahEntry = sAuctionHouseStore.LookupEntry(ahID);
		AuctionHouseObject* aho = sAuctionMgr.GetAuctionsMap(ahEntry);
		if (!aho)
		{
			sLog.outError("AuctionHouseObject is null");
			return false;
		}
		toBuyAuctionIDSet.clear();
		std::map<uint32, AuctionEntry*> allAuctionsMap = aho->GetAuctions();
		for (std::map<uint32, AuctionEntry*>::iterator aeIT = allAuctionsMap.begin(); aeIT != allAuctionsMap.end(); aeIT++)
		{
			Item* checkItem = sAuctionMgr.GetAItem(aeIT->second->itemGuidLow);
			if (!checkItem)
			{
				continue;
			}
			if (aeIT->second->owner == 0)
			{
				continue;
			}
			const ItemPrototype* destIT = sObjectMgr.GetItemPrototype(aeIT->second->itemTemplate);
			if (destIT->SellPrice == 0 && destIT->BuyPrice == 0)
			{
				continue;
			}
			if (destIT->Quality < 1)
			{
				continue;
			}
			if (destIT->Quality > 4)
			{
				continue;
			}
			if (vendorUnlimitItemSet.find(aeIT->second->itemTemplate) != vendorUnlimitItemSet.end())
			{
				continue;
			}
			uint8 buyThis = 1;

			if (!destIT)
			{
				continue;
			}
			switch (destIT->Class)
			{
			case ItemClass::ITEM_CLASS_CONSUMABLE:
			{
				buyThis = urand(0, 5);
				break;
			}
			case ItemClass::ITEM_CLASS_CONTAINER:
			{
				if (destIT->Quality > 2)
				{
					buyThis = 0;
				}
				else if (destIT->Quality == 2)
				{
					buyThis = urand(0, 5);
				}
				break;
			}
			case ItemClass::ITEM_CLASS_WEAPON:
			{
				if (destIT->Quality > 2)
				{
					buyThis = 0;
				}
				else if (destIT->Quality == 2)
				{
					buyThis = urand(0, 5);
				}
				break;
			}
			case ItemClass::ITEM_CLASS_GEM:
			{
				buyThis = 0;
				break;
			}
			case ItemClass::ITEM_CLASS_ARMOR:
			{
				if (destIT->Quality > 2)
				{
					buyThis = 0;
				}
				else if (destIT->Quality == 2)
				{
					buyThis = urand(0, 5);
				}
				break;
			}
			case ItemClass::ITEM_CLASS_REAGENT:
			{
				buyThis = urand(0, 5);
				break;
			}
			case ItemClass::ITEM_CLASS_PROJECTILE:
			{
				buyThis = urand(0, 10);
				break;
			}
			case ItemClass::ITEM_CLASS_TRADE_GOODS:
			{
				buyThis = 0;
				break;
			}
			case ItemClass::ITEM_CLASS_GENERIC:
			{
				break;
			}
			case ItemClass::ITEM_CLASS_RECIPE:
			{
				buyThis = urand(0, 3);
				break;
			}
			case ItemClass::ITEM_CLASS_MONEY:
			{
				break;
			}
			case ItemClass::ITEM_CLASS_QUIVER:
			{
				if (destIT->Quality > 2)
				{
					buyThis = 0;
				}
				else if (destIT->Quality == 2)
				{
					buyThis = urand(0, 5);
				}
				break;
			}
			case ItemClass::ITEM_CLASS_QUEST:
			{
				buyThis = urand(0, 5);
				break;
			}
			case ItemClass::ITEM_CLASS_KEY:
			{
				break;
			}
			case ItemClass::ITEM_CLASS_PERMANENT:
			{
				break;
			}
			case ItemClass::ITEM_CLASS_GLYPH:
			{
				buyThis = urand(0, 5);
			}
			default:
			{
				break;
			}
			}
			if (buyThis != 0)
			{
				continue;
			}
			uint32 basePrice = 0;
			uint32 priceMultiple = 0;
			uint8 qualityMuliplier = 1;
			if (destIT->Quality > 3)
			{
				qualityMuliplier = 2;
			}
			uint32 finalPrice = 0;
			if (destIT->SellPrice > 0)
			{
				finalPrice = destIT->SellPrice * checkItem->GetCount();
			}
			else
			{
				finalPrice = destIT->BuyPrice * checkItem->GetCount();

			}
			finalPrice = finalPrice * qualityMuliplier;
			priceMultiple = aeIT->second->buyout / finalPrice;
			if (priceMultiple > 10)
			{
				priceMultiple = priceMultiple * priceMultiple;
			}
			if (urand(0, priceMultiple) == 0)
			{
				toBuyAuctionIDSet.insert(aeIT->first);
			}
		}

		for (std::set<uint32>::iterator toBuyIT = toBuyAuctionIDSet.begin(); toBuyIT != toBuyAuctionIDSet.end(); toBuyIT++)
		{
			AuctionEntry* destAE = aho->GetAuction(*toBuyIT);
			if (destAE)
			{
				destAE->UpdateBid(destAE->buyout);
				sLog.outBasic("Auction %d was bought by marketer buyer", *toBuyIT);
			}
		}
	}
	sLog.outBasic("Marketer buyer updated");
	return true;
}
