#include "combat.h"

#include "raylib.h"

int ResolveAttack(const AttackInfo& attack, int defense)
{
	int damage = GetRandomValue(-3, 6) + GetRandomValue(attack.MinDamage, attack.MaxDamage);
	int total = damage - defense;

	if (total < 0)
		return 0;

	return total;
}