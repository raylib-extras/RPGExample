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

#include "monsters.h"
#include "resource_ids.h"

#include <vector>

std::vector<MOB> MobDB;

MOB* AddMob(const char* name, int sprite, int health)
{
	int id = int(MobDB.size());

	CenterSprite(sprite);
	MobDB.emplace_back(MOB{ id, std::string(name), sprite, health });
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

	// Snake
	mob = AddMob("Snek", SnakeSprite, 10);
	mob->Defense.Defense = 4;
	mob->Attack.Name = "Bite";
	mob->Attack.MinDamage = 1;
	mob->Attack.MaxDamage = 2;
	mob->Attack.Cooldown = 1;

	// Ghost
	mob = AddMob("Ghust", GhostSprite, 10);
	mob->Defense.Defense = 7;
	mob->Attack.Name = "Scare";
	mob->Attack.Range = 50;
	mob->Attack.Melee = false;
	mob->Attack.MinDamage = 1;
	mob->Attack.MaxDamage = 10;
	mob->Attack.Cooldown = 5;

	// Troll
	mob = AddMob("Troll", TrollSprite, 100);
	mob->Defense.Defense = 5;
	mob->Attack.Name = "Punch";
	mob->Attack.Range = 10;
	mob->Attack.Melee = true;
	mob->Attack.MinDamage = 5;
	mob->Attack.MaxDamage = 10;
	mob->Attack.Cooldown = 1;

	// Turtle
	mob = AddMob("Tortile", TurtleSprite, 15);
	mob->Defense.Defense = 7;
	mob->Attack.Name = "Headbut";
	mob->Attack.Range = 10;
	mob->Attack.Melee = true;
	mob->Attack.MinDamage = 1;
	mob->Attack.MaxDamage = 1;
	mob->Attack.Cooldown = 15;

	// Blob
	mob = AddMob("Blorb", BlobSprite, 30);
	mob->Defense.Defense = 4;
	mob->Attack.Name = "Ewwww Gross";
	mob->Attack.Range = 15;
	mob->Attack.Melee = true;
	mob->Attack.MinDamage = 8;
	mob->Attack.MaxDamage = 15;
	mob->Attack.Cooldown = 2;

	// Ogre
	mob = AddMob("DudeBro", OgreSprite, 45);
	mob->Defense.Defense = 4;
	mob->Attack.Name = "Talk about how rust is a better language";
	mob->Attack.Range = 20;
	mob->Attack.Melee = true;
	mob->Attack.MinDamage = 15;
	mob->Attack.MaxDamage = 20;
	mob->Attack.Cooldown = 2;

	// Monk
	mob = AddMob("Munk", OgreSprite, 50);
	mob->Defense.Defense = 5;
	mob->Attack.Name = "GPL Virus Attack";
	mob->Attack.Range = 100;
	mob->Attack.Melee = false;
	mob->Attack.MinDamage = 20;
	mob->Attack.MaxDamage = 25;
	mob->Attack.Cooldown = 5;

	// EyeballMonster
	mob = AddMob("Moderator", BeholderSprite, 100);
	mob->Defense.Defense = 4;
	mob->Attack.Name = "Cast Ray";
	mob->Attack.Range = 100;
	mob->Attack.Melee = false;
	mob->Attack.MinDamage = 20;
	mob->Attack.MaxDamage = 25;
	mob->Attack.Cooldown = 3;
}