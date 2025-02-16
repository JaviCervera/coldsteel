#pragma once

#include "common.h"

struct Channel;
struct Sound;

void StopChannel(Channel *channel);
void PauseChannel(Channel *channel);
void ResumeChannel(Channel *channel);
void SetChannelPosition(Channel *channel, float x, float y, float z, float radius);
float ChannelX(Channel *channel);
float ChannelY(Channel *channel);
float ChannelZ(Channel *channel);
float ChannelRadius(Channel *channel);
void SetChannelPitch(Channel *channel, float pitch);
void SetChannelVolume(Channel *channel, float volume);
void SetChannelPan(Channel *channel, float pan);
bool_t ChannelPlaying(Channel *channel);
