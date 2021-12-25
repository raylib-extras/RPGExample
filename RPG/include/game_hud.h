#pragma once

#include "screens.h"
#include "game.h"

class GameHudScreen : public Screen
{
public:
	GameHudScreen(PlayerData& player);

	void Draw() override;

	PlayerData& Player;

private:
	void DrawButton(float x, float y, int sprite = -1, Color border = BROWN, Color center = BEIGE);

private:
	float ButtonSize = 70;
	float ButtonInset = 6;
};
