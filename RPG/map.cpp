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

#include "map.h"

#include "resource_ids.h"
#include "sprites.h"
#include "tile_map.h"
#include "audio.h"

#include "raylib.h"
#include "raymath.h"

#include <math.h>
#include <unordered_map>
#include <list>

struct EffectInstance
{
	Vector2 Position = { 0,0 };
	EffectType Effect = EffectType::Fade;
	int SpriteId = -1;
	float Lifetime = 1;
	float MaxLifetime = 1;
	Vector2 Target = { 0,0 };
};

std::list<EffectInstance> Effects;

Rectangle VisibilityInset = { 200, 200, 200, 250 };
Camera2D MapCamera = { 0 };
TileMap CurrentMap;

std::unordered_map<int, SpriteInstance> SpriteInstances;
int NextSpriteId = 0;

Rectangle MapBounds = { 0,0,0,0 };

Camera2D& GetMapCamera()
{
	return MapCamera;
}

void SetVisiblePoint(const Vector2& point)
{
	Vector2 screenPoint = GetWorldToScreen2D(point, MapCamera);

	if (screenPoint.x < VisibilityInset.x)
		MapCamera.target.x -= VisibilityInset.x - screenPoint.x;

	if (screenPoint.x > GetScreenWidth() -  VisibilityInset.width)
		MapCamera.target.x += screenPoint.x - (GetScreenWidth() - VisibilityInset.width);

	if (screenPoint.y < VisibilityInset.y)
		MapCamera.target.y -= VisibilityInset.y - screenPoint.y;

	if (screenPoint.y > GetScreenHeight() - VisibilityInset.height)
		MapCamera.target.y += screenPoint.y -(GetScreenHeight() - VisibilityInset.height);
}

bool PointInMap(const Vector2& point)
{
	if (!CheckCollisionPointRec(point, MapBounds))
		return false;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == "wall")
			{
				if (CheckCollisionPointRec(point, object->Bounds))
					return false;
			}
		}
	}

	return true;
}

// check to see if a line collides with a rectangle
bool CheckCollisionLineRec(const Vector2& startPoint, const Vector2& endPoint, const Rectangle& rectangle)
{
	// ether point is in the rectangle
	if (CheckCollisionPointRec(startPoint, rectangle) || CheckCollisionPointRec(endPoint, rectangle))
		return true;

	// top
	if (CheckCollisionLines(startPoint, endPoint, Vector2{ rectangle.x,rectangle.y }, Vector2{ rectangle.x + rectangle.width, rectangle.y }, nullptr))
		return true;

	// right
	if (CheckCollisionLines(startPoint, endPoint, Vector2{ rectangle.x + rectangle.width,rectangle.y }, Vector2{ rectangle.x + rectangle.width, rectangle.y + rectangle.height }, nullptr))
		return true;

	// bottom
	if (CheckCollisionLines(startPoint, endPoint, Vector2{ rectangle.x, rectangle.y + rectangle.height }, Vector2{ rectangle.x + rectangle.width, rectangle.y + rectangle.height }, nullptr))
		return true;

	// left
	if (CheckCollisionLines(startPoint, endPoint, Vector2{ rectangle.x,rectangle.y }, Vector2{ rectangle.x, rectangle.y + rectangle.height }, nullptr))
		return true;

	return false;
}

bool Ray2DHitsMap(const Vector2& startPoint, const Vector2& endPoint)
{
	if (!PointInMap(startPoint) || !PointInMap(endPoint))
		return true;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == "wall")
			{
				if (CheckCollisionLineRec(startPoint, endPoint, object->Bounds))
					return true;
			}
		}
	}

	return false;
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

	MapBounds = Rectangle{ 0,0,0,0 };

	if (!CurrentMap.TileLayers.empty())
	{
		int index = CurrentMap.TileLayers.rbegin()->first;

		MapBounds.width = (CurrentMap.TileLayers[index]->Size.x * CurrentMap.TileLayers[index]->TileSize.x);
		MapBounds.height = (CurrentMap.TileLayers[index]->Size.y * CurrentMap.TileLayers[index]->TileSize.y);

		MapCamera.target.x = MapBounds.width / 2;
		MapCamera.target.y = MapBounds.height / 2;
	}

	const auto* bgm = CurrentMap.GetProperty("bgm");
	if (bgm)
	{
		StopBGM();
		StartBGM(bgm->GetString());
	}
}

void ClearMap()
{
	CurrentMap.ObjectLayers.clear();
	CurrentMap.TileLayers.clear();
	ClearSprites();
	Effects.clear();
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
		{
			float offset = 0;
			if (sprite.Bobble)
				offset =  fabsf(sinf(float(GetTime() * 5)) * 3);

			if (sprite.Shadow)
				DrawSprite(sprite.SpriteFrame, sprite.Position.x+2, sprite.Position.y+2 + offset, 0.0f, 1.0f, ColorAlpha(BLACK,0.5f));

			DrawSprite(sprite.SpriteFrame, sprite.Position.x, sprite.Position.y + offset, 0.0f, 1.0f, sprite.Tint);
		}
	}

	for (std::list<EffectInstance>::iterator effect = Effects.begin(); effect != Effects.end();)
	{
		effect->Lifetime -= GetFrameTime();
		
		if (effect->Lifetime < 0)
		{
			effect = Effects.erase(effect);
			continue;
		}

		float param = effect->Lifetime / effect->MaxLifetime;
		float rotation = 0;
		float alpha = 1;
		float scale = 1;

		Vector2 pos = effect->Position;

		switch (effect->Effect)
		{
		case EffectType::Fade:
			alpha = param;
			break;

		case EffectType::RiseFade:
			alpha = param;
			pos.y -= (1.0f - param) * 30;
			break;

		case EffectType::RotateFade:
			rotation = (1.0f - param) * 360;
			alpha = param;
			break;

		case EffectType::ScaleFade:
			alpha = param;
			scale = 1 + (1.0f - param);
			break;

		case EffectType::ToTarget:
		{
			Vector2 vec = Vector2Subtract(effect->Target, effect->Position);
			float dist = Vector2Length(vec);
			vec = Vector2Normalize(vec);

			pos = Vector2Add(effect->Position, Vector2Scale(vec, dist * (1.0f - param)));
			break;
		}
		}

		DrawSprite(effect->SpriteId, pos.x, pos.y, rotation, scale, ColorAlpha(WHITE, alpha));

		effect++;
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

void UpdateSprite(int spriteId, const Vector2& position)
{
	auto itr = SpriteInstances.find(spriteId);
	if (itr == SpriteInstances.end())
		return;

	itr->second.Position = position;
}

void RemoveSprite(SpriteInstance* sprite)
{
	if (sprite != nullptr)
		RemoveSprite(sprite->Id);
}

void RemoveSprite(int spriteId)
{
	auto itr = SpriteInstances.find(spriteId);
	if (itr != SpriteInstances.end())
		SpriteInstances.erase(itr);
}

void ClearSprites()
{
	SpriteInstances.clear();
	NextSpriteId = 0;
}

void AddEffect(const Vector2& position, EffectType effect, int spriteId, float lifetime)
{
	CenterSprite(spriteId);
	Effects.emplace_back(EffectInstance{ position,effect,spriteId,lifetime,lifetime });
}

void AddEffect(const Vector2& position, EffectType effect, int spriteId, const Vector2& target, float lifetime)
{
	CenterSprite(spriteId);
	Effects.emplace_back(EffectInstance{ position, effect, spriteId, lifetime, lifetime, target });
}