#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#define STB_VORBIS_HEADER_ONLY
#include "../lib/stb/stb_vorbis.c"
#include "channel.h"
#include "listener.h"
#include "math.h"

struct Channel
{
  ALuint id;
  vector3df position;
  float radius;
  Channel(ALuint id, float radius) : id(id), position(0, 0, 0), radius(radius) {}
};

Channel *_PlayChannel(Sound *sound, bool_t loop)
{
  ALuint source;
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, *((ALuint *)sound));
  alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);

  Channel *channel = new Channel(source, 100);
  SetChannelPitch(channel, 1);
  SetChannelVolume(channel, 1);
  SetChannelPosition(channel, channel->position.X, channel->position.Y, channel->position.Z, channel->radius);
  alSourcePlay(source);
  return channel;
}

void StopChannel(Channel *channel)
{
  if (channel)
  {
    alSourceStop(channel->id);
    alDeleteSources(1, &channel->id);
    delete channel;
  }
}

void PauseChannel(Channel *channel)
{
  alSourcePause(channel->id);
}

void ResumeChannel(Channel *channel)
{
  alSourcePlay(channel->id);
}

void SetChannelPosition(Channel *channel, float x, float y, float z, float radius)
{
  alSource3f(channel->id, AL_POSITION, x, y, z);
  alSourcef(channel->id, AL_REFERENCE_DISTANCE, radius);
  // TODO Set velocity
  channel->position = vector3df(x, y, z);
  channel->radius = radius;
}

float ChannelX(Channel *channel)
{
  return channel->position.X;
}

float ChannelY(Channel *channel)
{
  return channel->position.Y;
}

float ChannelZ(Channel *channel)
{
  return channel->position.Z;
}

float ChannelRadius(Channel *channel)
{
  return channel->radius;
}

void SetChannelPitch(Channel *channel, float pitch)
{
  alSourcef(channel->id, AL_PITCH, pitch);
}

void SetChannelVolume(Channel *channel, float volume)
{
  alSourcef(channel->id, AL_GAIN, volume);
}

void SetChannelPan(Channel *channel, float pan)
{
  // TODO: Set radius to 2 and disable attenuation
}

bool_t ChannelPlaying(Channel *channel)
{
  int state;
  alGetSourcei(channel->id, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}
