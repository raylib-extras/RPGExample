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

#include "game.h"
#include "loading.h"
#include "main.h"
#include "map.h"
#include "pause.h"
#include "resource_ids.h"
#include "screens.h"
#include "audio.h"

#include "raylib.h"

// all the states the program can be in 
enum class ApplicationStates
{
	Startup,
	Loading,
	Menu,
	Running,
	Paused,
	GameOver,
	Quitting
};
ApplicationStates ApplicationState = ApplicationStates::Startup;

// the main menu screen
// based on the screen class
class MainMenuScreen : public Screen
{
public:
	void Draw() override
	{
		// dim the background
		DimSceen();

		// title
		DrawCenteredText(40, "Raylib RPG Example", 40, BLUE);

		// version and copyright
		DrawText(VersionString, 2, GetScreenHeight() - 10, 10, GRAY);
		DrawText(CopyrightString, GetScreenWidth() - 2 - MeasureText(CopyrightString, 10), GetScreenHeight() - 10, 10, GRAY);

		// play button
		if (CenteredButton(GetScreenHeight() / 4, "Play"))
			StartGame();
		
		// options button
		CenteredButton(GetScreenHeight()/2, "Options");

		// quit button
		if (CenteredButton(GetScreenHeight() - (GetScreenHeight() / 4), "Quit"))
			QuitApplication();
	}
};
MainMenuScreen MainMenu;

// the game over screen
// shows the win state and final score
class GameOverScreen : public Screen
{
public:
	void Draw() override
	{
		// dim the background
		DimSceen();

		// title
		DrawCenteredText(40, "Raylib RPG Example", 40, BLUE);


		// win state
		if (IsWin)
			DrawCenteredText(120, "Congratulations You WON!", 60, WHITE);
		else
			DrawCenteredText(120, "You died, better luck next time.", 60, RED);

		// score
		DrawCenteredText(200, TextFormat("Score = %d", Gold), 60, YELLOW);

		// version and copyright
		DrawText(VersionString, 2, GetScreenHeight() - 10, 10, GRAY);
		DrawText(CopyrightString, GetScreenWidth() - 2 - MeasureText(CopyrightString, 10), GetScreenHeight() - 10, 10, GRAY);

		// main menu button
		if (CenteredButton(GetScreenHeight() / 2, "Main Menu"))
			GoToMainMenu();

		// quit button
		if (CenteredButton(GetScreenHeight() - (GetScreenHeight() / 4), "Quit"))
			QuitApplication();
	}

	bool IsWin = false;
	int Gold = 0;
};
GameOverScreen GameOver;

// setup the window and icon
void SetupWindow()
{
	// Validate that the window is not taller than the monitor size, if so, set it to a smaller size
	int monitor = GetCurrentMonitor();

	int maxHeight = GetMonitorHeight(monitor) - 40;
	if (GetScreenHeight() > maxHeight)
		SetWindowSize(GetScreenWidth(), maxHeight);

	SetExitKey(0);
	SetTargetFPS(144);

	// load an image for the window icon
	Image icon = LoadImage("icons/Icon.6_98.png");

	// ensure that the picture is in the correct format
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	// replace the background and border colors with transparent
	ImageColorReplace(&icon, BLACK, BLANK);
	ImageColorReplace(&icon, Color{ 136,136,136,255 }, BLANK);

	// set the icon
	SetWindowIcon(icon);

	// free the image data
	UnloadImage(icon);
}

// called by the loading system when all assets are loaded
void LoadComplete()
{
	ApplicationState = ApplicationStates::Menu;
	SetActiveScreen(&MainMenu);

	// load background world so we have something to look at behind the menu
	LoadMap("maps/menu_map.tmx");
}

// called when the game wants to go back to the main menu, from pause or game over screens
void GoToMainMenu()
{
	// quit our game, if our game was running
	if (ApplicationState == ApplicationStates::Running || ApplicationState == ApplicationStates::Paused)
		QuitGame();

	// start our background music again
	StartBGM("sounds/Flowing Rocks.ogg");

	// go back to the main menu like we did when we started up
	LoadComplete();
}

// called by the main menu to check for exit
void UpdateMainMenu()
{
	if (IsKeyPressed(KEY_ESCAPE))
		QuitApplication();
}

// starts a new game
void StartGame()
{
	ApplicationState = ApplicationStates::Running;
	SetActiveScreen(nullptr);
	StopBGM();
	InitGame();
}

// called when the menu wants to pause the game
void PauseGame()
{
	ApplicationState = ApplicationStates::Paused;
}

// called when the menu wants to resume the game
void ResumeGame()
{
	ApplicationState = ApplicationStates::Running;
	SetActiveScreen(nullptr);
	ActivateGame();
}

// called by the game when it is over, by win or loss
void EndGame(bool win, int gold)
{
	ApplicationState = ApplicationStates::GameOver;
	SetActiveScreen(&GameOver);
	GameOver.IsWin = win;
	GameOver.Gold = gold;
}

// quit the entire application
void QuitApplication()
{
	ApplicationState = ApplicationStates::Quitting;
}

bool SearchAndSetResourceDir(const char* folderName)
{
	// check the working dir
	if (DirectoryExists(folderName))
	{
		ChangeDirectory(TextFormat("%s/%s", GetWorkingDirectory(), folderName));
		return true;
	}

	const char* appDir = GetApplicationDirectory();

	// check the applicationDir
	const char* dir = TextFormat("%s%s", appDir, folderName);
	if (DirectoryExists(dir))
	{
		ChangeDirectory(dir);
		return true;
	}

	// check one up from the app dir
	dir = TextFormat("%s../%s", appDir, folderName);
	if (DirectoryExists(dir))
	{
		ChangeDirectory(dir);
		return true;
	}

	// check two up from the app dir
	dir = TextFormat("%s../../%s", appDir, folderName);
	if (DirectoryExists(dir))
	{
		ChangeDirectory(dir);
		return true;
	}

	// check three up from the app dir
	dir = TextFormat("%s../../../%s", appDir, folderName);
	if (DirectoryExists(dir))
	{
		ChangeDirectory(dir);
		return true;
	}

	return false;
}

// the main application loop
int main()
{
	// setup the window
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280,700,"RPG Example");
	SetupWindow();

	SearchAndSetResourceDir("_resources");
	InitAudio();
	InitResources();

	ApplicationState = ApplicationStates::Loading;

	// game loop
	while (!WindowShouldClose() && ApplicationState != ApplicationStates::Quitting)
	{
		// call the update that goes with our current game state
		switch (ApplicationState)
		{
		case ApplicationStates::Loading:
			UpdateLoad();
			break;

		case ApplicationStates::Menu:
			UpdateMainMenu();
			break;

		case ApplicationStates::Running:
			UpdateGame();
			break;

		case ApplicationStates::Paused: 
			UpdatePaused();
			break;
		}

		// update the screen for this frame
		BeginDrawing();
		ClearBackground(BLACK);

		// the map is always first because it is always under the menu
		DrawMap();

		// draw whatever menu or hud screen we have
		DrawScreen();

		UpdateAudio();
		EndDrawing();
	}

	ShutdownAudio();
	CleanupResources();
	CloseWindow();

	return 0;
}
