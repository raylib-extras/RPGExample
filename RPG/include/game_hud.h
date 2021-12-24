#pragma once

#include "screens.h"
#include "game.h"

class GameHudScreen : public Screen
{
public:
	GameHudScreen(PlayerData& player);

	void Draw() override;

	PlayerData& Player;
};
