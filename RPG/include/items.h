#pragma once

#include<string>

class Item
{
public:
	int Id = -1;
	std::string Name;
	int Sprite;

	bool IsWeapon = false;
	bool IsArmor = false;
	bool IsActivatable = false;
};

// ItemDatabase
Item* AddItem(const char* name, int sprite);
Item* GetItem(int id);

void SetupDefaultItems();

// Item constants
constexpr int SwordItem = 0;
constexpr int LeatherArmorItem = 1;
constexpr int GoldBagItem = 2;