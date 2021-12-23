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
		if (layer.second->IsObject)
		{
			const ObjectLayer& objectLayer = *(static_cast<ObjectLayer*>(layer.second.get()));

			for (const auto& object : objectLayer.Objects)
			{
				if (object->SubType == TileObject::SubTypes::Text)
				{
					const TileTextObject* textObject = static_cast<TileTextObject*>(object.get());
					DrawText(textObject->Text.c_str(), textObject->Bounds.x, textObject->Bounds.y, textObject->FontSize, textObject->TextColor);
				}
			}
		}
		else
		{
			const TileLayer& tileLayer = *(static_cast<TileLayer*>(layer.second.get()));

			for (int y = 0; y < int(tileLayer.Size.y); ++y)
			{
				for (int x = 0; x < int(tileLayer.Size.x); ++x)
				{
					Rectangle destinationRect = GetTileDisplayRect(x, y, map.MapType == TileMapTypes::Orthographic, tileLayer.TileSize);
					if (!RectInView(destinationRect))
						continue;

					const Tile* tile = GetTile(x, y, tileLayer);
					if (tile == nullptr)
						continue;
					DrawSprite(tile->Sprite, destinationRect.x, destinationRect.y, 0, 1, WHITE, tile->Flip);
				}
			}
		}
	}
}