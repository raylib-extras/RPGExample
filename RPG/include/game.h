#pragma once

#include "raylib.h"
#include "sprites.h"
#include "map.h"

#include <vector>

void InitGame();
void ActivateGame();
void QuitGame();
void UpdateGame();

// game state data

struct InventoryContents
{
	int ItemId;
	int Quantity;
};

struct PlayerData
{
	Vector2 Position = { 0,0 };
	SpriteInstance* Sprite = nullptr;

	bool TargetActive = false;
	Vector2 Target = { 0, 0 };
	SpriteInstance* TargetSprite = nullptr;

	float Speed = 100;

	// player stats
	int Health = 100;
	int MaxHealth = 100;

	int Gold = 0;

	// inventory
	int EquipedWeapon = -1;
	int EquipedArmor = -1;
	std::vector<InventoryContents> BackpackContents;

	// event callbacks
};
