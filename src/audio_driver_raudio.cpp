#include "../lib/raudio/raudio.h"
#include "audio_driver.h"
#include "memblock.h"
#include "string.h"

array<Channel *> channels;

struct Channel
{
  Sound sound;
  bool_t looping;

  Channel(Sound sound, bool_t looping)
      : sound(sound), looping(looping)
  {
    channels.push_back(this);
  }

  ~Channel()
  {
    UnloadSound(sound);
    channels.erase(channels.binary_search(this));
  }
};

struct AudioDriver_Null : public AudioDriver
{
  AudioDriver_Null() : music() {}

  void Init()
  {
    InitAudioDevice();
  }

  void Finish()
  {
    CloseAudioDevice();
  }

  void StopChannel(Channel *channel)
  {
    if (channel != NULL)
    {
      delete channel;
    }
  }

  void PauseChannel(Channel *channel)
  {
    if (channel != NULL)
    {
      PauseSound(channel->sound);
    }
  }

  void ResumeChannel(Channel *channel)
  {
    if (channel != NULL)
    {
      ResumeSound(channel->sound);
    }
  }

  void SetChannelPosition(Channel *channel, float x, float y, float z, float radius) {}

  float ChannelX(Channel *channel) { return 0.0f; }

  float ChannelY(Channel *channel) { return 0.0f; }

  float ChannelZ(Channel *channel) { return 0.0f; }

  float ChannelRadius(Channel *channel) { return 0.0f; }

  void SetChannelPitch(Channel *channel, float pitch)
  {
    if (channel != NULL)
    {
      SetSoundPitch(channel->sound, pitch);
    }
  }

  void SetChannelVolume(Channel *channel, float volume)
  {
    if (channel != NULL)
    {
      SetSoundVolume(channel->sound, volume);
    }
  }

  void SetChannelPan(Channel *channel, float pan)
  {
    if (channel != NULL)
    {
      SetSoundPan(channel->sound, pan);
    }
  }

  bool_t ChannelPlaying(Channel *channel)
  {
    return channel != NULL && IsSoundPlaying(channel->sound);
  }

  void SetListener(float x, float y, float z, float yaw) {}

  float ListenerX() { return 0.0f; }

  float ListenerY() { return 0.0f; }

  float ListenerZ() { return 0.0f; }

  float ListenerYaw() { return 0.0f; }

  bool_t PlayMusic(const char *filename, bool_t loop)
  {
    Memblock *data = LoadMemblock(filename);
    if (data == NULL)
      return FALSE;
    if (IsMusicReady(music))
      UnloadMusicStream(music);
    music = LoadMusicStreamFromMemory(
        (stringc(".") + ExtractExt(filename)).c_str(),
        (const unsigned char *)data,
        MemblockSize(data));
    FreeMemblock(data);
    if (!loop)
      music.looping = FALSE;
    if (IsMusicReady(music))
      PlayMusicStream(music);
    return IsMusicStreamPlaying(music);
  }

  void StopMusic()
  {
    StopMusicStream(music);
    UnloadMusicStream(music);
  }

  void PauseMusic()
  {
    PauseMusicStream(music);
  }

  void ResumeMusic()
  {
    ResumeMusicStream(music);
  }

  void SetMusicVolume(float volume)
  {
    ::SetMusicVolume(music, volume);
  }

  bool_t MusicPlaying()
  {
    return IsMusicStreamPlaying(music);
  }

  Sound *LoadSound(const char *filename)
  {
    Memblock *data = LoadMemblock(filename);
    if (data == NULL)
      return NULL;
    Wave wave = LoadWaveFromMemory(
        (stringc(".") + ExtractExt(filename)).c_str(),
        (const unsigned char *)data,
        MemblockSize(data));
    FreeMemblock(data);
    Sound *sound = new Sound(LoadSoundFromWave(wave));
    UnloadWave(wave);
    if (!IsSoundReady(*sound))
    {
      FreeSound(sound);
      return NULL;
    }
    return sound;
  }

  void FreeSound(Sound *sound)
  {
    if (sound != NULL)
    {
      UnloadSound(*sound);
      delete sound;
    }
  }

  Channel *PlaySound(Sound *sound, bool_t loop)
  {
    if (sound == NULL)
      return NULL;
    Channel *channel = new Channel(LoadSoundAlias(*sound), loop);
    ::PlaySound(channel->sound);
    return channel;
  }

  void Update()
  {
    UpdateMusicStream(music);
    array<Channel *> channels_to_delete;
    for (u32 i = 0; i < channels.size(); ++i)
    {
      Channel *channel = channels[i];
      if (!IsSoundPlaying(channel->sound))
      {
        if (channel->looping)
          ::PlaySound(channel->sound);
        else
          channels_to_delete.push_back(channel);
      }
    }
    for (u32 i = 0; i < channels_to_delete.size(); ++i)
    {
      delete channels_to_delete[i];
    }
  }

private:
  Music music;
};

AudioDriver &AudioDriver::Get()
{
  static AudioDriver_Null driver;
  return driver;
}
