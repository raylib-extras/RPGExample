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

#include "audio.h"

#include "raylib.h"

#include <vector>

Music BGM = { 0 };
std::vector<Sound> Sounds;

void InitAudio()
{
	InitAudioDevice();
	SetMasterVolume(0.25f);
	StartBGM("sounds/Flowing Rocks.ogg");
}

void ShutdownAudio()
{
	CloseAudioDevice();
	for (const auto& sound : Sounds)
		UnloadSound(sound);

	Sounds.clear();
	StopBGM();
}

void UpdateAudio()
{
	if (BGM.frameCount > 0)
		UpdateMusicStream(BGM);
}

void StartBGM(const char* musicFIle) 
{
	StopBGM();
	BGM = LoadMusicStream(musicFIle);
	BGM.looping = true;
	PlayMusicStream(BGM);
}

void StopBGM()
{
	if (BGM.frameCount > 0)
	{
		StopMusicStream(BGM);
		UnloadMusicStream(BGM);
		BGM.frameCount = 0;
	}
}

int LoadSoundFile(const char* sound)
{
	Sounds.push_back(LoadSound(sound));
	return int(Sounds.size() - 1);
}

void PlaySound(int sound)
{
	if (sound < 0 || sound > Sounds.size())
		return;

	PlaySound(Sounds[sound]);
}