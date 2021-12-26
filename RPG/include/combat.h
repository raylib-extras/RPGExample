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

	int Damage = 0;
	float Cooldown = 1;

	float Range = 10;
};