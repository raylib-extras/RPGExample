#pragma once

#include "raylib.h"

void LoadMap(const char* file);
void DrawMap();

Camera2D& GetMapCamera();