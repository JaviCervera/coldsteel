/**
 * @file
 * Bakes direct lighting into a lightmap atlas. The lightmap is applied to every static mesh
 * under the given root node and the resulting atlas is returned as a pixmap.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Bakes direct lighting into a lightmap atlas.
   *
   * All static mesh nodes under root (including meshes nested inside empty entities) receive
   * lightmap UVs, a lightmapped material and a lightmap texture. The lightmap is rendered in
   * real time; this function is typically called once after the scene has been built.
   *
   * @param root The scene node (entity) to bake. Pass NULL to bake the whole scene.
   * @param texelDensity The number of lightmap texels per world unit.
   * @param maxAtlasSize The maximum atlas width/height in pixels. If the baked triangles do not
   *        fit in an atlas of this size, NULL is returned and the scene is left untouched.
   * @return A pixmap containing the lightmap atlas, or NULL if the bake failed.
   */
  EXPORT IImage *CALL BakeLightmaps(ISceneNode *root, float texelDensity, int maxAtlasSize);

#ifdef __cplusplus
} /* extern "C" */
#endif