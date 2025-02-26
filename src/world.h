#pragma once

#include "common.h"

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
  EXPORT void CALL DrawWorld(ICameraSceneNode *camera);
  EXPORT void CALL WorldToScreen(ICameraSceneNode *camera, float x, float y, float z);
  EXPORT void CALL ScreenToWorld(ICameraSceneNode *camera, int x, int y, float depth);
  EXPORT ISceneNode *CALL Raycast(float x1, float y1, float z1, float x2, float y2, float z2, int group);

#ifndef SWIG
  bool _HasSetAmbient();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
