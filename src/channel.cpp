#include "internal/audio.h"
#include "channel.h"

extern "C"
{

  EXPORT void CALL StopChannel(Channel channel)
  {
    Audio::Get().StopChannel(channel);
  }

  EXPORT void CALL PauseChannel(Channel channel)
  {
    Audio::Get().PauseChannel(channel);
  }

  EXPORT void CALL ResumeChannel(Channel channel)
  {
    Audio::Get().ResumeChannel(channel);
  }

  EXPORT void CALL SetChannel3DPosition(Channel channel, float x, float y, float z)
  {
    Audio::Get().SetChannel3DPosition(channel, x, y, z);
  }

  EXPORT void CALL SetChannelRadius(Channel channel, float radius)
  {
    Audio::Get().SetChannelRadius(channel, radius);
  }

  EXPORT void CALL SetChannelPitch(Channel channel, float pitch)
  {
    Audio::Get().SetChannelPitch(channel, pitch);
  }

  EXPORT void CALL SetChannelVolume(Channel channel, float volume)
  {
    Audio::Get().SetChannelVolume(channel, volume);
  }

  EXPORT void CALL SetChannelPan(Channel channel, float pan)
  {
    Audio::Get().SetChannelPan(channel, pan);
  }

  EXPORT bool_t CALL ChannelPlaying(Channel channel)
  {
    return Audio::Get().ChannelPlaying(channel);
  }

} // extern "C"
