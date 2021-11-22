#pragma once

#include "raylib.h"

void LoadSpriteFrames(int textureId, int colums, int rows, int spacing);
void SetSpriteOrigin(int spriteId, int x, int y);

void DrawSprite(int spriteId, float x, float y, float rotation = 0, float scale = 1, Color tint = { 255, 255, 255, 255 });
void FillRectWithSprite(int spriteId, const Rectangle& rect, Color tint = { 255, 255, 255, 255 });

// sprite IDs
#define BackgroundSprite 60