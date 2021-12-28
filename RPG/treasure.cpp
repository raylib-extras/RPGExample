#include "treasure.h"
#include "items.h"

std::vector<TreasureInstance> GetLoot(const std::string& loot_name)
{
	std::vector<TreasureInstance> loot;

	if (loot_name == "tutorial_loot_0")
	{
		loot.emplace_back(TreasureInstance{ LeatherArmorItem });
		loot.emplace_back(TreasureInstance{ FoodItem, GetRandomValue(2,5) });
	}
	else if (loot_name == "tutorial_loot_1")
	{
		loot.emplace_back(TreasureInstance{ SwordItem });
	}
	else if (loot_name == "random_loot")
	{
		int count = GetRandomValue(1, 3);
		for (int i = 0; i < count; i++)
			loot.emplace_back(TreasureInstance{ GetRandomItem(GoldBagItem) });
	}
	else if (loot_name == "mob_loot")
	{
		loot.emplace_back(TreasureInstance{ GetRandomItem(GoldBagItem) });
	}

	// random gold
	int value = GetRandomValue(1, 20);
	loot.emplace_back(TreasureInstance{ GoldBagItem, value });

	return loot;
}