#pragma once

#include "common.h"

typedef int Channel;
struct Sound;

Sound *LoadSound(const char *filename);
void FreeSound(Sound *sound);
Channel PlaySound(Sound *sound, bool_t loop);
Channel PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool_t loop);
