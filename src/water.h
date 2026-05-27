/**
 * @file
 * Water volume entities with animated waves.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a water plane with the given dimensions, tile subdivisions, and wave parameters.
   *
   * @param width The water width in world units.
   * @param depth The water depth in world units.
   * @param tiles_width The number of tile subdivisions along the width.
   * @param tiles_depth The number of tile subdivisions along the depth.
   * @param wave_height The wave height.
   * @param wave_speed The wave animation speed.
   * @param wave_length The wave length.
   * @return A new water volume entity.
   */
  EXPORT ISceneNode *CALL CreateWaterVolume(float width, float depth, int tiles_width, int tiles_depth, float wave_height, float wave_speed, float wave_length);

#ifdef __cplusplus
} /* extern "C" */
#endif
