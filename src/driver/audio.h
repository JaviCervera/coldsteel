#pragma once

typedef int Channel;
struct Sound;

struct Audio
{
  static Audio &Get();
  virtual void Init() = 0;
  virtual void Finish() = 0;

  virtual void StopChannel(Channel channel) = 0;
  virtual void PauseChannel(Channel channel) = 0;
  virtual void ResumeChannel(Channel channel) = 0;
  virtual void SetChannelPosition(Channel channel, float x, float y, float z, float radius) = 0;
  virtual void SetChannelPitch(Channel channel, float pitch) = 0;
  virtual void SetChannelVolume(Channel channel, float volume) = 0;
  virtual void SetChannelPan(Channel channel, float pan) = 0;
  virtual bool ChannelPlaying(Channel channel) = 0;

  virtual void SetListener(float x, float y, float z, float yaw) = 0;

  virtual bool PlayMusic(const char *filename, bool loop) = 0;
  virtual void StopMusic() = 0;
  virtual void PauseMusic() = 0;
  virtual void ResumeMusic() = 0;
  virtual void SetMusicVolume(float volume) = 0;
  virtual bool MusicPlaying() = 0;

  virtual Sound *LoadSound(const char *filename) = 0;
  virtual void FreeSound(Sound *sound) = 0;
  virtual Channel PlaySound(Sound *sound, bool loop) = 0;
  virtual Channel PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool loop) = 0;

  virtual void Update() = 0;
};
