#pragma once

#include <string>

struct DefenseInfo
{
	int Defense;
};

struct AttackInfo
{
	std::string Name;

	bool Melee = true;

	int MinDamage = 0;
	int MaxDamage = 0;

	float Cooldown = 1;

	float Range = 10;
};

int ResolveAttack(const AttackInfo& attack, int defense);