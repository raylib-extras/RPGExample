#include "raylib.h"

void main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280,800,"RPG Example");
	SetTargetFPS(144);


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();
}