#include "raylib.h"

#include "loading.h"
#include "game.h"
#include "pause.h"
#include "screens.h"

enum class ApplicationStates
{
	Startup,
	Loading,
	Running,
	Paused
};

ApplicationStates ApplicationState = ApplicationStates::Startup;

void SetupWindow()
{
	SetTargetFPS(144);

	// load an image for the window icon
	Image icon = LoadImage("resources/icons/Icon.6_98.png");

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

void main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280,800,"RPG Example");
	SetupWindow();

	InitResources();

	ApplicationState = ApplicationStates::Loading;

	while (!WindowShouldClose())
	{
		switch (ApplicationState)
		{
		case ApplicationStates::Loading:
			UpdateLoad();
			break;

		case ApplicationStates::Running:
			UpdateGame();
			break;

		case ApplicationStates::Paused: 
			UpdatePaused();
			break;
		}

		UpdateScreen();

		BeginDrawing();
		ClearBackground(BLACK);

		// draw world

		DrawScreen();

		EndDrawing();
	}

	CleanupResources();
	CloseWindow();
}