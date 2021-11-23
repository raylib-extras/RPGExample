#pragma once

#include <stdint.h>
#include <vector>
#include <map>

#include "raylib.h"

enum class TileMapTypes
{
	Orthographic,
	Isometric,
};

enum class TileFlipMasks
{
	None = 0,
	X = 0x02,
	Y = 0x04,
	Diagonal = 0x08
};

struct Tile
{
	int16_t Sprite = -1;
	TileFlipMasks Flip = TileFlipMasks::None;
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

	bool Read(const char* filePath);
};