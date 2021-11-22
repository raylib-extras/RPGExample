#include "sprites.h"
#include "resource_ids.h"

#include "raylib.h"

#include <vector>

struct SpriteInfo
{
	int TextureId = -1;
	Rectangle SourceRect = { 0,0,0,0 };
	Vector2 Origin = { 0,0 };
};

std::vector<SpriteInfo> Sprites;

void LoadSpriteFrames(int textureId, int columns, int rows, int spacing)
{
	if (columns == 0 || rows == 0)
		return;

	const Texture2D& texture = GetTexture(textureId);

	int itemWidth = (texture.width + spacing) / columns;
	int itemHeight = (texture.height + spacing) / rows;

	SpriteInfo info;
	info.TextureId = textureId;
	info.SourceRect.width = float(itemWidth - spacing);
	info.SourceRect.height = float(itemHeight - spacing);

	for (int y = 0; y < rows; ++y)
	{
		info.SourceRect.x = 0;
		for (int x = 0; x < columns; ++x)
		{
			Sprites.push_back(info);

			info.SourceRect.x += itemWidth;
		}

		info.SourceRect.y += itemHeight;
	}
}

void SetSpriteOrigin(int spriteId, int x, int y)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];
	sprite.Origin.x = float(x);
	sprite.Origin.y = float(y);
}

void DrawSprite(int spriteId, float x, float y, float rotation, float scale, Color tint)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];

	Rectangle destination = { x, y, sprite.SourceRect.width * scale,sprite.SourceRect.height * scale };

	DrawTexturePro(GetTexture(sprite.TextureId), sprite.SourceRect, destination, sprite.Origin, rotation, tint);
}

void FillRectWithSprite(int spriteId, const Rectangle& rect, Color tint)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];

	DrawTextureTiled(GetTexture(sprite.TextureId), sprite.SourceRect, rect, Vector2{ 0, 0 }, 0, 1, tint);
}