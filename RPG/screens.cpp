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

#include "screens.h"
#include "audio.h"
#include "resource_ids.h"

Screen* ActiveScreen = nullptr;

void SetActiveScreen(Screen* screen)
{
	ActiveScreen = screen;
}

void DrawScreen()
{
	if (ActiveScreen != nullptr)
		ActiveScreen->Draw();
}

bool Screen::RectIsHovered(Rectangle& rect)
{
	return CheckCollisionPointRec(GetMousePosition(), rect);
}

void Screen::DrawCenteredText(int y, const char* text, int fontSize, Color color)
{
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, GetScreenWidth() / 2 - textWidth / 2, y - fontSize / 2, fontSize, color);
}

bool Screen::CenteredButton(int y, const char* text)
{
	float rectHeight = ButtonFontSize + (ButtonBorder * 2.0f);
	float textWidth = float(MeasureText(text, ButtonFontSize));

	float textXOrigin = GetScreenWidth() / 2.0f - textWidth / 2.0f;
	float textYOrigin = y - ButtonFontSize / 2.0f;

	Rectangle buttonRect = { textXOrigin - ButtonBorder, textYOrigin - ButtonBorder, textWidth + (ButtonBorder * 2.0f), ButtonFontSize + (ButtonBorder * 2.0f) };

	bool hovered = RectIsHovered(buttonRect);
	bool down = hovered & IsMouseButtonDown(MOUSE_BUTTON_LEFT);

	Color color = hovered ? (down ? ButtonPressColor : ButtonHighlight) : (ButtonColor);

	DrawRectangleRec(buttonRect, ColorAlpha(color, 0.25f));
	DrawText(text, int(textXOrigin), int(textYOrigin), ButtonFontSize, color);
	DrawRectangleLinesEx(buttonRect, 2, color);

	bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	if (clicked)
		PlaySound(ClickSoundId);

	return clicked;
}

void Screen::DimSceen(float alpha)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, alpha));
}