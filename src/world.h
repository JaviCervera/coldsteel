/**
 * @file
 * Scene-wide settings and operations including ambient light, fog, skybox, and scene rendering.
 */
#pragma once

#include "color.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Sets the global ambient light color.
   *
   * @param color The ambient light color.
   */
  EXPORT void CALL SetAmbient(int color);

  /**
   * Sets the shadow volume color.
   *
   * @param color The shadow volume color.
   */
  EXPORT void CALL SetShadowColor(int color);

  /**
   * Enables distance fog with the given color and near/far distances.
   *
   * @param color The fog color.
   * @param near_ The near distance where fog begins.
   * @param far_ The far distance where fog reaches full strength.
   */
  EXPORT void CALL SetFog(int color, float near_, float far_);

  /**
   * Sets a skybox using six textures.
   *
   * @param top The texture for the top face.
   * @param bottom The texture for the bottom face.
   * @param left The texture for the left face.
   * @param right The texture for the right face.
   * @param front The texture for the front face.
   * @param back The texture for the back face.
   */
  EXPORT void CALL SetSkybox(ITexture *top, ITexture *bottom, ITexture *left, ITexture *right, ITexture *front, ITexture *back);

  /**
   * Sets a skydome using a single texture.
   *
   * @param tex The texture to apply to the skydome.
   * @param texScale The UV tiling scale.
   * @param fullSphere True to use a full sphere, or false to use a half sphere.
   */
  EXPORT void CALL SetSkydome(ITexture *tex, float texScale, bool_t fullSphere);

  /**
   * Returns the material of the skybox.
   *
   * @return The material of the skybox.
   */
  EXPORT SMaterial *CALL SkyboxMaterial();

  /**
   * Returns the material of the skydome.
   *
   * @return The material of the skydome.
   */
  EXPORT SMaterial *CALL SkydomeMaterial();

  /**
   * Renders the entire scene using all active cameras.
   */
  EXPORT void CALL DrawWorld();

  /**
   * Projects a 3D world position to 2D screen coordinates. Read the result with PointX and PointY.
   *
   * @param camera The camera used for the projection.
   * @param x The world-space X position.
   * @param y The world-space Y position.
   * @param z The world-space Z position.
   */
  EXPORT void CALL WorldToScreen(ICameraSceneNode *camera, float x, float y, float z);

  /**
   * Unprojects a 2D screen position at the given depth to 3D world coordinates. Read the result
   * with PointX, PointY, and PointZ.
   *
   * @param camera The camera used for the unprojection.
   * @param x The screen-space X position in pixels.
   * @param y The screen-space Y position in pixels.
   * @param depth The depth value to unproject at.
   */
  EXPORT void CALL ScreenToWorld(ICameraSceneNode *camera, int x, int y, float depth);

  /**
   * Casts a ray and returns the first entity hit in the given collision group. The hit point and
   * normal are stored in PointX, PointY, PointZ and NormalX, NormalY, NormalZ.
   *
   * @param x1 The ray start X position.
   * @param y1 The ray start Y position.
   * @param z1 The ray start Z position.
   * @param x2 The ray end X position.
   * @param y2 The ray end Y position.
   * @param z2 The ray end Z position.
   * @param group The collision group to test against, or 0 for all groups.
   * @return The first entity hit in the requested collision group.
   */
  EXPORT ISceneNode *CALL Raycast(float x1, float y1, float z1, float x2, float y2, float z2, int group);

#ifndef SWIG
  struct CameraData
  {
    bool active;
    bool isOrtho;
    float ratio;
    recti viewport;
    int clearFlags;
    int clearColor;
    ITexture *renderTarget;

    CameraData()
        : active(true),
          isOrtho(false),
          ratio(0),
          viewport(),
          clearFlags(3),
          clearColor(RGB(0, 0, 64)),
          renderTarget(NULL)
    {
    }
  };

  bool _HasSetAmbient();
  void _AddCamera(ICameraSceneNode *camera);
  void _RemoveCamera(ICameraSceneNode *camera);
  CameraData *_CameraData(ICameraSceneNode *camera);

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
