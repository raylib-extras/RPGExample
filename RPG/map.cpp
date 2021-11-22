#include "map.h"

#include "resource_ids.h"
#include "sprites.h"

#include "raylib.h"

void DrawMap()
{
	Rectangle rect = { 0,0, float(GetScreenWidth()), float(GetScreenHeight()) };

	FillRectWithSprite(BackgroundSprite, rect);
}