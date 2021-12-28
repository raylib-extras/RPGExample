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

#pragma once

#include "raylib.h"
#include "tile_map.h"

#include <vector>

// map basics
void LoadMap(const char* file);
void ClearMap();
void DrawMap();

Camera2D& GetMapCamera();

void SetVisiblePoint(const Vector2& point);

// tile map objects
std::vector<const TileObject*> GetMapObjectsOfType(const char* objType, TileObject::SubTypes requiredType = TileObject::SubTypes::None);
const TileObject* GetFirstMapObjectOfType(const char* objType, TileObject::SubTypes requiredType = TileObject::SubTypes::None);

// map collisions
bool PointInMap(const Vector2& point);
bool Ray2DHitsMap(const Vector2& startPoint, const Vector2& endPoint);

// map sprites
struct SpriteInstance
{
	int Id = -1;
	bool Active = true;
	int SpriteFrame = -1;
	Vector2 Position = { 0,0 };
	Color Tint = WHITE;
	bool Bobble = false;
	bool Shadow = true;
};

SpriteInstance* AddSprite(int frame, const Vector2& position);
void UpdateSprite(int spriteId, const Vector2& position);
void RemoveSprite(SpriteInstance* sprite);
void RemoveSprite(int id);
void ClearSprites();

// Effects
enum class EffectType
{
	Fade,
	RiseFade,
	RotateFade,
	ScaleFade,
	ToTarget,
};
void AddEffect(const Vector2& position, EffectType effect, int spriteId, float lifetime = 1);
void AddEffect(const Vector2& position, EffectType effect, int spriteId, const Vector2& target, float lifetime = 1);

// common object types
constexpr char PlayerSpawnType[] = "player_spawn";
constexpr char MobSpawnType[] = "mob_spawn";
constexpr char ChestType[] = "chest";
constexpr char ExitType[] = "exit";