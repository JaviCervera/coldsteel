/**
 * @file
 * A Channel represents a currently playing sound instance returned by PlaySound.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef int Channel;
  struct Sound;

  /**
   * Stops playback on a channel.
   *
   * @param channel The channel to stop.
   */
  EXPORT void CALL StopChannel(Channel channel);

  /**
   * Pauses playback on a channel.
   *
   * @param channel The channel to pause.
   */
  EXPORT void CALL PauseChannel(Channel channel);

  /**
   * Resumes playback on a paused channel.
   *
   * @param channel The channel to resume.
   */
  EXPORT void CALL ResumeChannel(Channel channel);

  /**
   * Sets the 3D world position of a channel.
   *
   * @param channel The channel whose position will be set.
   * @param x The X position in world space.
   * @param y The Y position in world space.
   * @param z The Z position in world space.
   */
  EXPORT void CALL SetChannel3DPosition(Channel channel, float x, float y, float z);

  /**
   * Sets the radius within which a channel is heard at full volume.
   *
   * @param channel The channel whose radius will be set.
   * @param radius The full-volume radius.
   */
  EXPORT void CALL SetChannelRadius(Channel channel, float radius);

  /**
   * Sets the pitch multiplier for a channel.
   *
   * @param channel The channel whose pitch will be set.
   * @param pitch The new pitch multiplier, where 1.0 is normal.
   */
  EXPORT void CALL SetChannelPitch(Channel channel, float pitch);

  /**
   * Sets the playback volume of a channel.
   *
   * @param channel The channel whose volume will be set.
   * @param volume The new volume from 0.0 to 1.0.
   */
  EXPORT void CALL SetChannelVolume(Channel channel, float volume);

  /**
   * Sets the stereo pan of a channel.
   *
   * @param channel The channel whose pan will be set.
   * @param pan The new pan, where -1.0 is left, 0.0 is center, and 1.0 is right.
   */
  EXPORT void CALL SetChannelPan(Channel channel, float pan);

  /**
   * Returns whether a channel is still playing.
   *
   * @param channel The channel to query.
   * @return TRUE if the channel is still playing, otherwise FALSE.
   */
  EXPORT bool_t CALL ChannelPlaying(Channel channel);

#ifdef __cplusplus
} /* extern "C" */
#endif
