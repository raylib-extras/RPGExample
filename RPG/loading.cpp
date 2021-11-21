#include "loading.h"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <string>
#include "screens.h"

std::vector<std::string> ResourcesToLoad;

std::vector<Texture> LoadedTextures;

class LoadingScreen : public Screen
{
public:
	std::string LoadingText = "Loading...";

	LoadingScreen()
	{
		float size = MeasureText(LoadingText.c_str(), 20);
		Origin.x = GetScreenWidth() * 0.5f - size * 0.5f;
		Origin.y = GetScreenHeight() * 0.5 - 10;

		LeftSpinner.x = Origin.x - 25;
		RightSpinner.x = Origin.x + size + 25;
		LeftSpinner.y = RightSpinner.y = GetScreenHeight() * 0.5;

		LeftSpinner.width = RightSpinner.width = 20;
		LeftSpinner.height = RightSpinner.height = 20;

	}

	void Update() override {}

	void Draw() override
	{
		DrawText(LoadingText.c_str(), Origin.x, Origin.y, 20, WHITE);

		DrawRectanglePro(LeftSpinner, Vector2{ 10, 10 }, float(GetTime()) * 180.0f, BLUE);
		DrawRectanglePro(RightSpinner, Vector2{ 10, 10 }, float(GetTime()) * -180.0f, BLUE);
	}

	Vector2 Origin = { 0,0 };

	Rectangle LeftSpinner = { 0,0 };
	Rectangle RightSpinner = { 0,0 };
};

LoadingScreen* LoadScreen = nullptr;

void InitResources()
{
	LoadScreen = new LoadingScreen();

	SetActiveScreen(LoadScreen);
}

void CleanupResources()
{
	if (LoadScreen != nullptr)
		delete(LoadScreen);
}

void UpdateLoad()
{

}