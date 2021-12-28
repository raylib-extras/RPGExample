#pragma once

#include "combat.h"
#include<string>

enum class ItemTypes
{
	None,
	Weapon,
	Armor,
	Activatable,
};

enum class ActivatableEffects
{
	None,
	Healing,
	Defense,
	Damage
};

class Item
{
public:
	int Id = -1;
	std::string Name;
	int Sprite;

	ItemTypes ItemType = ItemTypes::None;
	int Value = 0;
	float Lifetime = 0;

	inline bool IsWeapon() const { return ItemType == ItemTypes::Weapon; }
	inline bool IsArmor() const { return ItemType == ItemTypes::Armor; }
	inline bool IsActivatable() const { return ItemType == ItemTypes::Activatable; }

	AttackInfo Attack;
	DefenseInfo Defense;

	ActivatableEffects Effect = ActivatableEffects::None;
	float Durration = 0;
};

// ItemDatabase
Item* AddItem(const char* name, int sprite, ItemTypes type);
Item* GetItem(int id);

int GetRandomItem(int except = -1);

void SetupDefaultItems();

// Item constants
constexpr int SwordItem = 0;
constexpr int LeatherArmorItem = 1;
constexpr int GoldBagItem = 2;
constexpr int FoodItem = 3;