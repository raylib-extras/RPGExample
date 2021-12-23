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
PlayerData Player;


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
}

void InitGame()
{
	// load background world
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

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
		PauseGame();

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
			Player.Position = Vector2Add(Player.Position, Vector2Scale(movement, frameSpeed));
		}
	}

	SetSpritePos(Player.Sprite, Player.Position);

	if (Player.TargetSprite != nullptr)
	{
		Player.TargetSprite->Active = Player.TargetActive;
		Player.TargetSprite->Position = Player.Target;
	}
}
