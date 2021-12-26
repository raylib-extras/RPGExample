#pragma once

#include "sprites.h"

#include "raylib.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

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

class Layer
{
public:
	int Id;
	std::string Name;
	Vector2 Size = { 0,0 };

	bool IsObject = false;
};

class TileLayer : public Layer
{
public:
	Vector2 TileSize = { 0,0 };
	std::vector<Tile> Tiles;
};

class TileObject
{
public:
	int ID = 0;
	std::string Name;
	Rectangle Bounds = { 0,0,0,0 };

	bool Visible = true;
	std::string Type;

	float Rotation = 0;
	int GridTile = -1;

	std::string Template;

	enum class SubTypes
	{
		None,
		Ellipse,
		Point,
		Polygon,
		Polyline,
		Text,
	};

	SubTypes SubType = SubTypes::None;

	class Property
	{
	public:
		std::string Name;
		std::string Type;
		std::string Value;

		inline int GetInt() const
		{
			if (Type != "int" || Value.empty())
				return 0;

			return atoi(Value.c_str());
		}

		inline float GetFloat() const
		{
			if (Type != "float" || Value.empty())
				return 0;

			return float(atof(Value.c_str()));
		}

		inline const char* GetString() const
		{
			return Value.c_str();
		}
	};

	std::vector<Property> Properties;

	inline const Property* GetProperty(const char* name) const
	{
		for (const auto& prop : Properties)
		{
			if (prop.Name == name)
				return &prop;
		}
		return nullptr;
	}
};

class TilePolygonObject : public TileObject
{
public:
	std::vector<Vector2> Points;
};

class TileTextObject : public TileObject
{
public:
	std::string Text;
	Color TextColor = WHITE;
	bool Wrap = false;

	int FontSize = 20;
	std::string FontFamily;
	bool Bold = false;
	bool Italic = false;
	bool Underline = false;
	bool Strikeout = false;
	bool Kerning = true;
	std::string HorizontalAlignment = "left";
	std::string VerticalAlignment = "top";
};

class ObjectLayer : public Layer
{
public:
	ObjectLayer() { IsObject = true; }

	std::vector<std::shared_ptr<TileObject>> Objects;
};

class TileMap
{
public:
	TileMapTypes MapType = TileMapTypes::Orthographic;

	std::map<int, std::shared_ptr<Layer>> Layers;

	std::map<int, TileLayer*> TileLayers;
	std::map<int, ObjectLayer*> ObjectLayers;
};

bool ReadTileMap(const char* filePath, TileMap& map);

void DrawTileMap(Camera2D& camera, const TileMap& map);