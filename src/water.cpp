/** @file */
#pragma once

#include "color.h"
#include "core.h"
#include "material.h"
#include "math.h"
#include "mesh.h"
#include "string.h"
#include "water.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ISceneNode *CALL CreateWaterVolume(float width, float depth, int tiles_width, int tiles_depth, float wave_height, float wave_speed, float wave_length)
  {
    const dimension2df tile_count(tiles_width, tiles_depth);
    IAnimatedMesh *hill = _Device()->getSceneManager()->addHillPlaneMesh(
        Str(Rand(0, INT_MAX)),
        dimension2df(width / tiles_width, depth / tiles_depth),
        dimension2du(tiles_width, tiles_depth),
        NULL,
        0,
        tile_count,
        tile_count);
    TranslateMesh(hill->getMesh(0), width / 2, 0, depth / 2);
    _FixMaterial(&hill->getMesh(0)->getMeshBuffer(0)->getMaterial(), true);
    ISceneNode *water = _Device()->getSceneManager()->addWaterSurfaceSceneNode(hill->getMesh(0), wave_height, wave_speed, wave_length);
    hill->drop();
    return water;
  }

#ifdef __cplusplus
} /* extern "C" */
#endif
