#pragma once

#include "common.h"

typedef int Channel;
struct Sound;

void StopChannel(Channel channel);
void PauseChannel(Channel channel);
void ResumeChannel(Channel channel);
void SetChannelPosition(Channel channel, float x, float y, float z, float radius);
void SetChannelPitch(Channel channel, float pitch);
void SetChannelVolume(Channel channel, float volume);
void SetChannelPan(Channel channel, float pan);
bool_t ChannelPlaying(Channel channel);
