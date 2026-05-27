/**
 * @file
 * Terrain entities generated from heightmaps.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Loads a terrain from a heightmap file with the given world dimensions.
   *
   * @param heightmap The path to the heightmap file.
   * @param width The terrain width in world units.
   * @param height The terrain height scale in world units.
   * @param depth The terrain depth in world units.
   * @return The loaded terrain entity.
   */
  EXPORT ITerrainSceneNode *CALL LoadTerrain(const char *heightmap, float width, float height, float depth);

  /**
   * Sets the texture scale for the two terrain texture layers.
   *
   * @param terrain The terrain to update.
   * @param scale1 The texture scale for the first layer.
   * @param scale2 The texture scale for the second layer.
   */
  EXPORT void CALL ScaleTerrainTexture(ITerrainSceneNode *terrain, float scale1, float scale2);

#ifdef __cplusplus
} /* extern "C" */
#endif
