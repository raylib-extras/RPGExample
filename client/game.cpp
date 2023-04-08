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

PlayerData Player1("Player1", PLAYER1);
PlayerData Player2("Player2", PLAYER2);


PlayerData::PlayerData(std::string name, PLAYER_TYPE type) : Name(name), Type(type) {

}
const AttackInfo &PlayerData::GetAttack() const
{
	if (EquipedWeapon == -1)
		return DefaultAttack;

	return GetItem(EquipedWeapon)->Attack;
}

const int PlayerData::GetDefense() const
{
	if (EquipedArmor == -1)
		return 0 + BuffDefense;

	return GetItem(EquipedArmor)->Defense.Defense + BuffDefense;
}

std::vector<Exit> Exits;
std::vector<Chest> Chests;
Chest *TargetChest = nullptr;
MobInstance *TargetMob = nullptr;

double GameClock = 0;

float GetGameTime() { return float(GameClock); }

std::vector<TreasureInstance> ItemDrops;
std::vector<MobInstance> Mobs;

GameHudScreen GameHud(Player1, Player2);
// GameHudScreen GameHud2(Player2);

void LoadLevel(const char *level)
{
	LoadMap(level);
	Player1.Sprite = AddSprite(PlayerSprite, Player1.Position);
	Player1.Sprite->Bobble = true;
	Player1.Sprite->Shadow = true;

	Player2.Sprite = AddSprite(PlayerSprite, Player2.Position);
	Player2.Sprite->Bobble = true;
	Player2.Sprite->Shadow = true;
}

