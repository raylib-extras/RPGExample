#pragma once

#include "screens.h"
#include "game.h"
#include "items.h"

class GameHudScreen : public Screen
{
public:
	GameHudScreen(PlayerData& player);

	void Draw() override;

	PlayerData& Player;

private:
	bool DrawButton(float x, float y, int itemId = -1, int quantity = 1, Color border = BROWN, Color center = BEIGE);
	void DrawInventory();	
	void ShowItemToolTip(const Item* item, const Rectangle& rect);

private:
	float ButtonSize = 70;
	float ButtonInset = 6;

	bool InventoryOpen = false;

	const Item* HoveredItem = nullptr;
};
