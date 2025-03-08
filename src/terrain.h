#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ITerrainSceneNode *CALL LoadTerrain(const char *heightmap, float width, float height, float depth);
  EXPORT void CALL ScaleTerrainTexture(ITerrainSceneNode *terrain, float scale1, float scale2);

#ifdef __cplusplus
} /* extern "C" */
#endif
