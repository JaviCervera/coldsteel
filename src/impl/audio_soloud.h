#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include "../include/audio.h"
#include "../math.h"
#include "../memblock.h"

#define MIN_DISTANCE 0.0001f

struct Position
{
  vector3df pos;
  vector3df vel;

  Position(const vector3df &pos = vector3df())
      : pos(pos), vel() {}

  Position &operator=(const Position &other)
  {
    vel += other.pos - pos;
    pos = other.pos;
    return *this;
  }
};

struct Audio_SoLoud : public Audio
{
  Audio_SoLoud()
      : soloud(), music(NULL), musicHandle(0)
  {
    soloud.init(SoLoud::Soloud::LEFT_HANDED_3D);
  }

  ~Audio_SoLoud()
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

  void SetChannel3DPosition(Channel channel, float x, float y, float z)
  {
    positions[channel] = Position(vector3df(x, y, z));
    const Position pos = positions[channel];
    soloud.set3dSourcePosition(channel, x, y, z);
    soloud.set3dSourceVelocity(channel, pos.vel.X, pos.vel.Y, pos.vel.Z);
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

  void SetListener(float x, float y, float z, float yaw)
  {
    const float atX = Sin(yaw);
    const float atY = 0;
    const float atZ = Cos(yaw);
    listener = Position(vector3df(x, y, z));
    soloud.set3dListenerParameters(x, y, z, atX, atY, atZ, 0, 1, 0, listener.vel.X, listener.vel.Y, listener.vel.Z);
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
    SoLoud::handle channel = soloud.play(*(SoLoud::AudioSource *)sound);
    positions.remove(channel);
    return channel;
  }

  Channel PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool loop)
  {
    ((SoLoud::AudioSource *)sound)->setLooping(loop);
    SoLoud::handle channel = soloud.play3d(*(SoLoud::AudioSource *)sound, x, y, z);
    positions.remove(channel);
    positions[channel] = Position(vector3df(x, y, z));
    soloud.set3dSourceMinMaxDistance(channel, MIN_DISTANCE, radius);
    return channel;
  }

  void Update()
  {
    soloud.update3dAudio();
    listener.vel = vector3df();
    for (map<SoLoud::handle, Position>::Iterator it = positions.getIterator(); !it.atEnd(); it++)
    {
      it->getValue() = vector3df();
    }
  }

private:
  SoLoud::Soloud soloud;
  SoLoud::WavStream *music;
  SoLoud::handle musicHandle;
  Position listener;
  map<SoLoud::handle, Position> positions; // Channel positions
};
