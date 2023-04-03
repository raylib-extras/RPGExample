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

#include "sprites.h"
#include "combat.h"

#include "raylib.h"

#include <string>

class MOB
{
public:
	int Id = -1;

	std::string Name;
	int Sprite = -1;

	int Health = 1;
	DefenseInfo Defense;
	AttackInfo Attack;

	float DetectionRadius = 200;
	float Speed = 50;

	std::string lootTable = "mob_loot";
};

// Mob Database
MOB* AddMob(const char* name, int sprite, int health = 1);
MOB* GetMob(int id);

void SetupDefaultMobs();

// monster constants
constexpr int RatMob = 0;
constexpr int SnakeMob = 1;
constexpr int GhostMob = 2;
constexpr int TrollMob = 3;
constexpr int TurtleMob = 4;
constexpr int BlobMob = 5;
constexpr int OgreMob = 6;
constexpr int MonkMob = 7;
constexpr int BeholderMob = 8;
