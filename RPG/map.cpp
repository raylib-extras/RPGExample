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

	MapCamera.offset.x = GetScreenWidth() / 2;
	MapCamera.offset.y = GetScreenHeight() / 2;

	MapCamera.rotation = 0;
	MapCamera.zoom = 1;

	MapCamera.target.x = 0;
	MapCamera.target.y = 0;

	if (!CurrentMap.Layers.empty())
	{
		int index = int(CurrentMap.Layers.size()) - 1;
		MapCamera.target.x = (CurrentMap.Layers[index].Size.x * CurrentMap.Layers[index].TileSize.x) / 2;
		MapCamera.target.y = (CurrentMap.Layers[index].Size.y * CurrentMap.Layers[index].TileSize.y) / 2;
	}
}

void DrawMap()
{
	if (CurrentMap.Layers.empty())
		return;

	BeginMode2D(GetMapCamera());
	DrawTileMap(MapCamera, CurrentMap);
	EndMode2D();
}