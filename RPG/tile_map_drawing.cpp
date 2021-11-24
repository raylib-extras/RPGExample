#include "tile_map.h"
#include "sprites.h"


Rectangle CurrentViewRect = { 0 };


Rectangle GetTileDisplayRect(int x, int y, bool orthographic, const Vector2& tileSize)
{
	if (orthographic)
		return Rectangle{ static_cast<float>(x * tileSize.x), static_cast<float>(y * tileSize.y), tileSize.x, tileSize.y };

	// isometric
	float halfWidth = tileSize.x * 0.5f;
	float halfHeight = tileSize.y * 0.5f;
	float quarterHeight = tileSize.y * 0.25f;

	return Rectangle{ x * halfWidth - y * halfWidth - halfWidth, y * halfHeight + (x * halfHeight), tileSize.x, tileSize.y };
}


const Tile* GetTile(int x, int y, const TileLayer& layer)
{
	if (x < 0 || y < 0 || x >= layer.Size.x || y >= layer.Size.y)
		return nullptr;

	return &layer.Tiles[y * int(layer.Size.x) + x];
}

bool RectInView(const Rectangle& rect)
{
	if (rect.x + rect.width < CurrentViewRect.x)
		return false;

	if (rect.y + rect.height < CurrentViewRect.y)
		return false;

	if (rect.x > CurrentViewRect.x + CurrentViewRect.width)
		return false;

	if (rect.y > CurrentViewRect.y + CurrentViewRect.height)
		return false;

	return true;
}

void DrawTileMap(Camera2D& camera, const TileMap& map)
{
	CurrentViewRect.x = camera.target.x - (camera.offset.x / camera.zoom);
	CurrentViewRect.y = camera.target.y - (camera.offset.y / camera.zoom);

	CurrentViewRect.width = GetScreenWidth() / camera.zoom;
	CurrentViewRect.height = GetScreenHeight() / camera.zoom;

	// iterate the layers, back to front
	for (const auto& layer : map.Layers)
	{
		for (int y = 0; y < int(layer.second.Size.y); ++y)
		{
			for (int x = 0; x < int(layer.second.Size.x); ++x)
			{
				Rectangle destinationRect = GetTileDisplayRect(x, y, map.MapType == TileMapTypes::Orthographic, layer.second.TileSize);
				if (!RectInView(destinationRect))
					continue;

				const Tile* tile = GetTile(x, y, layer.second);
				if (tile == nullptr)
					continue;
				DrawSprite(tile->Sprite, destinationRect.x, destinationRect.y, 0, 1, WHITE, tile->Flip);
			}
		}
	}
}