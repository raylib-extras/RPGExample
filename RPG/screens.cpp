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
	int rectHeight = ButtonFontSize + (ButtonBorder * 2);
	int textWidth = MeasureText(text, ButtonFontSize);

	int textXOrigin = GetScreenWidth() / 2 - textWidth / 2;
	int textYOrigin = y - ButtonFontSize / 2;

	Rectangle buttonRect = { textXOrigin - ButtonBorder, textYOrigin - ButtonBorder, textWidth + (ButtonBorder * 2), ButtonFontSize + (ButtonBorder * 2) };

	bool hovered = RectIsHovered(buttonRect);
	bool down = hovered & IsMouseButtonDown(MOUSE_BUTTON_LEFT);

	Color color = hovered ? (down ? ButtonPressColor : ButtonHighlight) : (ButtonColor);

	DrawRectangleRec(buttonRect, ColorAlpha(color, 0.25f));
	DrawText(text, textXOrigin, textYOrigin, ButtonFontSize, color);
	DrawRectangleLines(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, color);

	return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Screen::DimSceen(float alpha)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, alpha));
}