#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include "audio.h"
#include "../math.h"
#include "../memblock.h"

#define MIN_DISTANCE 0.0001f

struct Audio_SoLoud : public Audio
{
  Audio_SoLoud()
      : soloud(), music(NULL), musicHandle(0) {}

  void Init()
  {
    soloud.init(SoLoud::Soloud::LEFT_HANDED_3D);
  }

  void Finish()
  {
    soloud.deinit();
  }

  void StopChannel(Channel channel)
  {
    soloud.stop(channel);
  }

  void PauseChannel(Channel channel)
  {
    soloud.setPause(channel, true);
  }

  void ResumeChannel(Channel channel)
  {
    soloud.setPause(channel, false);
  }

  void SetChannelPosition(Channel channel, float x, float y, float z)
  {
    soloud.set3dSourcePosition(channel, x, y, z);
  }

  void SetChannelVelocity(Channel channel, float x, float y, float z)
  {
    soloud.set3dSourceVelocity(channel, x, y, z);
  }

  void SetChannelRadius(Channel channel, float radius)
  {
    soloud.set3dSourceMinMaxDistance(channel, MIN_DISTANCE, radius);
  }

  void SetChannelPitch(Channel channel, float pitch)
  {
    soloud.setRelativePlaySpeed(channel, pitch);
  }

  void SetChannelVolume(Channel channel, float volume)
  {
    soloud.setVolume(channel, volume);
  }

  void SetChannelPan(Channel channel, float pan)
  {
    soloud.setPan(channel, pan);
  }

  bool ChannelPlaying(Channel channel)
  {
    return soloud.isValidVoiceHandle(channel);
  }

  void SetListenerPosition(float x, float y, float z)
  {
    soloud.set3dListenerPosition(x, y, z);
  }

  void SetListenerVelocity(float x, float y, float z)
  {
    soloud.set3dListenerVelocity(x, y, z);
  }

  void SetListenerYaw(float yaw)
  {
    const float atX = Sin(yaw);
    const float atY = 0;
    const float atZ = Cos(yaw);
    soloud.set3dListenerAt(atX, atY, atZ);
    soloud.set3dListenerUp(0, 1, 0);
  }

  bool PlayMusic(const char *filename, bool loop)
  {
    Memblock *data = LoadMemblock(filename);
    if (!data)
      return FALSE;
    SoLoud::WavStream *newMusic = new SoLoud::WavStream();
    SoLoud::handle newHandle = 0;
    SoLoud::result result = newMusic->loadMem((const unsigned char *)data, MemblockSize(data), true, true);
    FreeMemblock(data);
    if (result == SoLoud::SO_NO_ERROR)
    {
      newMusic->setLooping(true);
      newHandle = soloud.play(*newMusic);
    }
    else
    {
      delete newMusic;
    }
    if (newHandle)
    {
      StopMusic();
      music = newMusic;
      musicHandle = newHandle;
    }
    return result == SoLoud::SO_NO_ERROR && newHandle != 0;
  }

  void StopMusic()
  {
    if (MusicPlaying())
      soloud.stop(musicHandle);
    if (music)
      delete music;
    music = NULL;
    musicHandle = 0;
  }

  void PauseMusic()
  {
    soloud.setPause(musicHandle, true);
  }

  void ResumeMusic()
  {
    soloud.setPause(musicHandle, false);
  }

  void SetMusicVolume(float volume)
  {
    soloud.setVolume(musicHandle, volume);
  }

  bool MusicPlaying()
  {
    return soloud.isValidVoiceHandle(musicHandle);
  }

  Sound *LoadSound(const char *filename)
  {
    Memblock *data = LoadMemblock(filename);
    SoLoud::Wav *sound = new SoLoud::Wav();
    SoLoud::result result = sound->loadMem((const unsigned char *)data, MemblockSize(data), true, true);
    FreeMemblock(data);
    if (result != SoLoud::SO_NO_ERROR)
    {
      delete sound;
      sound = NULL;
    }
    return (Sound *)sound;
  }

  void FreeSound(Sound *sound)
  {
    if (sound)
    {
      soloud.stopAudioSource(*(SoLoud::AudioSource *)sound);
      delete (SoLoud::Wav *)sound;
    }
  }

  Channel PlaySound(Sound *sound, bool loop)
  {
    ((SoLoud::AudioSource *)sound)->setLooping(loop);
    return soloud.play(*(SoLoud::AudioSource *)sound);
  }

  Channel PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool loop)
  {
    ((SoLoud::AudioSource *)sound)->setLooping(loop);
    SoLoud::handle channel = soloud.play3d(*(SoLoud::AudioSource *)sound, x, y, z);
    soloud.set3dSourceMinMaxDistance(channel, MIN_DISTANCE, radius);
    return channel;
  }

  void Update()
  {
    soloud.update3dAudio();
  }

private:
  SoLoud::Soloud soloud;
  SoLoud::WavStream *music;
  SoLoud::handle musicHandle;
};

Audio &Audio::Get()
{
  static Audio_SoLoud driver;
  return driver;
}
