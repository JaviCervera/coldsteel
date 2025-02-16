#include "audio_driver.h"
#include "channel.h"

void StopChannel(Channel *channel)
{
  AudioDriver::Get().StopChannel(channel);
}

void PauseChannel(Channel *channel)
{
  AudioDriver::Get().PauseChannel(channel);
}

void ResumeChannel(Channel *channel)
{
  AudioDriver::Get().ResumeChannel(channel);
}

void SetChannelPosition(Channel *channel, float x, float y, float z, float radius)
{
  AudioDriver::Get().SetChannelPosition(channel, x, y, z, radius);
}

float ChannelX(Channel *channel)
{
  return AudioDriver::Get().ChannelX(channel);
}

float ChannelY(Channel *channel)
{
  return AudioDriver::Get().ChannelY(channel);
}

float ChannelZ(Channel *channel)
{
  return AudioDriver::Get().ChannelZ(channel);
}

float ChannelRadius(Channel *channel)
{
  return AudioDriver::Get().ChannelRadius(channel);
}

void SetChannelPitch(Channel *channel, float pitch)
{
  AudioDriver::Get().SetChannelPitch(channel, pitch);
}

void SetChannelVolume(Channel *channel, float volume)
{
  AudioDriver::Get().SetChannelVolume(channel, volume);
}

void SetChannelPan(Channel *channel, float pan)
{
  AudioDriver::Get().SetChannelPan(channel, pan);
}

bool_t ChannelPlaying(Channel *channel)
{
  return AudioDriver::Get().ChannelPlaying(channel);
}
