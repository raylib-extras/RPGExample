#include "game.h"
#include "main.h"

#include "raylib.h"

void InitGame()
{

}

void UpdateGame()
{
	if (IsKeyPressed(KEY_ESCAPE))
		PauseGame();
}
