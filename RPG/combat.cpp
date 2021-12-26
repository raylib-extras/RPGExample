#include "combat.h"

#include "raylib.h"

int ResolveAttack(const AttackInfo& attack, const DefenseInfo& defense)
{
	int damage = GetRandomValue(-3, 6) + GetRandomValue(attack.MinDamage, attack.MaxDamage);
	int total = damage - defense.Defense;

	if (total < 0)
		return 0;

	return total;
}