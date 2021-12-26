#include "audio.h"

#include "raylib.h"

#include <vector>

Music BGM = { 0 };
std::vector<Sound> Sounds;

void InitAudio()
{
	InitAudioDevice();
	StartBGM("resources/sounds/Flowing Rocks.ogg");
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

	PlaySoundMulti(Sounds[sound]);
}