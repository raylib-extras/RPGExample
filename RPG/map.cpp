#include "map.h"

#include "resource_ids.h"
#include "sprites.h"
#include "tile_map.h"

#include "raylib.h"

Camera2D MapCamera = { 0 };
TileMap CurrentMap;

Camera2D& GetMapCamera()
{
	return MapCamera;
}

void LoadMap(const char* file)
{
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
		MapCamera.target.x = (CurrentMap.TileLayers[index].Size.x * CurrentMap.TileLayers[index].TileSize.x) / 2;
		MapCamera.target.y = (CurrentMap.TileLayers[index].Size.y * CurrentMap.TileLayers[index].TileSize.y) / 2;
	}
}

void ClearMap()
{
	CurrentMap.ObjectLayers.clear();
	CurrentMap.TileLayers.clear();
}

void DrawMap()
{
	if (CurrentMap.TileLayers.empty())
		return;

	BeginMode2D(GetMapCamera());
	DrawTileMap(MapCamera, CurrentMap);
	EndMode2D();
}