#pragma once

#include "common.h"

bool_t PlayMusic(const char *filename, bool_t loop);
void StopMusic();
void PauseMusic();
void ResumeMusic();
void SetMusicVolume(float volume);
bool_t MusicPlaying();
