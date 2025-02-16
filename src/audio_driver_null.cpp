#ifdef NO_AUDIO

#include "audio_driver.h"

struct AudioDriver_Null : public AudioDriver
{
  void Init() {}
  void Finish() {}

  void StopChannel(Channel *channel) {}
  void PauseChannel(Channel *channel) {}
  void ResumeChannel(Channel *channel) {}
  void SetChannelPosition(Channel *channel, float x, float y, float z, float radius) {}
  float ChannelX(Channel *channel) { return 0.0f; }
  float ChannelY(Channel *channel) { return 0.0f; }
  float ChannelZ(Channel *channel) { return 0.0f; }
  float ChannelRadius(Channel *channel) { return 0.0f; }
  void SetChannelPitch(Channel *channel, float pitch) {}
  void SetChannelVolume(Channel *channel, float volume) {}
  void SetChannelPan(Channel *channel, float pan) {}
  bool_t ChannelPlaying(Channel *channel) { return FALSE; }

  void SetListener(float x, float y, float z, float yaw) {}
  float ListenerX() { return 0.0f; }
  float ListenerY() { return 0.0f; }
  float ListenerZ() { return 0.0f; }
  float ListenerYaw() { return 0.0f; }

  bool_t PlayMusic(const char *filename, bool_t loop) { return FALSE; }
  void StopMusic() {}
  void PauseMusic() {}
  void ResumeMusic() {}
  void SetMusicVolume(float volume) {}
  bool_t MusicPlaying() { return FALSE; }

  Sound *LoadSound(const char *filename) { return NULL; }
  void FreeSound(Sound *sound) {}
  Channel *PlaySound(Sound *sound, bool_t loop) { return NULL; }

  void Update() {}
};

AudioDriver &AudioDriver::Get()
{
  static AudioDriver_Null driver;
  return driver;
}

#endif // NO_AUDIO
