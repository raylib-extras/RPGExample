#pragma once

#include "items.h"

#include "raylib.h"

#include <vector>
#include <string>

struct TreasureInstance
{
	int ItemId = -1;
	int Quantity = 1;

	Vector2 Position = { 0,0 };
	int SpriteId;
};

std::vector<TreasureInstance> GetLoot(const std::string& loot_name);