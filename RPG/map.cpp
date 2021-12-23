#include "map.h"

#include "resource_ids.h"
#include "sprites.h"
#include "tile_map.h"

#include "raylib.h"

#include <unordered_map>

Camera2D MapCamera = { 0 };
TileMap CurrentMap;

std::unordered_map<int, SpriteInstance> SpriteInstances;
int NextSpriteId = 0;

Camera2D& GetMapCamera()
{
	return MapCamera;
}

void LoadMap(const char* file)
{
	ClearSprites();
	ReadTileMap(file, CurrentMap);

	MapCamera.offset.x = GetScreenWidth() * 0.5f;
	MapCamera.offset.y = GetScreenHeight() * 0.5f;

	MapCamera.rotation = 0;
	MapCamera.zoom = 1;

	MapCamera.target.x = 0;
	MapCamera.target.y = 0;

	if (!CurrentMap.TileLayers.empty())
	{
		int index = CurrentMap.TileLayers.rbegin()->first;
		MapCamera.target.x = (CurrentMap.TileLayers[index]->Size.x * CurrentMap.TileLayers[index]->TileSize.x) / 2;
		MapCamera.target.y = (CurrentMap.TileLayers[index]->Size.y * CurrentMap.TileLayers[index]->TileSize.y) / 2;
	}
}

void ClearMap()
{
	CurrentMap.ObjectLayers.clear();
	CurrentMap.TileLayers.clear();
	ClearSprites();
}

void DrawMap()
{
	if (CurrentMap.TileLayers.empty())
		return;

	BeginMode2D(GetMapCamera());
	DrawTileMap(MapCamera, CurrentMap);

	for (const auto& entry : SpriteInstances)
	{
		const SpriteInstance& sprite = entry.second;
		if (sprite.Active)
			DrawSprite(sprite.SpriteFrame, sprite.Position.x, sprite.Position.y, 0.0f, 1.0f, sprite.Tint);
	}
	EndMode2D();
}

std::vector<const TileObject*> GetMapObjectsOfType(const char* objType, TileObject::SubTypes requiredType)
{
	std::vector<const TileObject*> objects;
	if (CurrentMap.ObjectLayers.empty())
		return objects;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == objType && (requiredType == TileObject::SubTypes::None || object->SubType == requiredType))
				objects.push_back(object.get());
		}
	}

	return objects;
}

const TileObject* GetFirstMapObjectOfType(const char* objType, TileObject::SubTypes requiredType)
{
	std::vector<const TileObject*> objects;
	if (CurrentMap.ObjectLayers.empty())
		return nullptr;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == objType && (requiredType == TileObject::SubTypes::None || object->SubType == requiredType))
				return object.get();
		}
	}

	return nullptr;
}

SpriteInstance* AddSprite(int frame, const Vector2& position)
{
	NextSpriteId++;
	return &(SpriteInstances.insert_or_assign(NextSpriteId, SpriteInstance{ NextSpriteId, true, frame, position }).first->second);
}

void RemoveSprite(SpriteInstance* sprite)
{
	if (sprite != nullptr)
		RemoveSprite(sprite->Id);
}

void RemoveSprite(int id)
{
	auto itr = SpriteInstances.find(id);
	if (itr != SpriteInstances.end())
		SpriteInstances.erase(itr);
}

void ClearSprites()
{
	SpriteInstances.clear();
	NextSpriteId = 0;
}