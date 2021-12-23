#pragma once

#include "raylib.h"

void LoadMap(const char* file);
void ClearMap();
void DrawMap();

Camera2D& GetMapCamera();