/**********************************************************************************************
*
*   Raylib RPG Example * A simple RPG made using raylib
*
*    LICENSE: zlib/libpng
*
*   Copyright (c) 2020 Jeffery Myers
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

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