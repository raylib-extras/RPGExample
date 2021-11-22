#pragma once

#include "raylib.h"

class Screen
{
public:
	virtual void Draw() = 0;

protected:
	bool RectIsHovered(Rectangle& rect);
	void DrawCenteredText(int y, const char* text, int fontSize, Color color);
	bool CenteredButton(int y, const char* text);

protected:
	Color ButtonColor = WHITE;
	Color ButtonHighlight = SKYBLUE;
	Color ButtonPressColor = DARKBLUE;

	int ButtonFontSize = 60;
	int ButtonBorder = 10;
};

void SetActiveScreen(Screen* screen);

void DrawScreen();