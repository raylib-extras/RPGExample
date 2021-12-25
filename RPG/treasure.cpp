#include "treasure.h"
#include "items.h"

std::vector<TreasureInstance> GetLoot(const std::string& loot_name)
{
	std::vector<TreasureInstance> loot;

	if (loot_name == "tutorial_loot_0")
		loot.emplace_back(TreasureInstance{ LeatherArmorItem });
	else if (loot_name == "tutorial_loot_1")
		loot.emplace_back(TreasureInstance{ SwordItem });

	return loot;
}