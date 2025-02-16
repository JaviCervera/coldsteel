#ifndef NO_AUDIO

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "audio_driver.h"
#define STB_VORBIS_HEADER_ONLY
#include "../lib/stb/stb_vorbis.c"

#define BUFFER_SIZE 4096 * 8

struct AudioStream
{
  ALuint source;
  stb_vorbis *stream;
  stb_vorbis_info info;
  ALuint buffers[2];
  size_t samplesLeft;
  bool_t loop;

  AudioStream(ALuint source, stb_vorbis *stream, bool_t loop) : stream(stream), samplesLeft(0), loop(loop)
  {
    memset(&info, 0, sizeof(stb_vorbis_info));
    buffers[0] = 0;
    buffers[1] = 0;
  }
};

struct Channel
{
  ALuint id;
  vector3df position;
  float radius;
  Channel(ALuint id, float radius) : id(id), position(0, 0, 0), radius(radius) {}
};

struct Sound
{
  ALuint id;
  Sound(ALuint id) : id(id) {}
};

struct AudioDriver_OpenAL : public AudioDriver
{
  AudioDriver_OpenAL()
      : alDevice(NULL),
        alContext(NULL),
        listenerPosition(0, 0, 0),
        listenerYaw(0),
        music(NULL) {}

  void Init()
  {
    if (!alDevice)
      alDevice = alcOpenDevice(NULL);
    if (!alContext)
      alContext = alcCreateContext(alDevice, NULL);
    if (alContext)
      alcMakeContextCurrent(alContext);
  }

  void Finish()
  {
    if (alContext)
      alcDestroyContext(alContext);
    if (alDevice)
      alcCloseDevice(alDevice);
    alDevice = NULL;
    alContext = NULL;
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

  void SetListener(float x, float y, float z, float yaw)
  {
    alListener3f(AL_POSITION, x, y, z);
    alListener3f(AL_ORIENTATION, 0, yaw, 0);
    // TODO: Set alVelocity based on previous position
    listenerPosition = vector3df(x, y, z);
    listenerYaw = yaw;
  }

  float ListenerX()
  {
    return listenerPosition.X;
  }

  float ListenerY()
  {
    return listenerPosition.Y;
  }

  float ListenerZ()
  {
    return listenerPosition.Z;
  }

  float ListenerYaw()
  {
    return listenerYaw;
  }

  bool_t PlayMusic(const char *filename, bool_t loop)
  {
    StopMusic();

    stb_vorbis *oggStream = stb_vorbis_open_filename(filename, NULL, NULL);
    if (!oggStream)
      return false;

    ALuint source;
    alGenSources(1, &source);

    music = new AudioStream(source, oggStream, loop);
    music->info = stb_vorbis_get_info(oggStream);
    music->samplesLeft = stb_vorbis_stream_length_in_samples(oggStream) * music->info.channels;

    alGenBuffers(2, music->buffers);
    if (!UpdateStream(music, 0))
    {
      StopMusic();
      return false;
    }
    if (!UpdateStream(music, 1))
    {
      StopMusic();
      return false;
    }
    alSourceQueueBuffers(music->source, 2, music->buffers);

    return true;
  }

  void StopMusic()
  {
    if (music)
    {
      alDeleteSources(1, &music->source);
      alDeleteBuffers(2, music->buffers);
      stb_vorbis_close(music->stream);
      delete music;
    }
    music = NULL;
  }

  void PauseMusic()
  {
    if (music)
      alSourcePause(music->source);
  }

  void ResumeMusic()
  {
    if (music)
      alSourcePlay(music->source);
  }

  void SetMusicVolume(float volume)
  {
    if (music)
      alSourcef(music->source, AL_GAIN, volume);
  }

  bool_t MusicPlaying()
  {
    if (music)
    {
      int state;
      alGetSourcei(music->source, AL_SOURCE_STATE, &state);
      return state == AL_PLAYING;
    }
    else
    {
      return false;
    }
  }

  Sound *LoadSound(const char *filename)
  {
    stb_vorbis *file = stb_vorbis_open_filename((char *)filename, NULL, NULL);
    if (!file)
      return NULL;

    const stb_vorbis_info fileinfo = stb_vorbis_get_info(file);
    const int length_samples = stb_vorbis_stream_length_in_samples(file) * fileinfo.channels;
    short *buffer = new short[length_samples];
    stb_vorbis_get_samples_short_interleaved(file, fileinfo.channels, buffer, length_samples);

    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);
    alBufferData(alBuffer, GetFormatFromInfo(fileinfo.channels, 0), buffer, length_samples * sizeof(short), fileinfo.sample_rate);
    stb_vorbis_close(file);
    delete[] buffer;

    return new Sound(alBuffer);
  }

  void FreeSound(Sound *sound)
  {
    if (sound)
    {
      alDeleteBuffers(1, &sound->id);
      delete sound;
    }
  }

  Channel *PlaySound(Sound *sound, bool_t loop)
  {
    return PlayChannel(sound, loop);
  }

  void Update()
  {
    if (music)
    {
      ALint processed = 0;
      alGetSourcei(music->source, AL_BUFFERS_PROCESSED, &processed);
      while (processed--)
      {
        ALuint buffer;
        alSourceUnqueueBuffers(music->source, 1, &buffer);
        bool streamOk = UpdateStream(music, buffer);
        if (streamOk)
        {
          alSourceQueueBuffers(music->source, 1, &buffer);
        }
        else if (music->loop)
        {
          stb_vorbis_seek_start(music->stream);
          music->samplesLeft = stb_vorbis_stream_length_in_samples(music->stream) * music->info.channels;
          if (UpdateStream(music, buffer))
            alSourceQueueBuffers(music->source, 1, &buffer);
        }
      }
    }
  }

private:
  ALCdevice *alDevice;
  ALCcontext *alContext;
  vector3df listenerPosition;
  float listenerYaw;
  AudioStream *music;

  Channel *PlayChannel(Sound *sound, bool_t loop)
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

  static bool_t UpdateStream(AudioStream *stream, size_t bufferIndex)
  {
    ALshort pcm[BUFFER_SIZE];
    int size = stb_vorbis_get_samples_short_interleaved(stream->stream, stream->info.channels, pcm, BUFFER_SIZE);
    if (size == 0)
      return false;
    alBufferData(
        stream->buffers[bufferIndex],
        stream->info.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
        pcm,
        size * stream->info.channels * sizeof(ALshort), stream->info.sample_rate);
    stream->samplesLeft -= size;
    return true;
  }

  static ALenum GetFormatFromInfo(short channels, short bitsPerSample)
  {
    if (channels == 1)
      return AL_FORMAT_MONO16;
    return AL_FORMAT_STEREO16;
  }
};

AudioDriver &AudioDriver::Get()
{
  static AudioDriver_OpenAL driver;
  return driver;
}

#endif // NO_AUDIO
