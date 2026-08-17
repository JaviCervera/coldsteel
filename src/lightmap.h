/**
 * @file
 * Bakes direct and optionally bounced (global illumination) lighting into a lightmap atlas. The
 * lightmap is applied to every static mesh under the given root node and the resulting atlas is
 * returned as a pixmap.
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
   * Bounce lighting is a progressive radiosity gather in the style of Quake's qrad / Half-Life's
   * hlrad: after the direct pass, each surface reflects its accumulated radiance (albedo * light)
   * onto every other surface, scaled by a Lambert form factor and one hard shadow ray per texel.
   * bounces = 0 keeps the bake direct-only.
   *
   * @param root The scene node (entity) to bake. Pass NULL to bake the whole scene.
   * @param texelDensity The number of lightmap texels per world unit.
   * @param maxAtlasSize The maximum atlas width/height in pixels. If the baked triangles do not
   *        fit in an atlas of this size, NULL is returned and the scene is left untouched.
   * @param bounces The number of radiosity bounces. 0 = direct lighting only.
   * @param useTextureAlbedo If true, reflectance for bounces is the material DiffuseColor blended
   *        with the average color of the material's diffuse texture; otherwise only DiffuseColor.
   * @return A pixmap containing the lightmap atlas, or NULL if the bake failed.
   */
  EXPORT IImage *CALL BakeLightmaps(ISceneNode *root, float texelDensity, int maxAtlasSize,
                                    int bounces = 0, bool useTextureAlbedo = false);

#ifdef __cplusplus
} /* extern "C" */
#endif