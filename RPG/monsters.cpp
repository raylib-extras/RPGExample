#include "monsters.h"

#include <vector>

std::vector<MOB> MobDB;

MOB* AddMob(const char* name, int sprite, int health)
{
	int id = int(MobDB.size());

	CenterSprite(sprite);
	MobDB.emplace_back(MOB{ id, std::string(name), sprite });
	return GetMob(id);
}

MOB* GetMob(int id)
{
	if (id < 0 || id >= MobDB.size())
		return nullptr;

	return &MobDB[id];
}

void SetupDefaultMobs()
{
	MOB* mob = nullptr;

	// rat
	mob = AddMob("Rat", RatSprite, 1);
	mob->Defense.Defense = 0;
	mob->Attack.Name = "Claw";
	mob->Attack.MinDamage = mob->Attack.MaxDamage = 1;
	mob->Attack.Cooldown = 1;
}