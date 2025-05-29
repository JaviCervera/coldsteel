#include "include/audio.h"
#include "include/engine.h"
#include "channel.h"

extern "C"
{

  EXPORT void CALL StopChannel(Channel channel)
  {
    GetEngine().GetAudio().StopChannel(channel);
  }

  EXPORT void CALL PauseChannel(Channel channel)
  {
    GetEngine().GetAudio().PauseChannel(channel);
  }

  EXPORT void CALL ResumeChannel(Channel channel)
  {
    GetEngine().GetAudio().ResumeChannel(channel);
  }

  EXPORT void CALL SetChannel3DPosition(Channel channel, float x, float y, float z)
  {
    GetEngine().GetAudio().SetChannel3DPosition(channel, x, y, z);
  }

  EXPORT void CALL SetChannelRadius(Channel channel, float radius)
  {
    GetEngine().GetAudio().SetChannelRadius(channel, radius);
  }

  EXPORT void CALL SetChannelPitch(Channel channel, float pitch)
  {
    GetEngine().GetAudio().SetChannelPitch(channel, pitch);
  }

  EXPORT void CALL SetChannelVolume(Channel channel, float volume)
  {
    GetEngine().GetAudio().SetChannelVolume(channel, volume);
  }

  EXPORT void CALL SetChannelPan(Channel channel, float pan)
  {
    GetEngine().GetAudio().SetChannelPan(channel, pan);
  }

  EXPORT bool_t CALL ChannelPlaying(Channel channel)
  {
    return GetEngine().GetAudio().ChannelPlaying(channel);
  }

} // extern "C"
