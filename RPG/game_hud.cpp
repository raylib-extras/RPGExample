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

#include "game_hud.h"
#include "items.h"
#include "resource_ids.h"

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
	if (DrawButton(inventoryWindowRect.x + 20, inventoryWindowRect.y + 20, weaponItem != nullptr ? weaponItem->Sprite : -1, 0, DARKGRAY, GRAY))
	{
		HoveredItem = weaponItem;
	}
	DrawText("Weapon", int(inventoryWindowRect.x + 20 + ButtonSize + 2), int(inventoryWindowRect.y + 20), 20, DARKBROWN);
	DrawText(TextFormat("%d - %d", Player.GetAttack().MinDamage, Player.GetAttack().MaxDamage), int(inventoryWindowRect.x + 20 + ButtonSize + 2), int(inventoryWindowRect.y + 40), 20, WHITE);
	
	Item* armorItem = GetItem(Player.EquipedArmor);
	if (DrawButton(inventoryWindowRect.x + inventoryWindowRect.width - (20 + ButtonSize), inventoryWindowRect.y + 20, armorItem != nullptr ? armorItem->Sprite : -1, 0, DARKBROWN, BROWN))
	{
		HoveredItem = armorItem;
	}
	DrawText("Armor", int(inventoryWindowRect.x + inventoryWindowRect.width - (20 + ButtonSize + 62)), int(inventoryWindowRect.y + ButtonSize), 20, DARKBROWN);
	DrawText(TextFormat("%d", Player.GetDefense()), int(inventoryWindowRect.x + inventoryWindowRect.width - (20 + ButtonSize + 22)), int(inventoryWindowRect.y + ButtonSize - 20), 20, WHITE);

	// backpack contents
	constexpr int inventoryItemSize = 64;
	constexpr int inventoryItemPadding = 4;

	DrawText("Backpack (LMB)Use/Equip (RMB)Drop", int(inventoryWindowRect.x + 10), int(inventoryWindowRect.y + 100), 10, DARKBROWN);

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

					if (Player.BackpackContents[itemIndex].Quantity > 1)
						DrawText(TextFormat("%d", Player.BackpackContents[itemIndex].Quantity), int(itemRect.x) + 2, int(itemRect.y + itemRect.height - 10), 10, WHITE);

					bool hovered = CheckCollisionPointRec(GetMousePosition(), itemRect);

					if (hovered)
					{
						HoveredItem = item;
						if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
						{
							if (item->IsActivatable() && Player.ActivateItemCallback != nullptr)
								Player.ActivateItemCallback(itemIndex);
							else if (item->IsWeapon() && Player.EquipWeaponCallback != nullptr)
								Player.EquipWeaponCallback(itemIndex);
							else if (item->IsArmor() && Player.EquipArmorCallback != nullptr)
								Player.EquipArmorCallback(itemIndex);
						}
						else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && Player.DropItemCallback != nullptr)
						{
							Player.DropItemCallback(itemIndex);
						}
					}
				}
			}
			itemIndex++;
		}
	}
}

bool GameHudScreen::IsUiClick(const Vector2& pos)
{
	if (pos.y > GetScreenHeight() - 80.0f)
		return true;

	Rectangle inventoryWindowRect = { GetScreenWidth() - 475.0f ,GetScreenHeight() - 500.0f, 354, 400.0f };

	if (InventoryOpen && CheckCollisionPointRec(pos, inventoryWindowRect))
		return true;

	return false;
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
	DrawButton(buttonX, buttonY, weapon != nullptr ? weapon->Sprite : -1, 0, DARKGRAY, GRAY);

	if (Player.AttackCooldown > 0)
	{
		float height = ButtonSize * Player.AttackCooldown;
		DrawRectangleRec(Rectangle{ buttonX,buttonY + (ButtonSize - height),ButtonSize,height }, ColorAlpha(RED, 0.5f));
	}

	std::vector<int> activatableItems;
	for (int i = 0; i < Player.BackpackContents.size(); i++)
	{
		Item* item = GetItem(Player.BackpackContents[i].ItemId);
		if (item != nullptr && item->IsActivatable())
			activatableItems.push_back(i);
	}

	int activatedItem = -1;

	// activatable items
	int backpackSlot = 0;
	for (int i = 0; i < 7; i++)
	{
		buttonX += ButtonSize + 4;

		if (i < activatableItems.size())
		{
			bool shortcutPressed = IsKeyPressed(KEY_ONE + i);

			Item* item = GetItem(Player.BackpackContents[activatableItems[i]].ItemId);
			if ((DrawButton(buttonX, buttonY, item->Sprite, Player.BackpackContents[activatableItems[i]].Quantity) || shortcutPressed) && item != nullptr)
			{
				if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || shortcutPressed) && Player.ItemCooldown == 0)
				{
					activatedItem = activatableItems[i];
				}
				else
				{
					HoveredItem = item;
				}
			}

			DrawText(TextFormat("%d", i + 1), int(buttonX), int(buttonY), 20, WHITE);

			if (Player.ItemCooldown > 0)
			{
				float height = ButtonSize * Player.ItemCooldown;
				DrawRectangleRec(Rectangle{ buttonX,buttonY + (ButtonSize - height),ButtonSize,height }, ColorAlpha(BLACK, 0.5f));
			}
		}
	}

	if (activatedItem != -1 && Player.ActivateItemCallback != nullptr)
		Player.ActivateItemCallback(activatedItem);

	// backpack buttons
	buttonX += ButtonSize + 4;
	if ((DrawButton(buttonX, buttonY, BagSprite, 0, GRAY, LIGHTGRAY) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_I))
	{
		InventoryOpen = !InventoryOpen;
	}

	buttonX += ButtonSize + 4;

	// buff icon
	if (Player.BuffLifetimeLeft > 0)
	{
		DrawSprite(Player.BuffItem, buttonX + ButtonSize/2, buttonY + ButtonSize/2, 0, 2);
		DrawText(TextFormat("%0.0f", Player.BuffLifetimeLeft), int(buttonX), int(buttonY + ButtonSize-30), 30, RED);
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

bool GameHudScreen::DrawButton(float x, float y, int sprite, int quantity, Color border, Color center )
{
	Rectangle buttonRect = { x, y, ButtonSize, ButtonSize };
	DrawRectangleRec(buttonRect, border);
	DrawRectangleRec(Rectangle{ x + ButtonInset, y + ButtonInset, ButtonSize - ButtonInset * 2, ButtonSize - ButtonInset * 2 }, center);

	if (sprite != -1)
	{
		Vector2 center = { x + ButtonSize / 2, y + ButtonSize / 2 };
		DrawSprite(sprite, center.x+2, center.y+2, 0, 2, BLACK);
		DrawSprite(sprite, center.x, center.y, 0, 2);
	}

	if (quantity > 1)
	{
		DrawText(TextFormat("X%d", quantity), int(x + ButtonSize/2), int(y + ButtonSize - 22), 20, WHITE);
	}

	return CheckCollisionPointRec(GetMousePosition(), buttonRect);
}
