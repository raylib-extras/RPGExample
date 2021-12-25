#include "items.h"
#include "sprites.h"

#include <vector>

std::vector<Item> ItemDB;

// ItemDatabase
Item* AddItem(const char* name, int sprite)
{
	int id = int(ItemDB.size());

	CenterSprite(sprite);
	ItemDB.emplace_back(Item{ id, std::string(name), sprite });
	return GetItem(id);
}

Item* GetItem(int id)
{
	if (id < 0 || id >= ItemDB.size())
	return nullptr;

	return &ItemDB[id];
}

void SetupDefaultItems()
{
	AddItem("Sword", SwordSprite)->IsWeapon = true;
	AddItem("Pleather Armor", LeatherArmorSprite)->IsArmor = true;
	AddItem("Bag-o-Gold", BagSprite);
}