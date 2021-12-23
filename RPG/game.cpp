#include "game.h"
#include "main.h"
#include "map.h"

#include "raylib.h"
#include "raymath.h"

// game state data

struct PlayerData
{
	Vector2 Position = { 0,0 };
	SpriteInstance* Sprite = nullptr;

	bool TargetActive = false;
	Vector2 Target = { 0, 0 };
	SpriteInstance* TargetSprite = nullptr;

	float Speed = 100;
};

struct Exit
{
	Rectangle Bounds;
	std::string Destination;
};

PlayerData Player;
std::vector<Exit> Exits;

void LoadLevel(const char* level)
{
	LoadMap(level);
	Player.TargetSprite = AddSprite(ClickTargetSprite, Player.Target);
	Player.TargetSprite->Tint = ColorAlpha(Player.TargetSprite->Tint, 0.5f);

	Player.Sprite = AddSprite(PlayerSprite, Player.Position);
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
		std::string level;
		for (auto prop : exit->Properties)
		{
			if (prop.Name == "target_level")
			{
				level = prop.Value;
			}
		}

		Exits.emplace_back(Exit{ exit->Bounds,"level" + level + ".tmx" });
	}
}

void InitGame()
{
	// load start level
	LoadLevel("resources/maps/level0.tmx");
	StartLevel();
}

void QuitGame()
{
	ClearMap();
}

void SetSpritePos(SpriteInstance* sprite, Vector2 pos)
{
	if (sprite != nullptr)
	{
		sprite->Position = pos;
		sprite->Position.y += fabsf(sinf(float(GetTime() * 5)) * 3);
	}
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
	}
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

void UpdatePlayerSprite()
{
	SetSpritePos(Player.Sprite, Player.Position);

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
