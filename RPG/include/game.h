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
	const DefenseInfo& GetDefense() const;

	float LastAttack = -100;
	float LastConsumeable = -100;
	float AttackCooldown = 0;
	float ItemCooldown = 0;

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
	AttackInfo DefaultAttack = { "Slap", true, 1,1 };
	DefenseInfo DefaultDefense = { 0 };
};
