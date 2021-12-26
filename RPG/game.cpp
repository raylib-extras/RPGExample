#include "game.h"
#include "main.h"
#include "map.h"
#include "game_hud.h"
#include "items.h"
#include "treasure.h"
#include "monsters.h"
#include "audio.h"

#include "raylib.h"
#include "raymath.h"

struct Exit
{
	Rectangle Bounds;
	std::string Destination;
};

struct Chest
{
	Rectangle Bounds;
	std::string Contents;
	bool Opened = false;
};

struct MobInstance
{
	int MobId = -1;
	Vector2 Position;
	int Health;
	int SpriteId;
	
	bool Triggered = false;
	float LastAttack = -100;
};

PlayerData Player;

const AttackInfo& PlayerData::GetAttack() const
{
	if (EquipedWeapon == -1)
		return DefaultAttack;

	return GetItem(EquipedWeapon)->Attack;
}

const DefenseInfo& PlayerData::GetDefense() const
{
	if (EquipedArmor == -1)
		return DefaultDefense;

	return GetItem(Player.EquipedArmor)->Defense;
}

std::vector<Exit> Exits;
std::vector<Chest> Chests;
Chest* TargetChest = nullptr;
MobInstance* TargetMob = nullptr;

double GameClock = 0;

float GetGameTime() { return float(GameClock); }

std::vector<TreasureInstance> ItemDrops;
std::vector<MobInstance> Mobs;

GameHudScreen GameHud(Player);

void LoadLevel(const char* level)
{
	LoadMap(level);
	Player.TargetSprite = AddSprite(ClickTargetSprite, Player.Target);
	Player.TargetSprite->Tint = ColorAlpha(Player.TargetSprite->Tint, 0.5f);

	Player.Sprite = AddSprite(PlayerSprite, Player.Position);
	Player.Sprite->Bobble = true;
	Player.Sprite->Shadow = true;
}

void StartLevel()
{
	GameClock = 0;

	auto* spawn = GetFirstMapObjectOfType(PlayerSpawnType);
	if (spawn != nullptr)
	{
		Player.Position.x = spawn->Bounds.x;
		Player.Position.y = spawn->Bounds.y;
	}

	Player.TargetActive = false;

	Exits.clear();
	for (const TileObject* exit : GetMapObjectsOfType(ExitType))
	{
		const TileObject::Property* level = exit->GetProperty("target_level");
		if (level != nullptr)
			Exits.emplace_back(Exit{ exit->Bounds,"level" + level->Value + ".tmx" });
	}

	Chests.clear();
	TargetChest = nullptr;
	for (const TileObject* chest : GetMapObjectsOfType(ChestType))
	{
		const TileObject::Property* contents = chest->GetProperty("contents");
		if (contents != nullptr)
			Chests.emplace_back(Chest{ chest->Bounds, contents->Value });
	}

	ItemDrops.clear();

	for (const TileObject* mobSpawn : GetMapObjectsOfType(MobSpawnType))
	{
		const TileObject::Property* mobType = mobSpawn->GetProperty("mob_type");

		MOB* monster = GetMob(mobType->GetInt());
		if (monster == nullptr)
			continue;

		Vector2 pos = Vector2{ mobSpawn->Bounds.x,mobSpawn->Bounds.y };
		auto* sprite = AddSprite(monster->Sprite, pos);
		sprite->Bobble = true;
		sprite->Shadow = true;

		Mobs.push_back(MobInstance{ monster->Id ,pos ,monster->Health, sprite->Id });
	}
}

void InitGame()
{
	ActivateGame();

	// load start level
	LoadLevel("resources/maps/level0.tmx");
	StartLevel();
}

void QuitGame()
{
	ClearMap();
}

void ActivateGame()
{
	SetActiveScreen(&GameHud);
}

void GetPlayerInput()
{
	// check for clicks
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), GetMapCamera());

		if (PointInMap(mousePos))
		{
			Player.TargetActive = true;
			Player.Target = mousePos;
		}

		TargetChest = nullptr;
		for (auto& chest : Chests)
		{
			if (CheckCollisionPointRec(mousePos, chest.Bounds))
			{
				TargetChest = &chest;
			}
		}

		for (auto& mob : Mobs)
		{
			if (CheckCollisionPointCircle(mousePos, mob.Position, Player.GetAttack().Range + 20))
			{
				TargetMob = &mob;
				break;
			}
		}
	}
}

