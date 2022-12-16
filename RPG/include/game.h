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

#pragma once

#include "raylib.h"
#include "sprites.h"
#include "map.h"
#include "combat.h"

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

class PlayerData
{
public:
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

	const AttackInfo& GetAttack() const;
	const int GetDefense() const;

	float LastAttack = 0;
	float LastConsumeable = 0;
	float AttackCooldown = 0;
	float ItemCooldown = 0;

	int BuffItem = -1;
	float BuffLifetimeLeft = 0;

	int BuffDefense = 0;

	// inventory
	int EquipedWeapon = -1;
	int EquipedArmor = -1;
	std::vector<InventoryContents> BackpackContents;

	float PickupDistance = 20;

	// event callbacks
	// a callback that takes an int
	typedef void(*ItemCallback)(int);

	// callbacks that the HUD can trigger on inventory
	ItemCallback ActivateItemCallback = nullptr;
	ItemCallback EquipArmorCallback = nullptr;
	ItemCallback EquipWeaponCallback = nullptr;
	ItemCallback DropItemCallback = nullptr;

private:
	AttackInfo DefaultAttack = { "Slap", true, 1, 1, 1.0f, 10.0f };
	DefenseInfo DefaultDefense = { 0 };
};