void StartLevel()
{
	GameClock = 0;

	Player1.LastConsumeable = -100;
	Player1.LastAttack = -100;

	Player2.LastConsumeable = -100;
	Player2.LastAttack = -100;

	auto *spawn = GetFirstMapObjectOfType(PlayerSpawnType);
	if (spawn != nullptr)
	{
		Player1.Position.x = spawn->Bounds.x;
		Player1.Position.y = spawn->Bounds.y;

		Player2.Position.x = spawn->Bounds.x;
		Player2.Position.y = spawn->Bounds.y;
	}

	Player1.TargetActive = false;
	Player2.TargetActive = false;

	Exits.clear();
	for (const TileObject *exit : GetMapObjectsOfType(ExitType))
	{
		const Property *level = exit->GetProperty("target_level");
		if (level != nullptr)
		{
			if (level->Value == "-1")
				Exits.emplace_back(Exit{exit->Bounds, "endgame"});
			else
				Exits.emplace_back(Exit{exit->Bounds, "level" + level->Value + ".tmx"});
		}
	}

	Chests.clear();
	TargetChest = nullptr;
	for (const TileObject *chest : GetMapObjectsOfType(ChestType))
	{
		const Property *contents = chest->GetProperty("contents");
		if (contents != nullptr)
			Chests.emplace_back(Chest{chest->Bounds, contents->Value});
	}

	ItemDrops.clear();

	for (const TileObject *mobSpawn : GetMapObjectsOfType(MobSpawnType))
	{
		const Property *mobType = mobSpawn->GetProperty("mob_type");

		MOB *monster = GetMob(mobType->GetInt());
		if (monster == nullptr)
			continue;

		Vector2 pos = Vector2{mobSpawn->Bounds.x, mobSpawn->Bounds.y};
		auto *sprite = AddSprite(monster->Sprite, pos);
		sprite->Bobble = true;
		sprite->Shadow = true;

		Mobs.push_back(MobInstance{monster->Id, pos, monster->Health, sprite->Id});
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
	float moveUnit = 2.0f;

	// User1 input
	bool player1KeyPressed = false;
	Vector2 player1TargetPosition = Player1.Position;

	if (IsKeyDown(KEY_LEFT))
	{
		player1TargetPosition.x -= moveUnit;
		player1KeyPressed = true;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		player1TargetPosition.x += moveUnit;
		player1KeyPressed = true;
	}

	if (IsKeyDown(KEY_UP))
	{
		player1TargetPosition.y -= moveUnit;
		player1KeyPressed = true;
	}

	if (IsKeyDown(KEY_DOWN))
	{
		player1TargetPosition.y += moveUnit;
		player1KeyPressed = true;
	}

	// User2 input
	bool player2KeyPressed = false;
	Vector2 player2TargetPosition = Player2.Position;

	if (IsKeyDown(KEY_A))
	{
		player2TargetPosition.x -= moveUnit;
		player2KeyPressed = true;
	}

	if (IsKeyDown(KEY_D))
	{
		player2TargetPosition.x += moveUnit;
		player2KeyPressed = true;
	}

	if (IsKeyDown(KEY_W))
	{
		player2TargetPosition.y -= moveUnit;
		player2KeyPressed = true;
	}

	if (IsKeyDown(KEY_S))
	{
		player2TargetPosition.y += moveUnit;
		player2KeyPressed = true;
	}

	// check for key inputs
	if (player1KeyPressed)
	{
		if (PointInMap(player1TargetPosition))
		{
			Player1.TargetActive = true;
			Player1.Target = player1TargetPosition;
		}

		TargetChest = nullptr;
		for (auto &chest : Chests)
		{
			if (CheckCollisionPointRec(player1TargetPosition, chest.Bounds))
			{
				TargetChest = &chest;
			}
		}

		for (auto &mob : Mobs)
		{
			if (CheckCollisionPointCircle(player1TargetPosition, mob.Position, 20))
			{
				TargetMob = &mob;

				if (Vector2Distance(Player1.Position, mob.Position) <= Player1.GetAttack().Range + 40)
					Player1.TargetActive = false;
				break;
			}
		}
	}

	if (player2KeyPressed)
	{
		if (PointInMap(player2TargetPosition))
		{
			Player2.TargetActive = true;
			Player2.Target = player2TargetPosition;
		}

		TargetChest = nullptr;
		for (auto &chest : Chests)
		{
			if (CheckCollisionPointRec(player2TargetPosition, chest.Bounds))
			{
				TargetChest = &chest;
			}
		}

		for (auto &mob : Mobs)
		{
			if (CheckCollisionPointCircle(player2TargetPosition, mob.Position, 20))
			{
				TargetMob = &mob;

				if (Vector2Distance(Player2.Position, mob.Position) <= Player2.GetAttack().Range + 40)
					Player2.TargetActive = false;
				break;
			}
		}
	}
}

void PlaceItemDrop(TreasureInstance &item, Vector2 &dropPoint)
{
	Item *itemRecord = GetItem(item.ItemId);
	if (!itemRecord)
		return;

	bool valid = false;
	while (!valid)
	{
		float angle = float(GetRandomValue(-180, 180));
		Vector2 vec = {cosf(angle * DEG2RAD), sinf(angle * DEG2RAD)};
		vec = Vector2Add(dropPoint, Vector2Scale(vec, 45));

		if (PointInMap(vec) && Vector2Distance(vec, Player1.Position) > Player1.PickupDistance)
		{
			item.Position = vec;
			valid = true;
		}

		if (PointInMap(vec) && Vector2Distance(vec, Player2.Position) > Player1.PickupDistance)
		{
			item.Position = vec;
			valid = true;
		}
	}

	auto *sprite = AddSprite(itemRecord->Sprite, item.Position);
	sprite->Shadow = true;
	sprite->Bobble = true;
	item.SpriteId = sprite->Id;

	ItemDrops.push_back(item);
}

void DropLoot(const char *contents, Vector2 &dropPoint)
{
	std::vector<TreasureInstance> loot = GetLoot(contents);
	for (TreasureInstance &item : loot)
	{
		PlaceItemDrop(item, dropPoint);
		AddEffect(item.Position, EffectType::ScaleFade, LootSprite, 1);
	}
}

TreasureInstance PlayerData::RemoveInventoryItem(int slot, int quantity)
{
	TreasureInstance treasure = {-1, 0};

	// is it a valid slot?
	if (slot < 0 || slot >= BackpackContents.size())
		return treasure;

	// can't take more than we have
	InventoryContents &inventory = BackpackContents[slot];
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
		BackpackContents.erase(BackpackContents.begin() + slot);
	}

	// return the drop instance
	return treasure;
}

