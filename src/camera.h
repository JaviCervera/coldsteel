/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ICameraSceneNode *CALL CreateCamera();
  EXPORT void CALL SetCameraActive(ICameraSceneNode *cam, bool_t active);
  EXPORT bool_t CALL CameraActive(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraViewport(ICameraSceneNode *cam, int x, int y, int width, int height);
  EXPORT int CALL CameraViewportX(ICameraSceneNode *cam);
  EXPORT int CALL CameraViewportY(ICameraSceneNode *cam);
  EXPORT int CALL CameraViewportWidth(ICameraSceneNode *cam);
  EXPORT int CALL CameraViewportHeight(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraClearMode(ICameraSceneNode *cam, bool_t clear_color, bool_t clear_depth);
  EXPORT bool_t CALL CameraClearColorEnabled(ICameraSceneNode *cam);
  EXPORT bool_t CALL CameraClearDepthEnabled(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraClearColor(ICameraSceneNode *cam, int color);
  EXPORT int CALL CameraClearColor(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraRenderTarget(ICameraSceneNode *cam, ITexture *target);
  EXPORT ITexture *CALL CameraRenderTarget(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraRange(ICameraSceneNode *cam, float near_, float far_);
  EXPORT float CALL CameraNearRange(ICameraSceneNode *cam);
  EXPORT float CALL CameraFarRange(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraAspectRatio(ICameraSceneNode *cam, float ratio);
  EXPORT float CALL CameraAspectRatio(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraFOV(ICameraSceneNode *cam, float fov);
  EXPORT float CALL CameraFOV(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraOrtho(ICameraSceneNode *cam, bool_t enable);
  EXPORT bool_t CALL CameraOrtho(ICameraSceneNode *cam);
  EXPORT ISceneNode *CALL PickEntity(ICameraSceneNode *camera, int x, int y, int group);

#ifdef __cplusplus
} /* extern "C" */
#endif
