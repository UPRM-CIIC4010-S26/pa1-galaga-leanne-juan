#include "raylib.h"

#include "resource_dir.h"
#include "Program.hpp"

int screen_width = 1000;
int screen_height = 1000;

int main()
{
	SearchAndSetResourceDir("resources");
	InitAudioDevice();
	SetTargetFPS(60);

	raylib::Window window(screen_width, screen_width, "Galaga", FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	Program Galaga;
	ImageManager::Load();
	SoundManager::Load();

	if (!IsWindowReady())
	{
		TraceLog(LOG_ERROR, "Window failed to initialize!");
		return 1;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Galaga.Update();
		Galaga.Draw();
		Galaga.KeyInputs();

		EndDrawing();
	}

	ImageManager::Unload();
	SoundManager::Unload();
	CloseAudioDevice();
	window.Close();
	return 0;
}
