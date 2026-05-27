/** @file
 * The listener represents the position and orientation of the "ears" in 3D audio.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Sets the 3D position and yaw of the audio listener.
   *
   * @param x The listener X position.
   * @param y The listener Y position.
   * @param z The listener Z position.
   * @param yaw The listener yaw in degrees.
   */
  EXPORT void CALL SetListener(float x, float y, float z, float yaw);

#ifdef __cplusplus
} /* extern "C" */
#endif