void DropLoot(const char* contents, Vector2& dropPoint)
{
	std::vector<TreasureInstance> loot = GetLoot(contents);
	for (TreasureInstance& item : loot)
	{
		bool valid = false;
		while (!valid)
		{
			float angle = float(GetRandomValue(-180, 180));
			Vector2 vec = { cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
			vec = Vector2Add(dropPoint,Vector2Scale(vec, 45));

			if (PointInMap(vec) && Vector2Distance(vec, Player.Position) > Player.PickupDistance)
			{
				item.Position = vec;
				valid = true;
			}
		}
	
		Item* itemRecord = GetItem(item.ItemId);
		if (!itemRecord)
			continue;

		auto* sprite = AddSprite(itemRecord->Sprite, item.Position);
		sprite->Shadow = true;
		sprite->Bobble = true;
		item.SpriteId = sprite->Id;

		ItemDrops.emplace_back(std::move(item));

		AddEffect(item.Position, EffectType::ScaleFade, LootSprite, 1);
	}
}

bool PickupItem(TreasureInstance& drop)
{
	// special case for bag of gold, because it's not a real item
	if (drop.ItemId == GoldBagItem)
	{
		PlaySound(CoinSoundId);
		Player.Gold += drop.Quantity;
		return true;
	}

	// find our item 
	Item* item = GetItem(drop.ItemId);

	// it's an invalid item, remove it but nobody gets it
	if (item == nullptr)
		return true;

	// see if this is a weapon, and we are unarmed, if so, equip one
	if (item->IsWeapon() && Player.EquipedWeapon == -1)
	{
		Player.EquipedWeapon = item->Id;
		drop.Quantity--;
		PlaySound(ItemPickupSoundId);
	}

	// see if this is armor, and we are naked, if so, equip one
	if (item->IsArmor() && Player.EquipedArmor == -1)
	{
		Player.EquipedArmor = item->Id;
		drop.Quantity--;
		PlaySound(ItemPickupSoundId);
	}

	// Try to add items to any stacks we already have
	if (drop.Quantity > 0)
	{
		// see if we have any already
		for (InventoryContents& content : Player.BackpackContents)
		{
			if (content.ItemId == item->Id)
			{
				content.Quantity += drop.Quantity;
				drop.Quantity = 0;
				PlaySound(ItemPickupSoundId);
				break;
			}
		}
	}

	// Try to add items to a new inventory slot
	if (drop.Quantity > 0 && Player.BackpackContents.size() < 20 )
	{
		Player.BackpackContents.emplace_back(InventoryContents{item->Id,drop.Quantity });
		drop.Quantity = 0;

		PlaySound(ItemPickupSoundId);
	}

	// if we picked them all up, we can destroy the item
	return drop.Quantity == 0;
}


void MovePlayer()
{
	// does the player want to move
	if (Player.TargetActive)
	{
		Vector2 movement = Vector2Subtract(Player.Target, Player.Position);
		float distance = Vector2Length(movement);

		float frameSpeed = GetFrameTime() * Player.Speed;

		if (distance <= frameSpeed)
		{
			Player.Position = Player.Target;
			Player.TargetActive = false;
		}
		else
		{
			movement = Vector2Normalize(movement);
			Vector2 newPos = Vector2Add(Player.Position, Vector2Scale(movement, frameSpeed));

			if (!PointInMap(newPos))
			{
				Player.TargetActive = false;
			}
			else
			{
				Player.Position = newPos;
			}
		}
	}

	// see if the player entered an exit
	for (auto exit : Exits)
	{
		if (CheckCollisionPointRec(Player.Position, exit.Bounds))
		{
			std::string map = "resources/maps/" + exit.Destination;
			LoadLevel(map.c_str());
			StartLevel();
		}
	}
}

void ApplyPlayerActions()
{
	// see if we want to attack any mobs
	if (TargetMob != nullptr)
	{
		// see if we can even attack.
		if (GetGameTime() - Player.LastAttack >= Player.GetAttack().Cooldown)
		{
			float distance = Vector2Distance(TargetMob->Position, Player.Position);
			if (distance < Player.GetAttack().Range)
			{
				MOB* monsterInfo = GetMob(TargetMob->MobId);
				if (monsterInfo != nullptr)
				{
					AddEffect(TargetMob->Position, EffectType::ScaleFade, ClickTargetSprite);

					int damage = ResolveAttack(Player.GetAttack(), monsterInfo->Defense);
					if (damage == 0)
					{
						PlaySound(MissSoundId);
					}
					else
					{
						PlaySound(HitSoundId);
						PlaySound(CreatureDamageSoundId);
						AddEffect(Vector2{ TargetMob->Position.x, TargetMob->Position.y - 16 }, EffectType::RiseFade, DamageSprite);
						TargetMob->Health -= damage;
					}
				}
			}

			TargetMob = nullptr;
		}
	}

	// see if the player is near the last clicked chest, if so open it
	if (TargetChest != nullptr)
	{
		Vector2 center = { TargetChest->Bounds.x + TargetChest->Bounds.width / 2,TargetChest->Bounds.y + TargetChest->Bounds.height / 2 };

		float distance = Vector2Distance(center, Player.Position);
		if (distance <= 50)
		{
			if (!TargetChest->Opened)
			{
				PlaySound(ChestOpenSoundId);
				TargetChest->Opened = true;
				DropLoot(TargetChest->Contents.c_str(), center);
			}
			TargetChest = nullptr;
		}
	}

	// see if we are under any items to pickup
	for (std::vector<TreasureInstance>::iterator item = ItemDrops.begin(); item != ItemDrops.end(); )
	{
		float distance = Vector2Distance(item->Position, Player.Position);
		if (distance <= Player.PickupDistance)
		{
			if (PickupItem(*item))
			{
				RemoveSprite(item->SpriteId);
				item = ItemDrops.erase(item);
				continue;
			}
		}

		item++;
	}

	Player.AttackCooldown = 1.0f - std::min(1.0f, (GetGameTime() - Player.LastAttack) / Player.GetAttack().Cooldown);
}

void CullDeadMobs()
{
	for (std::vector<MobInstance>::iterator mobItr = Mobs.begin(); mobItr != Mobs.end();)
	{
		MOB* monsterInfo = GetMob(mobItr->MobId);
		if (monsterInfo != nullptr && mobItr->Health > 0)
		{
			mobItr++;
			continue;
		}

		if (monsterInfo != nullptr)
			DropLoot(monsterInfo->lootTable.c_str(), mobItr->Position);

		RemoveSprite(mobItr->SpriteId);
		AddEffect(mobItr->Position, EffectType::RotateFade, monsterInfo->Sprite, 3.5f);

		mobItr = Mobs.erase(mobItr);
	}
}

void UpdateMobs()
{
	CullDeadMobs();

	// check for mob actions
	for (auto& mob : Mobs)
	{
		Vector2 vecToPlayer = Vector2Subtract(Player.Position, mob.Position);
		float distance = Vector2Length(vecToPlayer);

		MOB* monsterInfo = GetMob(mob.MobId);
		if (monsterInfo == nullptr)
			continue;

		if (!mob.Triggered)
		{
			// see if the mob should wake up
			if (distance > monsterInfo->DetectionRadius)		// too far away
				continue;

			if (Ray2DHitsMap(Player.Position, mob.Position))
				continue; // something is blocking line of sight
	
			// we see our prey, wake up and get em.
			mob.Triggered = true;

			PlaySound(AlertSoundId);
			AddEffect(mob.Position, EffectType::RiseFade, AwakeSprite, 1);
		}

		if (mob.Triggered)
		{
			if (distance < monsterInfo->Attack.Range)
			{
				// try to attack the player
				if (GetGameTime() - mob.LastAttack  >= monsterInfo->Attack.Cooldown)
				{
					mob.LastAttack = GetGameTime();
					int damage = ResolveAttack(monsterInfo->Attack, Player.GetDefense());

					AddEffect(Player.Position, EffectType::RotateFade, MobAttackSprite);
					if (damage == 0)
					{
						PlaySound(MissSoundId);
					}
					else
					{
						PlaySound(HitSoundId);
						PlaySound(PlayerDamageSoundId);
						AddEffect(Vector2{ Player.Position.x,Player.Position.y - 16 }, EffectType::RiseFade, DamageSprite);
						Player.Health -= damage;
					}
				}
			}
			else
			{
				// try to move
				Vector2 movement = Vector2Normalize(vecToPlayer);

				float frameSpeed = monsterInfo->Speed * GetFrameTime();
				Vector2 newPos = Vector2Add(mob.Position, Vector2Scale(movement, frameSpeed));

				if (PointInMap(newPos))
					mob.Position = newPos;
			}
		}
	}
}

void UpdatePlayerSprite()
{
	if (Player.Sprite != nullptr)
		Player.Sprite->Position = Player.Position;

	if (Player.TargetSprite != nullptr)
	{
		Player.TargetSprite->Active = Player.TargetActive;
		Player.TargetSprite->Position = Player.Target;
	}
}

void UpdateMobSprites()
{
	for (auto& mob : Mobs)
	{
		UpdateSprite(mob.SpriteId, mob.Position);
	}
}

void UpdateSprites()
{
	UpdatePlayerSprite();
	UpdateMobSprites();
}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
		PauseGame();
	
	// only update our game clock when we are unpaused
	GameClock += GetFrameTime();

	GetPlayerInput();
	MovePlayer();
	ApplyPlayerActions();

	UpdateMobs();

	if (Player.Health < 0)
	{
		// you died, change to the end screen
		EndGame(false,Player.Gold);
	}

	UpdateSprites();
}
