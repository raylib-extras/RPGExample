#include "game_hud.h"
#include "raylib.h"

GameHudScreen::GameHudScreen(PlayerData& player) 
	: Screen()
	, Player(player)
{

}

void GameHudScreen::Draw()
{
	DrawRectangle(0, GetScreenHeight() - 80, GetScreenWidth(), 80, ColorAlpha(BLACK, 0.5f));
	DrawSprite(CoinSprite, GetScreenWidth() - 200, GetScreenHeight() - 60, 4);
	DrawText("x 000", GetScreenWidth() - 170, GetScreenHeight() - 80, 40, WHITE);
}