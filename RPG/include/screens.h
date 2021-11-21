#pragma once

class Screen
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

void SetActiveScreen(Screen* screen);

void UpdateScreen();
void DrawScreen();