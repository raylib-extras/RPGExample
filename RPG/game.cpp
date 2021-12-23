#include "game.h"
#include "main.h"
#include "map.h"

#include "raylib.h"

// game state data

SpriteInstance* Player = nullptr;


void LoadLevel(const char* level)
{
	LoadMap(level);
	Player = AddSprite(PlayerSprite, Vector2{ 0,0 });
}

void InitGame()
{
	// load background world
	LoadLevel("resources/maps/level0.tmx");

	auto* spawn = GetFirstMapObjectOfType(PlayerSpawnType);
	if (spawn != nullptr)
	{
		Player->Position.x = spawn->Bounds.x;
		Player->Position.y = spawn->Bounds.y;
	}
}

void QuitGame()
{
	ClearMap();
}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
		PauseGame();
}
