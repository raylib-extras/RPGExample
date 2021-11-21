#include "screens.h"

Screen* ActiveScreen = nullptr;

void SetActiveScreen(Screen* screen)
{
	ActiveScreen = screen;
}

void UpdateScreen()
{
	if (ActiveScreen != nullptr)
		ActiveScreen->Update();
}

void DrawScreen()
{
	if (ActiveScreen != nullptr)
		ActiveScreen->Draw();
}