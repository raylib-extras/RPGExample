#include "game.h"
#include "main.h"
#include "map.h"
#include "game_hud.h"
#include "items.h"
#include "treasure.h"
#include "monsters.h"
#include "audio.h"
#include "resource_ids.h"

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

const int PlayerData::GetDefense() const
{
	if (EquipedArmor == -1)
		return 0 + BuffDefense;

	return GetItem(Player.EquipedArmor)->Defense.Defense + BuffDefense;
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

void ActivateItem(int item);
void DropItem(int item);

void LoadLevel(const char* level)
{
	LoadMap(level);
	Player.TargetSprite = AddSprite(ClickTargetSprite, Player.Target);
	Player.TargetSprite->Tint = ColorAlpha(Player.TargetSprite->Tint, 0.5f);

	Player.Sprite = AddSprite(PlayerSprite, Player.Position);
	Player.Sprite->Bobble = true;
	Player.Sprite->Shadow = true;

	Player.ActivateItemCallback = ActivateItem;
	Player.DropItemCallback = DropItem;
	Player.EquipWeaponCallback = ActivateItem;
	Player.EquipArmorCallback = ActivateItem;
}

void StartLevel()
{
	GameClock = 0;

	Player.LastConsumeable = -100;
	Player.LastAttack = -100;

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
		const Property* level = exit->GetProperty("target_level");
		if (level != nullptr)
		{
			if (level->Value == "-1")
				Exits.emplace_back(Exit{ exit->Bounds,"endgame" });
			else
				Exits.emplace_back(Exit{ exit->Bounds,"level" + level->Value + ".tmx" });
		}
	}

	Chests.clear();
	TargetChest = nullptr;
	for (const TileObject* chest : GetMapObjectsOfType(ChestType))
	{
		const Property* contents = chest->GetProperty("contents");
		if (contents != nullptr)
			Chests.emplace_back(Chest{ chest->Bounds, contents->Value });
	}

	ItemDrops.clear();

	for (const TileObject* mobSpawn : GetMapObjectsOfType(MobSpawnType))
	{
		const Property* mobType = mobSpawn->GetProperty("mob_type");

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
	LoadLevel("maps/level0.tmx");
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
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !GameHud.IsUiClick(GetMousePosition()))
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
			if (CheckCollisionPointCircle(mousePos, mob.Position, 20))
			{
				TargetMob = &mob;

				if (Vector2Distance(Player.Position, mob.Position) <= Player.GetAttack().Range + 40)
					Player.TargetActive = false;
				break;
			}
		}
	}
}

