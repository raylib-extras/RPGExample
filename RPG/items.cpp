#include "items.h"
#include "sprites.h"

#include <vector>

std::vector<Item> ItemDB;

// ItemDatabase
Item* AddItem(const char* name, int sprite, ItemTypes type)
{
	int id = int(ItemDB.size());

	CenterSprite(sprite);
	ItemDB.emplace_back(Item{ id, std::string(name), sprite, type });
	return GetItem(id);
}

Item* GetItem(int id)
{
	if (id < 0 || id >= ItemDB.size())
	return nullptr;

	return &ItemDB[id];
}

int GetRandomItem(int except)
{
	int id = -1;
	while (id == -1)
	{
		int index = GetRandomValue(0, int(ItemDB.size()) - 1);
		id = ItemDB[index].Id;
		if (id == except)
			id = -1;
	}

	return id;
}


void SetupDefaultItems()
{
	auto* item = AddItem("Sword", SwordSprite, ItemTypes::Weapon);
	item->Attack.MinDamage = 0;
	item->Attack.MaxDamage = 2;
	item->Attack.Cooldown = 1;

	item = AddItem("Pleather Armor", LeatherArmorSprite, ItemTypes::Armor);
	item->Defense.Defense = 2;

	item = AddItem("Bag-o-Gold", BagSprite, ItemTypes::None);

	item = AddItem("Fud", FoodSprite, ItemTypes::Activatable);
	item->Effect = ActivatableEffects::Healing;
	item->Value = 5;
}