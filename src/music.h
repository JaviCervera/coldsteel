/**
 * @file
 * Functions for playing background music streams.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Plays a music file.
   *
   * @param filename The music file to play.
   * @param loop True to repeat the music; otherwise false.
   * @return True on success; otherwise false.
   */
  EXPORT bool_t CALL PlayMusic(const char *filename, bool_t loop);

  /**
   * Stops the currently playing music.
   */
  EXPORT void CALL StopMusic();

  /**
   * Pauses the currently playing music.
   */
  EXPORT void CALL PauseMusic();

  /**
   * Resumes paused music.
   */
  EXPORT void CALL ResumeMusic();

  /**
   * Sets the music volume.
   *
   * @param volume The volume in the range 0.0 to 1.0.
   */
  EXPORT void CALL SetMusicVolume(float volume);

  /**
   * Returns whether music is currently playing.
   *
   * @return True if music is currently playing; otherwise false.
   */
  EXPORT bool_t CALL MusicPlaying();

#ifdef __cplusplus
} /* extern "C" */
#endif
