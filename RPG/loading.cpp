#include "loading.h"
#include "main.h"
#include "resource_ids.h"
#include "screens.h"
#include "sprites.h"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <string>
#include <deque>

class LoadingScreen : public Screen
{
public:
	std::string LoadingText = "Loading...";

	LoadingScreen()
	{
		int size = MeasureText(LoadingText.c_str(), 20);
		Origin.x = GetScreenWidth() * 0.5f - size * 0.5f;
		Origin.y = GetScreenHeight() * 0.5f - 10;

		LeftSpinner.x = Origin.x - 25.0f;
		RightSpinner.x = Origin.x + size + 25.0f;
		LeftSpinner.y = RightSpinner.y = GetScreenHeight() * 0.5f;

		LeftSpinner.width = RightSpinner.width = 20;
		LeftSpinner.height = RightSpinner.height = 20;
	}

	void Draw() override
	{
		// tell them we are loading
		DrawText(LoadingText.c_str(), int(Origin.x), int(Origin.y), 20, WHITE);

		// some spinny things to know that the app hasn't locked up
		DrawRectanglePro(LeftSpinner, Vector2{ 10, 10 }, float(GetTime()) * 180.0f, BLUE);
		DrawRectanglePro(RightSpinner, Vector2{ 10, 10 }, float(GetTime()) * -180.0f, BLUE);

		// progress bar.
		float progressWidth = RightSpinner.x - LeftSpinner.x;
		DrawRectangle(int(LeftSpinner.x), int(LeftSpinner.y + 20), (int)(progressWidth * Progress), 5, RAYWHITE);
	}

	Vector2 Origin = { 0,0 };

	Rectangle LeftSpinner = { 0,0 };
	Rectangle RightSpinner = { 0,0 };

	// Load progress 0 = 0% 1 = 100%
	float Progress = 0;
};

LoadingScreen* LoadScreen = nullptr;

std::deque<std::string> TexturesToLoad;

std::vector<Texture> LoadedTextures;

Texture DefaultTexture = { 0 };

void InitResources()
{
	LoadScreen = new LoadingScreen();
	SetActiveScreen(LoadScreen);

	// setup the assets to load
	TexturesToLoad.emplace_back("resources/colored_tilemap.png"); //TileSetTexture
	TexturesToLoad.emplace_back("resources/icons/Icon.5_46.png"); //LogoTexture


	// setup default texture
	Image checkered = GenImageChecked(32, 32, 8, 8, GRAY, RAYWHITE);
	DefaultTexture = LoadTextureFromImage(checkered);
	UnloadImage(checkered);
}

void CleanupResources()
{
	// destroy the loading screen
	if (LoadScreen != nullptr)
		delete(LoadScreen);

	// unload the textures
	UnloadTexture(DefaultTexture);
	for (const Texture& texture : LoadedTextures)
		UnloadTexture(texture);

	// clear any data we stored
	LoadedTextures.clear();
	DefaultTexture.id = 0;
	LoadScreen = nullptr;
}

void FinalizeLoad()
{
	LoadSpriteFrames(TileSetTexture, 14, 10, 4);
}

void UpdateLoad()
{
	if (TexturesToLoad.empty())
	{
		FinalizeLoad();
		LoadComplete();
		return;
	}

	// load some resources
	// we don't want to load them all in one shot, that may take some time, and the app will look like it is dead
	// so we only load a few per frame.
	const int maxToLoadPerFrame = 1;

	for (int i = 0; i < maxToLoadPerFrame; ++i)
	{
		if (TexturesToLoad.empty())
			break;

		LoadedTextures.push_back(LoadTexture(TexturesToLoad.front().c_str()));
		TexturesToLoad.pop_front();
	}

	// update the progress by computing how many we have left to load out of the total
	LoadScreen->Progress = TexturesToLoad.size() / float(LoadedTextures.size() + TexturesToLoad.size());

	// then get the inverse to get how much we have loaded
	LoadScreen->Progress = 1.0f - LoadScreen->Progress;
}

// gets a texture from an ID. The textures are loaded in ID order.
const Texture& GetTexture(int id)
{
	if (id < 0 || id > int(LoadedTextures.size()))
		return DefaultTexture;

	return LoadedTextures[id];
}