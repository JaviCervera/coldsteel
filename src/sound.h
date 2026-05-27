/**
 * @file
 * Functions for loading and playing sound effects. Sounds are played on Channels.
 */
#pragma once

#include "common.h"
#include "memblock.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef int Channel;
  struct Sound;

  /**
   * Loads a sound from a file.
   *
   * @param filename The path to the sound file to load.
   * @return The loaded sound.
   */
  EXPORT Sound *CALL LoadSound(const char *filename);

  /**
   * Frees a sound.
   *
   * @param sound The sound to free.
   */
  EXPORT void CALL FreeSound(Sound *sound);

  /**
   * Plays a sound.
   *
   * @param sound The sound to play.
   * @param loop True to loop the sound, or false to play it once.
   * @return A channel handle for the playing sound.
   */
  EXPORT Channel CALL PlaySound(Sound *sound, bool_t loop);

  /**
   * Plays a sound at a 3D world position.
   *
   * @param sound The sound to play.
   * @param x The world-space X position of the sound.
   * @param y The world-space Y position of the sound.
   * @param z The world-space Z position of the sound.
   * @param radius The audible radius of the sound.
   * @param loop True to loop the sound, or false to play it once.
   * @return A channel handle for the playing sound.
   */
  EXPORT Channel CALL PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool_t loop);

#ifdef __cplusplus
} /* extern "C" */
#endif
