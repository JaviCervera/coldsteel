#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ICameraSceneNode *CALL CreateCamera();
  EXPORT void CALL SetCameraRange(ICameraSceneNode *cam, float near_, float far_);
  EXPORT float CALL CameraNearRange(ICameraSceneNode *cam);
  EXPORT float CALL CameraFarRange(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraFOV(ICameraSceneNode *cam, float fov);
  EXPORT float CALL CameraFOV(ICameraSceneNode *cam);
  EXPORT void CALL SetCameraOrtho(ICameraSceneNode *cam, float width, float height, float near_, float far_);
  EXPORT ISceneNode *CALL PickEntity(ICameraSceneNode *camera, int x, int y, int group);

#ifdef __cplusplus
} /* extern "C" */
#endif
