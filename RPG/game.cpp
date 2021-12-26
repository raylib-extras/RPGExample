#include "game.h"
#include "main.h"
#include "map.h"
#include "game_hud.h"
#include "items.h"
#include "treasure.h"

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

PlayerData Player;
std::vector<Exit> Exits;
std::vector<Chest> Chests;
Chest* TargetChest = nullptr;

std::vector<TreasureInstance> ItemDrops;

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
	auto* spawn = GetFirstMapObjectOfType(PlayerSpawnType);
	if (spawn != nullptr)
	{
		Player.Position.x = spawn->Bounds.x;
		Player.Position.y = spawn->Bounds.y;
	}

	Player.TargetActive = false;

	Exits.clear();
	for (const TileObject* exit : GetMapObjectsOfType("exit"))
	{
		const TileObject::Property* level = exit->GetProperty("target_level");
		if (level != nullptr)
			Exits.emplace_back(Exit{ exit->Bounds,"level" + level->Value + ".tmx" });
	}

	Chests.clear();
	TargetChest = nullptr;
	for (const TileObject* chest : GetMapObjectsOfType("chest"))
	{
		const TileObject::Property* contents = chest->GetProperty("contents");
		if (contents != nullptr)
			Chests.emplace_back(Chest{ chest->Bounds, contents->Value });
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

void GetMoveInput()
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
	}
}

void OpenChest(Chest* chest, Vector2& dropPoint)
{
	if (chest->Opened)
		return;

	chest->Opened = true;

	std::vector<TreasureInstance> loot = GetLoot(chest->Contents);
	for (TreasureInstance& item : loot)
	{
		float angle = float(GetRandomValue(-180, 180));
		Vector2 vec = { cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
		vec = Vector2Scale(vec, 20);

		item.Position = Vector2Add(dropPoint, vec);

		Item* itemRecord = GetItem(item.ItemId);
		if (!itemRecord)
			continue;

		auto* sprite = AddSprite(itemRecord->Sprite, item.Position);
		sprite->Shadow = true;
		sprite->Bobble = true;
		item.SpriteId = sprite->Id;

		ItemDrops.emplace_back(std::move(item));
	}
}

bool PickupItem(TreasureInstance& drop)
{
	// special case for bag of gold, because it's not a real item
	if (drop.ItemId == GoldBagItem)
	{
		Player.Gold += drop.Quantity;
		return true;
	}

	// find our item 
	Item* item = GetItem(drop.ItemId);

	// it's an invalid item, remove it but nobody gets it
	if (item == nullptr)
		return true;

	// see if this is a weapon, and we are unarmed, if so, equip one
	if (item->IsWeapon && Player.EquipedWeapon == -1)
	{
		Player.EquipedWeapon = item->Id;
		drop.Quantity--;
	}

	// see if this is armor, and we are naked, if so, equip one
	if (item->IsArmor && Player.EquipedArmor == -1)
	{
		Player.EquipedArmor = item->Id;
		drop.Quantity--;
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
				break;
			}
		}
	}

	// Try to add items to a new inventory slot
	if (drop.Quantity > 0 && Player.BackpackContents.size() < 20 )
	{
		Player.BackpackContents.emplace_back(InventoryContents{item->Id,drop.Quantity });
		drop.Quantity = 0;
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

	// see if the player is near the last clicked chest, if so open it
	if (TargetChest != nullptr)
	{
		Vector2 center = { TargetChest->Bounds.x + TargetChest->Bounds.width / 2,TargetChest->Bounds.y + TargetChest->Bounds.height / 2 };

		float distance = Vector2Distance(center, Player.Position);
		if (distance <= 40)
		{
			OpenChest(TargetChest, center);
			TargetChest = nullptr;
		}
	}

	// see if we are under any items to pickup
	for (std::vector<TreasureInstance>::iterator item = ItemDrops.begin(); item != ItemDrops.end(); )
	{
		float distance = Vector2Distance(item->Position, Player.Position);
		if (distance <= 20)
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

void UpdateSprites()
{
	UpdatePlayerSprite();
}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
		PauseGame();

	GetMoveInput();
	MovePlayer();

	UpdateSprites();
}
