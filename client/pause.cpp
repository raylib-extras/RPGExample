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

#include "pause.h"

#include "screens.h"
#include "main.h"

class PauseMenuScreen : public Screen
{
public:
	void Draw() override
	{
		// dim the background
		DimSceen();

		DrawCenteredText(40, "Raylib RPG Example", 40, BLUE);
		DrawCenteredText(105, "Paused", 60, RED);

		DrawText(VersionString, 2, GetScreenHeight() - 10, 10, GRAY);
		DrawText(CopyrightString, GetScreenWidth() - 2 - MeasureText(CopyrightString, 10), GetScreenHeight() - 10, 10, GRAY);

		if (CenteredButton(GetScreenHeight() / 4, "Resume"))
			ResumeGame();

		if (CenteredButton(GetScreenHeight() / 2, "Quit to Menu"))
			GoToMainMenu();

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