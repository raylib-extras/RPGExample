#include "game_hud.h"
#include "items.h"

#include "raylib.h"

GameHudScreen::GameHudScreen(PlayerData& player) 
	: Screen()
	, Player(player)
{

}

void GameHudScreen::Draw()
{
	float barHieght = GetScreenHeight() - 80.0f;
	// background
	DrawRectangleRec(Rectangle{ 0, barHieght, float(GetScreenWidth()), 80 }, ColorAlpha(DARKGRAY, 0.25f));

	// score
	DrawSprite(CoinSprite, GetScreenWidth() - 200.0f, barHieght + 40.0f, 4);
	DrawText(TextFormat("x %03d", Player.Gold), GetScreenWidth() - 170, int(barHieght + 20), 40, WHITE);

	// health bar
	DrawText("Health", 20, int(barHieght + 5), 20, RED);

	float healthBarWidth = 300;
	DrawRectangleLinesEx(Rectangle{ 20, barHieght + 30, healthBarWidth, 32 }, 1, WHITE);

	float healthPram = Player.Health / float(Player.MaxHealth);
	DrawRectangleRec(Rectangle{ 22,barHieght + 32,healthBarWidth * healthPram - 4,28 }, RED);

	// action buttons
	float buttonX = 20 + healthBarWidth + 10;
	float buttonY = barHieght + 4;

	// equipped weapon
	DrawButton(buttonX, buttonY, Player.EquipedWeapon, DARKGRAY, GRAY);


	std::vector<int> activatableItems;
	for (int i = 0; i < Player.BackpackContents.size(); i++)
	{
		Item* item = GetItem(Player.BackpackContents[i].ItemId);
		if (item != nullptr && item->IsActivatable)
			activatableItems.push_back(i);
	}

	// activatable items
	int backpackSlot = 0;
	for (int i = 0; i < 8; i++)
	{
		buttonX += ButtonSize + 4;

		int item = -1;
		if (i < activatableItems.size())
		{
			item = Player.BackpackContents[activatableItems[i]].ItemId;
		}

		DrawButton(buttonX, buttonY, item);
	}

	buttonX += ButtonSize + 4;
	DrawSprite(BagSprite, buttonX + ButtonSize/2, buttonY + ButtonSize/2, 0, 1.5f);
}

void GameHudScreen::DrawButton(float x, float y, int itemId, Color border, Color center )
{
	Item* item = GetItem(itemId);
	DrawRectangleRec(Rectangle{ x, y, ButtonSize, ButtonSize }, border);
	DrawRectangleRec(Rectangle{ x + ButtonInset, y + ButtonInset, ButtonSize - ButtonInset * 2, ButtonSize - ButtonInset * 2 }, center);

	if (item)
		DrawSprite(item->Sprite, x + ButtonSize / 2, y + ButtonSize / 2, 0, 2);
}
