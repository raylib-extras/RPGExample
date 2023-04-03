/**********************************************************************************************
*
*   Raylib RPG Example * A simple RPG made using raylib
*
*    LICENSE: zlib/libpng
*
*   Copyright (c) 2020 Jeffery Myers
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "sprites.h"
#include "resource_ids.h"

#include "raylib.h"
#include "raymath.h"

#include <vector>

struct SpriteInfo
{
	int TextureId = -1;
	Rectangle SourceRect = { 0,0,0,0 };
	Vector2 Origin = { 0,0 };

	Rectangle Borders = { 0,0,0,0 };
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

void SetSpriteBorders(int spriteId, int left, int top, int right, int bottom)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];
	sprite.Borders.x = float(left);
	sprite.Borders.y = float(top);

	sprite.Borders.width = float(right);
	sprite.Borders.height = float(bottom);
}

void SetSpriteBorders(int spriteId, int inset)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];
	sprite.Borders.x = float(inset);
	sprite.Borders.y = float(inset);

	sprite.Borders.width = float(sprite.SourceRect.width - inset);
	sprite.Borders.height = float(sprite.SourceRect.height - inset);
}

void CenterSprite(int spriteId)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];
	sprite.Origin.x = sprite.SourceRect.width/2;
	sprite.Origin.y = sprite.SourceRect.height/2;
}

void DrawSprite(int spriteId, float x, float y, float rotation, float scale, Color tint, uint8_t flip)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];

	Rectangle source = sprite.SourceRect;

	if (flip & SpriteFlipDiagonal)
		rotation -= 90;
	if (flip & SpriteFlipX)
		source.width *= -1;
	if (flip & SpriteFlipY)
		source.height *= -1;

	Rectangle destination = { x, y, sprite.SourceRect.width * scale,sprite.SourceRect.height * scale };

	if (flip & SpriteFlipDiagonal)
		destination.y += destination.height;

	DrawTexturePro(GetTexture(sprite.TextureId), source, destination, Vector2Scale(sprite.Origin,scale), rotation, tint);
}

void FillRectWithSprite(int spriteId, const Rectangle& rect, Color tint, uint8_t flip)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];

	Rectangle source = sprite.SourceRect;
	float rotation = 0;

	if (flip && SpriteFlipDiagonal == 0)
		rotation += 90;
	if (flip && SpriteFlipX == 0)
		source.width *= -1;
	if (flip && SpriteFlipY == 0)
		source.height *= -1;

	if (sprite.Borders.width != 0 || sprite.Borders.height != 0)
	{
		NPatchInfo info;
		info.source = source;
		info.left = int(sprite.Borders.x);
		info.right = int(sprite.Borders.width);
		info.top = int(sprite.Borders.y);
		info.bottom = int(sprite.Borders.height);
		info.layout = NPATCH_NINE_PATCH;

		DrawTextureNPatch(GetTexture(sprite.TextureId), info, rect, Vector2{ 0,0 }, rotation, tint);
	}
	else
	{
		Rectangle source = sprite.SourceRect;
		float rotation = 0;

		if (flip && SpriteFlipDiagonal == 0)
			rotation += 90;
		if (flip && SpriteFlipX == 0)
			source.width *= -1;
		if (flip && SpriteFlipY == 0)
			source.height *= -1;

		int yCount = int(floor(rect.height / sprite.SourceRect.height));
		int xCount = int(floor(rect.width / sprite.SourceRect.width));

		Rectangle destRect = { 0, 0, sprite.SourceRect.width, sprite.SourceRect.height };

		for (int y = 0; y < yCount; y++)
		{
			// full rows
			for (int x = 0; x < xCount; x++)
			{
				destRect.x = rect.x + (x * destRect.width);
				destRect.y = rect.y + (y * destRect.height);

				DrawTexturePro(GetTexture(sprite.TextureId), source, destRect, Vector2Zero(), 0, tint);
			}

			// remainder column
			Rectangle remainderRect;
			remainderRect.x = rect.x + (xCount * destRect.width);
			remainderRect.y = rect.y + (y * destRect.height);

			remainderRect.width = (rect.x + rect.width) - remainderRect.x;
			remainderRect.height = destRect.height;
			if(remainderRect.width > 0)
				DrawTexturePro(GetTexture(sprite.TextureId), source, remainderRect, Vector2Zero(), 0, tint);
		}

		destRect = { 0, (yCount * destRect.height), sprite.SourceRect.width, (rect.y + rect.height) - (yCount * destRect.height) };

		if (destRect.height > 0)
		{
			// remainder row
			for (int x = 0; x < xCount; x++)
			{
				destRect.x = rect.x + (x * destRect.width);
				DrawTexturePro(GetTexture(sprite.TextureId), source, destRect, Vector2Zero(), 0, tint);
			}

			// last item
			destRect.x = (xCount * destRect.width);
			destRect.width = (rect.x + rect.width) - destRect.x;

			if (destRect.x > 0)
				DrawTexturePro(GetTexture(sprite.TextureId), source, destRect, Vector2Zero(), 0, tint);
		}
	}
}