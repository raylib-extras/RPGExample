#include "game.h"
#include "main.h"
#include "map.h"

#include "raylib.h"

void InitGame()
{
	// load background world
	LoadMap("resources/maps/level0.tmx");
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
