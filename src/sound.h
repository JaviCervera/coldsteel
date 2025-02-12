#pragma once

#include "common.h"

struct Channel;
struct Sound;

Sound *LoadSound(const char *filename);
void FreeSound(Sound *sound);
Channel *PlaySound(Sound *sound, bool_t loop);
