#pragma once

void InitAudio();
void ShutdownAudio();
void UpdateAudio();

int LoadSoundFile(const char* soundFile);

void StartBGM(const char* musicFile);
void StopBGM();

void PlaySound(int sound);


// sound IDs
constexpr int ClickSoundId = 0;
constexpr int CoinSoundId = 1;
constexpr int ChestOpenSoundId = 2;
constexpr int ItemPickupSoundId = 3;
constexpr int AlertSoundId = 4;