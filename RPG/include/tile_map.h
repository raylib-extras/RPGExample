#pragma once

#include "sprites.h"

#include "raylib.h"

#include <stdint.h>
#include <vector>
#include <map>


enum class TileMapTypes
{
	Orthographic,
	Isometric,
};


struct Tile
{
	int16_t Sprite = -1;
	uint8_t Flip = SpriteFlipNone;
};

struct TileLayer
{
	int Id;
	Vector2 Size = { 0,0 };
	Vector2 TileSize = { 0,0 };

	std::vector<Tile> Tiles;
};

class TileMap
{
public:
	TileMapTypes MapType = TileMapTypes::Orthographic;

	std::map<int, TileLayer> Layers;
};

bool ReadTileMap(const char* filePath, TileMap& map);

void DrawTileMap(Camera2D& camera, const TileMap& map);