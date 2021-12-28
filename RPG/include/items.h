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
constexpr int CoolSwordItem = 4;
constexpr int AwesomeSwordItem = 5;
constexpr int AxeItem = 6;
constexpr int MightyAxeItem = 7;
constexpr int ForkItem = 8;
constexpr int BowItem = 9;
constexpr int GoodBowItem = 10;
constexpr int ClubItem = 11;
constexpr int ChainArmorItem = 12;
constexpr int PlateArmorItem = 13;
constexpr int PotionItem = 14;
constexpr int ShieldItem = 15;
constexpr int FireballItem = 16;