bool PlayerData::PickupItem(TreasureInstance &drop)
{
	// special case for bag of gold, because it's not a real item
	if (drop.ItemId == GoldBagItem)
	{
		PlaySound(CoinSoundId);
		Gold += drop.Quantity;
		return true;
	}

	// find our item
	Item *item = GetItem(drop.ItemId);

	// it's an invalid item, remove it but nobody gets it
	if (item == nullptr)
		return true;

	// see if this is a weapon, and we are unarmed, if so, equip one
	if (item->IsWeapon() && EquipedWeapon == -1)
	{
		EquipedWeapon = item->Id;
		drop.Quantity--;
		PlaySound(ItemPickupSoundId);
	}

	// see if this is armor, and we are naked, if so, equip one
	if (item->IsArmor() && EquipedArmor == -1)
	{
		EquipedArmor = item->Id;
		drop.Quantity--;
		PlaySound(ItemPickupSoundId);
	}

	// Try to add items to any stacks we already have
	if (drop.Quantity > 0)
	{
		// see if we have any already
		for (InventoryContents &content : BackpackContents)
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
	if (drop.Quantity > 0 && BackpackContents.size() < 20)
	{
		BackpackContents.emplace_back(InventoryContents{item->Id, drop.Quantity});
		drop.Quantity = 0;

		PlaySound(ItemPickupSoundId);
	}

	// if we picked them all up, we can destroy the item
	return drop.Quantity == 0;
}

void PlayerData::Move()
{
	// does the player want to move
	if (TargetActive)
	{
		Vector2 movement = Vector2Subtract(Target, Position);
		float distance = Vector2Length(movement);

		float frameSpeed = GetFrameTime() * Speed;

		if (distance <= frameSpeed)
		{
			Position = Target;
			TargetActive = false;
		}
		else
		{
			movement = Vector2Normalize(movement);
			Vector2 newPos = Vector2Add(Position, Vector2Scale(movement, frameSpeed));

			if (!PointInMap(newPos))
			{
				TargetActive = false;
			}
			else
			{
				Position = newPos;
			}
		}
	}

	// see if the player entered an exit
	for (auto exit : Exits)
	{
		if (CheckCollisionPointRec(Position, exit.Bounds))
		{
			if (exit.Destination == "endgame")
			{
				EndGame(true, Gold + 100);
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

void PlayerData::ApplyActions()
{
	// see if we want to attack any mobs
	if (TargetMob != nullptr)
	{
		// see if we can even attack.
		if (GetGameTime() - LastAttack >= GetAttack().Cooldown)
		{
			float distance = Vector2Distance(TargetMob->Position, Position);
			if (distance < GetAttack().Range + 40)
			{
				MOB *monsterInfo = GetMob(TargetMob->MobId);
				if (monsterInfo != nullptr)
				{
					AddEffect(TargetMob->Position, EffectType::ScaleFade, ClickTargetSprite);
					if (!GetAttack().Melee)
						AddEffect(Position, EffectType::ToTarget, ProjectileSprite, TargetMob->Position, 0.25f);

					int damage = ResolveAttack(GetAttack(), monsterInfo->Defense.Defense);
					if (damage == 0)
					{
						PlaySound(MissSoundId);
					}
					else
					{
						PlaySound(HitSoundId);
						PlaySound(CreatureDamageSoundId);
						AddEffect(Vector2{TargetMob->Position.x, TargetMob->Position.y - 16}, EffectType::RiseFade, DamageSprite);
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
		Vector2 center = {TargetChest->Bounds.x + TargetChest->Bounds.width / 2, TargetChest->Bounds.y + TargetChest->Bounds.height / 2};
        // TODO: player2 cannot open the chest
		float distance = Vector2Distance(center, Position);
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
	for (std::vector<TreasureInstance>::iterator item = ItemDrops.begin(); item != ItemDrops.end();)
	{
		float distance = Vector2Distance(item->Position, Position);
		if (distance <= PickupDistance)
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

	float attackTime = time - LastAttack;
	float itemTime = time - LastConsumeable;

	if (attackTime >= GetAttack().Cooldown)
		AttackCooldown = 0;
	else
		AttackCooldown = 1.0f - (attackTime / AttackCooldown);

	float itemCooldown = 1;

	if (itemTime >= itemCooldown)
		ItemCooldown = 0;
	else
		ItemCooldown = 1.0f - (itemTime / itemCooldown);

	if (BuffLifetimeLeft > 0)
	{
		BuffLifetimeLeft -= GetFrameTime();
		if (BuffLifetimeLeft <= 0)
		{
			BuffDefense = 0;
			BuffItem = -1;
			BuffLifetimeLeft = 0;
		}
	}
}

void CullDeadMobs()
{
	for (std::vector<MobInstance>::iterator mobItr = Mobs.begin(); mobItr != Mobs.end();)
	{
		MOB *monsterInfo = GetMob(mobItr->MobId);
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
	for (auto &mob : Mobs)
	{
		Vector2 vecToPlayer1 = Vector2Subtract(Player1.Position, mob.Position);
		float distance1 = Vector2Length(vecToPlayer1);

		Vector2 vecToPlayer2 = Vector2Subtract(Player2.Position, mob.Position);
		float distance2 = Vector2Length(vecToPlayer2);

		PlayerData *player;
		float distance;

		Vector2 &vecToPlayer = vecToPlayer1;
		if (distance1 < distance2)
		{
			player = &Player1;
			distance = distance1;
		}
		else
		{
			player = &Player2;
			distance = distance2;
			vecToPlayer = vecToPlayer2;
		}

		MOB *monsterInfo = GetMob(mob.MobId);
		if (monsterInfo == nullptr)
			continue;

		if (!mob.Triggered)
		{
			// see if the mob should wake up
			if (distance > monsterInfo->DetectionRadius) // too far away
				continue;

			if (Ray2DHitsMap(player->Position, mob.Position))
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
				if (GetGameTime() - mob.LastAttack >= monsterInfo->Attack.Cooldown)
				{
					mob.LastAttack = GetGameTime();
					int damage = ResolveAttack(monsterInfo->Attack, player->GetDefense());

					if (monsterInfo->Attack.Melee)
						AddEffect(player->Position, EffectType::RotateFade, MobAttackSprite);
					else
						AddEffect(mob.Position, EffectType::ToTarget, ProjectileSprite, player->Position, 0.5f);

					if (damage == 0)
					{
						PlaySound(MissSoundId);
					}
					else
					{
						PlaySound(HitSoundId);
						PlaySound(PlayerDamageSoundId);
						AddEffect(Vector2{player->Position.x, player->Position.y - 16}, EffectType::RiseFade, DamageSprite);
						player->Health -= damage;
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

void PlayerData::UpdateSprite()
{
	if (Sprite != nullptr)
		Sprite->Position = Position;

	if (EquipedArmor == ChainArmorItem)
		Sprite->SpriteFrame = PlayerChainSprite;
	else if (EquipedArmor == PlateArmorItem)
		Sprite->SpriteFrame = PlayerPlateSprite;
	else if (EquipedArmor == LeatherArmorItem)
		Sprite->SpriteFrame = PlayerLeatherSprite;
	else
		Sprite->SpriteFrame = PlayerSprite;
}

void UpdateMobSprites()
{
	for (auto &mob : Mobs)
	{
		UpdateSprite(mob.SpriteId, mob.Position);
	}
}

MobInstance *PlayerData::GetNearestMobInSight(std::vector<MobInstance> &mobs)
{
	MobInstance *nearest = nullptr;
	float nearestDistance = 9999999.9f;

	for (auto &mob : mobs)
	{
		if (Ray2DHitsMap(mob.Position, Position))
			continue;

		float dist = Vector2Distance(mob.Position, Position);

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
	Player1.UpdateSprite();
	Player2.UpdateSprite();
	UpdateMobSprites();
}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		if (GameHud.Player1InventoryOpen || GameHud.Player2InventoryOpen) {
            GameHud.Player1InventoryOpen = false;
            GameHud.Player2InventoryOpen = false;
        }
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
	Player1.Move();
    Player2.Move();

	Player1.ApplyActions();
	Player2.ApplyActions();

	UpdateMobs();

	if (Player1.Health < 0 || Player2.Health < 0)
	{
		// you died, change to the end screen
		EndGame(false, Player1.Gold + Player2.Gold);
	}

	UpdateSprites();

	SetVisiblePoint(Player1.Position);
	SetVisiblePoint(Player2.Position);
}

void PlayerData::UseConsumable(Item *item)
{
	if (item == nullptr || !item->IsActivatable())
		return;

	float time = GetGameTime() - LastConsumeable;
	if (time < 1)
		return;

	LastConsumeable = GetGameTime();

	switch (item->Effect)
	{
	case ActivatableEffects::Healing:
		Health += item->Value;
		if (Health > MaxHealth)
			Health = MaxHealth;

		PlaySound(PlayerHealSoundId);
		AddEffect(Position, EffectType::RiseFade, HealingSprite, 2);
		break;

	case ActivatableEffects::Defense:
		BuffDefense = item->Value;
		BuffLifetimeLeft = item->Durration;
		BuffItem = item->Sprite;
		break;

	case ActivatableEffects::Damage:
	{
		MobInstance *mob = GetNearestMobInSight(Mobs);
		if (mob != nullptr)
		{
			mob->Health -= item->Value;
			PlaySound(CreatureDamageSoundId);
			AddEffect(Position, EffectType::ToTarget, item->Sprite, mob->Position, 1);
			AddEffect(mob->Position, EffectType::RotateFade, item->Sprite, 1);
		}
		break;
	}
	}
}

void PlayerData::ActivateItem(int slotIndex)
{
	if (slotIndex < 0 || slotIndex >= BackpackContents.size())
		return;

	InventoryContents &inventorySlot = BackpackContents[slotIndex];

	Item *item = GetItem(inventorySlot.ItemId);
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
		int weapon = EquipedWeapon;

		// equip new weapon
		EquipedWeapon = removedItem.ItemId;

		// replace the removed item with the old weapon
		removedItem.ItemId = weapon;
		break;
	}

	case ItemTypes::Armor:
	{
		// save our current armor
		int armor = EquipedArmor;

		// equip new weapon
		EquipedArmor = removedItem.ItemId;

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
			PlaceItemDrop(removedItem, Position);
		}
	}
}

void PlayerData::DropItem(int item)
{
	TreasureInstance drop = RemoveInventoryItem(item, 999);
	PlaceItemDrop(drop, Position);
}