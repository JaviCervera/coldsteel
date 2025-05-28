/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef int Channel;
  struct Sound;

  EXPORT void CALL StopChannel(Channel channel);
  EXPORT void CALL PauseChannel(Channel channel);
  EXPORT void CALL ResumeChannel(Channel channel);
  EXPORT void CALL SetChannelPosition(Channel channel, float x, float y, float z);
  EXPORT void CALL SetChannelVelocity(Channel channel, float x, float y, float z);
  EXPORT void CALL SetChannelRadius(Channel channel, float radius);
  EXPORT void CALL SetChannelPitch(Channel channel, float pitch);
  EXPORT void CALL SetChannelVolume(Channel channel, float volume);
  EXPORT void CALL SetChannelPan(Channel channel, float pan);
  EXPORT bool_t CALL ChannelPlaying(Channel channel);

#ifdef __cplusplus
} /* extern "C" */
#endif
