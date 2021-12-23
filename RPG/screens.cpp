#include "screens.h"

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

	return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Screen::DimSceen(float alpha)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, alpha));
}