void PlaceItemDrop(TreasureInstance& item, Vector2& dropPoint)
{
	Item* itemRecord = GetItem(item.ItemId);
	if (!itemRecord)
		return;

	bool valid = false;
	while (!valid)
	{
		float angle = float(GetRandomValue(-180, 180));
		Vector2 vec = { cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
		vec = Vector2Add(dropPoint, Vector2Scale(vec, 45));

		if (PointInMap(vec) && Vector2Distance(vec, Player.Position) > Player.PickupDistance)
		{
			item.Position = vec;
			valid = true;
		}
	}

	auto* sprite = AddSprite(itemRecord->Sprite, item.Position);
	sprite->Shadow = true;
	sprite->Bobble = true;
	item.SpriteId = sprite->Id;

	ItemDrops.push_back(item);
}

void DropLoot(const char* contents, Vector2& dropPoint)
{
	std::vector<TreasureInstance> loot = GetLoot(contents);
	for (TreasureInstance& item : loot)
	{
		PlaceItemDrop(item, dropPoint);
		AddEffect(item.Position, EffectType::ScaleFade, LootSprite, 1);
	}
}

TreasureInstance RemoveInventoryItem(int slot, int quantity)
{
	TreasureInstance treasure = { -1,0 };

	// is it a valid slot?
	if (slot < 0 || slot >= Player.BackpackContents.size())
		return treasure;

	// can't take more than we have
	InventoryContents& inventory = Player.BackpackContents[slot];
	if (inventory.Quantity < quantity)
		quantity = inventory.Quantity;

	// make an item for what we removed
	treasure.ItemId = inventory.ItemId;
	treasure.Quantity = quantity;

	// reduce quantity in inventory
	inventory.Quantity -= quantity;

	// delete the item in inventory if it's empty
	if (inventory.Quantity <= 0)
	{
		Player.BackpackContents.erase(Player.BackpackContents.begin() + slot);
	}

	// return the drop instance
	return treasure;
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
			if (exit.Destination == "endgame")
			{
				EndGame(true, Player.Gold + 100);
			}
			else
			{
				std::string map = "maps/" + exit.Destination;
				LoadLevel(map.c_str());
				StartLevel();
			}

			break;
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
			if (distance < Player.GetAttack().Range + 40)
			{
				MOB* monsterInfo = GetMob(TargetMob->MobId);
				if (monsterInfo != nullptr)
				{
					AddEffect(TargetMob->Position, EffectType::ScaleFade, ClickTargetSprite);
					if (!Player.GetAttack().Melee)
						AddEffect(Player.Position, EffectType::ToTarget, ProjectileSprite, TargetMob->Position, 0.25f);

					int damage = ResolveAttack(Player.GetAttack(), monsterInfo->Defense.Defense);
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

						// if you hit them, they wake up!
						TargetMob->Triggered = true;
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


	float time = GetGameTime();

	float attackTime = time - Player.LastAttack;
	float itemTime = time - Player.LastConsumeable;

	if (attackTime >= Player.GetAttack().Cooldown)
		Player.AttackCooldown = 0;
	else
		Player.AttackCooldown = 1.0f - (attackTime / Player.AttackCooldown);

	float itemCooldown = 1;

	if (itemTime >= itemCooldown)
		Player.ItemCooldown = 0;
	else
		Player.ItemCooldown = 1.0f - (itemTime / itemCooldown);

	if (Player.BuffLifetimeLeft > 0)
	{
		Player.BuffLifetimeLeft -= GetFrameTime();
		if (Player.BuffLifetimeLeft <= 0)
		{
			Player.BuffDefense = 0;
			Player.BuffItem = -1;
			Player.BuffLifetimeLeft = 0;
		}
	}
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
		if (monsterInfo != nullptr)
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

					if (monsterInfo->Attack.Melee)
						AddEffect(Player.Position, EffectType::RotateFade, MobAttackSprite);
					else
						AddEffect(mob.Position, EffectType::ToTarget, ProjectileSprite, Player.Position, 0.5f);

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

	if (Player.EquipedArmor == ChainArmorItem)
		Player.Sprite->SpriteFrame = PlayerChainSprite;
	else if (Player.EquipedArmor == PlateArmorItem)
		Player.Sprite->SpriteFrame = PlayerPlateSprite;
	else if (Player.EquipedArmor == LeatherArmorItem)
		Player.Sprite->SpriteFrame = PlayerLeatherSprite;
	else
		Player.Sprite->SpriteFrame = PlayerSprite;

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

MobInstance* GetNearestMobInSight()
{
	MobInstance* nearest = nullptr;
	float nearestDistance = 9999999.9f;

	for (auto& mob : Mobs)
	{
		if (Ray2DHitsMap(mob.Position,Player.Position))
			continue;

		float dist = Vector2Distance(mob.Position, Player.Position);

		if (dist < nearestDistance)
		{
			nearest = &mob;
			nearestDistance = dist;
		}
	}

	return nearest;
}

void UpdateSprites()
{
	UpdatePlayerSprite();
	UpdateMobSprites();
}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		if (GameHud.InventoryOpen)
			GameHud.InventoryOpen = false;
		else
		{
			PauseGame();
			return;
		}
	}

	if (!IsWindowFocused())
	{
		PauseGame();
		return;
	}

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

	SetVisiblePoint(Player.Position);
}

void UseConsumable(Item* item)
{
	if (item == nullptr || !item->IsActivatable())
		return;

	float time = GetGameTime() - Player.LastConsumeable;
	if (time < 1)
		return;

	Player.LastConsumeable = GetGameTime();

	switch (item->Effect)
	{
	case ActivatableEffects::Healing:
		Player.Health += item->Value;
		if (Player.Health > Player.MaxHealth)
			Player.Health = Player.MaxHealth;

		PlaySound(PlayerHealSoundId);
		AddEffect(Player.Position, EffectType::RiseFade, HealingSprite, 2);
		break;

	case ActivatableEffects::Defense:
		Player.BuffDefense = item->Value;
		Player.BuffLifetimeLeft = item->Durration;
		Player.BuffItem = item->Sprite;
		break;

	case ActivatableEffects::Damage:
	{
		MobInstance* mob = GetNearestMobInSight();
		if (mob != nullptr)
		{
			mob->Health -= item->Value;
			PlaySound(CreatureDamageSoundId);
			AddEffect(Player.Position, EffectType::ToTarget, item->Sprite, mob->Position, 1);
			AddEffect(mob->Position, EffectType::RotateFade, item->Sprite, 1);
		}
		break;
	}
	}
}

void ActivateItem(int slotIndex)
{
	if (slotIndex < 0 || slotIndex >= Player.BackpackContents.size())
		return;

	InventoryContents& inventorySlot = Player.BackpackContents[slotIndex];

	Item* item = GetItem(inventorySlot.ItemId);
	if (item == nullptr)
		return;

	TreasureInstance removedItem = RemoveInventoryItem(slotIndex, 1);

	if (removedItem.Quantity == 0)
		return;

	switch (item->ItemType)
	{
		case ItemTypes::Activatable:
			UseConsumable(item);
			removedItem.ItemId = -1;
			removedItem.Quantity = 0;
			break;

		case ItemTypes::Weapon:
		{
			// save our current weapon
			int weapon = Player.EquipedWeapon;

			// equip new weapon
			Player.EquipedWeapon = removedItem.ItemId;

			// replace the removed item with the old weapon
			removedItem.ItemId = weapon;
			break;
		}

		case ItemTypes::Armor:
		{
			// save our current armor
			int armor = Player.EquipedArmor;

			// equip new weapon
			Player.EquipedArmor = removedItem.ItemId;

			// replace the removed item with the old weapon
			removedItem.ItemId = armor;
			break;
		}

	}

	// put whatever we have back, or drop it
	if (removedItem.ItemId != -1)
	{
		// stick it back in our bag
		if (!PickupItem(removedItem))
		{
			// no room, drop it
			PlaceItemDrop(removedItem, Player.Position);
		}
	}
}

void DropItem(int item)
{
	TreasureInstance drop = RemoveInventoryItem(item, 999);
	PlaceItemDrop(drop, Player.Position);
}