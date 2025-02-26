#include "audio_driver.h"
#include "channel.h"

extern "C"
{

  EXPORT void CALL StopChannel(Channel channel)
  {
    AudioDriver::Get().StopChannel(channel);
  }

  EXPORT void CALL PauseChannel(Channel channel)
  {
    AudioDriver::Get().PauseChannel(channel);
  }

  EXPORT void CALL ResumeChannel(Channel channel)
  {
    AudioDriver::Get().ResumeChannel(channel);
  }

  EXPORT void CALL SetChannelPosition(Channel channel, float x, float y, float z, float radius)
  {
    AudioDriver::Get().SetChannelPosition(channel, x, y, z, radius);
  }

  EXPORT void CALL SetChannelPitch(Channel channel, float pitch)
  {
    AudioDriver::Get().SetChannelPitch(channel, pitch);
  }

  EXPORT void CALL SetChannelVolume(Channel channel, float volume)
  {
    AudioDriver::Get().SetChannelVolume(channel, volume);
  }

  EXPORT void CALL SetChannelPan(Channel channel, float pan)
  {
    AudioDriver::Get().SetChannelPan(channel, pan);
  }

  EXPORT bool_t CALL ChannelPlaying(Channel channel)
  {
    return AudioDriver::Get().ChannelPlaying(channel);
  }

} // extern "C"
