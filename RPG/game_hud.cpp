#include "game_hud.h"
#include "items.h"

#include "raylib.h"

GameHudScreen::GameHudScreen(PlayerData& player) 
	: Screen()
	, Player(player)
{

}

void GameHudScreen::ShowItemToolTip(const Item* item, const Rectangle& rect)
{
	if (item == nullptr || !CheckCollisionPointRec(GetMousePosition(), rect))
		return;

	DrawRectangleRec(Rectangle{ rect.x,rect.y,100,100 }, ColorAlpha(BLACK, 0.75f));
	DrawText(item->Name.c_str(), int(rect.x), int(rect.y), 10, WHITE);
}

void GameHudScreen::DrawInventory()
{
	Rectangle inventoryWindowRect = { GetScreenWidth() - 475.0f ,GetScreenHeight() - 500.0f, 354, 400.0f };
	Rectangle shadowRect = inventoryWindowRect;
	shadowRect.x += 10;
	shadowRect.y += 10;
	DrawRectangleRec(shadowRect, ColorAlpha(DARKBROWN, 0.5f));
	FillRectWithSprite(InventoryBackgroundSprite, inventoryWindowRect);

	// equipment
	Item* weaponItem = GetItem(Player.EquipedWeapon);
	if (DrawButton(inventoryWindowRect.x + 20, inventoryWindowRect.y + 20, weaponItem != nullptr ? weaponItem->Sprite : -1, DARKGRAY, GRAY))
	{
		HoveredItem = weaponItem;
	}
	DrawText("Weapon", int(inventoryWindowRect.x + 20 + ButtonSize + 2), int(inventoryWindowRect.y + 20), 20, DARKBROWN);

	Item* armorItem = GetItem(Player.EquipedArmor);
	if (DrawButton(inventoryWindowRect.x + inventoryWindowRect.width - (20 + ButtonSize), inventoryWindowRect.y + 20, armorItem != nullptr ? armorItem->Sprite : -1, DARKBROWN, BROWN))
	{
		HoveredItem = armorItem;
	}
	DrawText("Armor", int(inventoryWindowRect.x + inventoryWindowRect.width - (20 + ButtonSize + 62)), int(inventoryWindowRect.y + ButtonSize), 20, DARKBROWN);

	// backpack contents
	constexpr int inventoryItemSize = 64;
	constexpr int inventoryItemPadding = 4;

	DrawText("Backpack", int(inventoryWindowRect.x + 10), int(inventoryWindowRect.y + 100), 20, DARKBROWN);


	int itemIndex = 0;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			float itemY = inventoryWindowRect.y + (inventoryWindowRect.height - inventoryItemPadding) - ((inventoryItemPadding + inventoryItemSize) * (4 - y));
			float itemX = inventoryWindowRect.x + (inventoryItemPadding * 2) + ((inventoryItemSize + inventoryItemPadding) * x);

			Rectangle itemRect = { itemX, itemY, inventoryItemSize, inventoryItemSize };
			Rectangle shadowRect = itemRect;
			shadowRect.x += 2;
			shadowRect.y += 2;

			DrawRectangleRec(shadowRect, ColorAlpha(BLACK, 0.5f));
			FillRectWithSprite(ItemBackgroundSprite, itemRect);

			if (itemIndex < Player.BackpackContents.size())
			{
				Item* item = GetItem(Player.BackpackContents[itemIndex].ItemId);
				if (item != nullptr)
				{
					DrawSprite(item->Sprite, itemRect.x + itemRect.width / 2, itemRect.y + itemRect.height / 2);

					if (CheckCollisionPointRec(GetMousePosition(), itemRect))
						HoveredItem = item;
				}
			}
			itemIndex++;
		}
	}
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

	// clear the hover item from last frame
	HoveredItem = nullptr;

	// action buttons
	float buttonX = 20 + healthBarWidth + 10;
	float buttonY = barHieght + 4;

	Item* weapon = GetItem(Player.EquipedWeapon);
	// equipped weapon
	DrawButton(buttonX, buttonY, weapon != nullptr ? weapon->Sprite : -1, DARKGRAY, GRAY);

	std::vector<int> activatableItems;
	for (int i = 0; i < Player.BackpackContents.size(); i++)
	{
		Item* item = GetItem(Player.BackpackContents[i].ItemId);
		if (item != nullptr && item->IsActivatable)
			activatableItems.push_back(i);
	}

	// activatable items
	int backpackSlot = 0;
	for (int i = 0; i < 7; i++)
	{
		buttonX += ButtonSize + 4;

		if (i < activatableItems.size())
		{
			Item* item = GetItem(Player.BackpackContents[activatableItems[i]].ItemId);
			if (DrawButton(buttonX, buttonY, item->Sprite) && item != nullptr)
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					// item clicked
				}
				else
				{
					HoveredItem = item;
				}
			}
		}
	}

	buttonX += ButtonSize + 4;
	if (DrawButton(buttonX, buttonY, BagSprite, GRAY, LIGHTGRAY) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		InventoryOpen = !InventoryOpen;
	}

	if (InventoryOpen)
		DrawInventory();

	if (HoveredItem != nullptr)
	{
		Vector2 size = MeasureTextEx(GetFontDefault(), HoveredItem->Name.c_str(), 20, 2);
		Rectangle toolTipRect = { GetMousePosition().x - (size.x/2 + 2), GetMousePosition().y - (size.y + 2), size.x + 4, size.y + 4 };

		DrawRectangleRec(toolTipRect, ColorAlpha(BLACK, 0.5f));
		DrawText(HoveredItem->Name.c_str(), int(toolTipRect.x) + 2, int(toolTipRect.y) + 2, 20, WHITE);
	}
}

bool GameHudScreen::DrawButton(float x, float y, int sprite, Color border, Color center )
{
	Rectangle buttonRect = { x, y, ButtonSize, ButtonSize };
	DrawRectangleRec(buttonRect, border);
	DrawRectangleRec(Rectangle{ x + ButtonInset, y + ButtonInset, ButtonSize - ButtonInset * 2, ButtonSize - ButtonInset * 2 }, center);

	if (sprite != -1)
	{
		Vector2 center = { x + ButtonSize / 2, y + ButtonSize / 2 };
		DrawSprite(sprite, center.x+2, center.y+2, 0, 2, BLACK);
	;	DrawSprite(sprite, center.x, center.y, 0, 2);
	}

	return CheckCollisionPointRec(GetMousePosition(), buttonRect);
}