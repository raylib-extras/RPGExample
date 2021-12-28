#pragma once

#include "raylib.h"

#include <stdint.h>

#define SpriteFlipNone 0
#define SpriteFlipX 0x02
#define SpriteFlipY 0x04
#define SpriteFlipDiagonal 0x08

void LoadSpriteFrames(int textureId, int colums, int rows, int spacing);
void SetSpriteOrigin(int spriteId, int x, int y);
void SetSpriteBorders(int spriteId, int left, int top, int right, int bottom);
void SetSpriteBorders(int spriteId, int inset);
void CenterSprite(int spriteId);

void DrawSprite(int spriteId, float x, float y, float rotation = 0, float scale = 1, Color tint = { 255, 255, 255, 255 }, uint8_t flip = SpriteFlipNone);
void FillRectWithSprite(int spriteId, const Rectangle& rect, Color tint = { 255, 255, 255, 255 }, uint8_t flip = SpriteFlipNone);

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