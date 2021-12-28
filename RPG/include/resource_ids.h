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

// texture IDs
constexpr int TileSetTexture = 0;
constexpr int LogoTexture = 1;

// sprite IDs
// UI sprites
constexpr int BackgroundSprite = 60;
constexpr int ClickTargetSprite = 140;
constexpr int CoinSprite = 141;
constexpr int InventoryBackgroundSprite = 147;
constexpr int ItemBackgroundSprite = 148;
constexpr int AwakeSprite = 149;
constexpr int LootSprite = 150;
constexpr int MobAttackSprite = 114;
constexpr int DamageSprite = 151;
constexpr int HealingSprite = 90;
constexpr int ProjectileSprite = 158;

// equipment sprites
constexpr int BagSprite = 108;
constexpr int SwordSprite = 62;
constexpr int CoolSwordSprite = 154;
constexpr int AwesomeSwordSprite = 155;
constexpr int AxeSprite = 63;
constexpr int MightyAxeSprite = 156;
constexpr int ForkSprite = 66;
constexpr int BowSprite = 64;
constexpr int GoodBowSprite = 157;
constexpr int ClubSprite = 52;
constexpr int LeatherArmorSprite = 142;
constexpr int ChainArmorSprite = 143;
constexpr int PlateArmorSprite = 38;
constexpr int FoodSprite = 122;
constexpr int PotionSprite = 119;
constexpr int ShieldSprite = 94;
constexpr int FireballSprite = 120;

// MOB sprites
constexpr int PlayerSprite = 159;
constexpr int PlayerLeatherSprite = 4;
constexpr int PlayerChainSprite = 5;
constexpr int PlayerPlateSprite = 6;
constexpr int RatSprite = 20;
constexpr int SnakeSprite = 18;
constexpr int GhostSprite = 23;
constexpr int TrollSprite = 11;
constexpr int TurtleSprite = 24;
constexpr int BlobSprite = 22;
constexpr int OgreSprite = 8;
constexpr int MonkSprite = 10;
constexpr int BeholderSprite = 13;

// sound IDs
constexpr int ClickSoundId = 0;
constexpr int CoinSoundId = 1;
constexpr int ChestOpenSoundId = 2;
constexpr int ItemPickupSoundId = 3;
constexpr int AlertSoundId = 4;
constexpr int MissSoundId = 5;
constexpr int HitSoundId = 6;
constexpr int PlayerDamageSoundId = 7;
constexpr int CreatureDamageSoundId = 8;
constexpr int PlayerHealSoundId = 9;

const Texture& GetTexture(int id);