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

#include "screens.h"
#include "game.h"
#include "items.h"

class GameHudScreen : public Screen
{
public:
	GameHudScreen(PlayerData& player);

	void Draw() override;

	PlayerData& Player;

	bool IsUiClick(const Vector2& pos);

	bool InventoryOpen = false;

private:
	bool DrawButton(float x, float y, int itemId = -1, int quantity = 1, Color border = BROWN, Color center = BEIGE);
	void DrawInventory();	
	void ShowItemToolTip(const Item* item, const Rectangle& rect);

private:
	float ButtonSize = 70;
	float ButtonInset = 6;

	const Item* HoveredItem = nullptr;
};
