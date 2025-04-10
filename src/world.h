#pragma once

#include "common.h"
#include "graphics.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetAmbient(int color);
  EXPORT void CALL SetShadowColor(int color);
  EXPORT void CALL SetFog(int color, float near_, float far_);
  EXPORT void CALL SetSkybox(ITexture *top, ITexture *bottom, ITexture *left, ITexture *right, ITexture *front, ITexture *back);
  EXPORT void CALL SetSkydome(ITexture *tex, float texScale, bool_t fullSphere);
  EXPORT SMaterial *CALL SkyboxMaterial();
  EXPORT SMaterial *CALL SkydomeMaterial();
  EXPORT void CALL DrawWorld();
  EXPORT void CALL WorldToScreen(ICameraSceneNode *camera, float x, float y, float z);
  EXPORT void CALL ScreenToWorld(ICameraSceneNode *camera, int x, int y, float depth);
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
