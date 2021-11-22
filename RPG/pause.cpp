#include "pause.h"

#include "screens.h"
#include "main.h"

class PauseMenuScreen : public Screen
{
public:
	void Draw() override
	{
		DrawCenteredText(40, "Raylib RPG Example", 40, BLUE);
		DrawCenteredText(105, "Paused", 60, RED);

		DrawText(VersionString, 2, GetScreenHeight() - 10, 10, GRAY);
		DrawText(CopyrightString, GetScreenWidth() - 2 - MeasureText(CopyrightString, 10), GetScreenHeight() - 10, 10, GRAY);

		if (CenteredButton(GetScreenHeight() / 4, "Resume"))
			ResumeGame();

		if (CenteredButton(GetScreenHeight() / 2, "Quit to Menu"))
			EndGame();

		if (CenteredButton(GetScreenHeight() - (GetScreenHeight() / 4), "Quit to Desktop"))
			QuitApplication();
	}
};

PauseMenuScreen PauseMenu;

void UpdatePaused()
{
	SetActiveScreen(&PauseMenu);

	if (IsKeyPressed(KEY_ESCAPE))
		ResumeGame();
}