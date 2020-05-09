#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ICameraSceneNode* CALL asCreateCamera();
EXPORT void CALL asSetCameraRange(ICameraSceneNode* cam, float near_, float far_);
EXPORT float CALL asCameraNearRange(ICameraSceneNode* cam);
EXPORT float CALL asCameraFarRange(ICameraSceneNode* cam);
EXPORT void CALL asSetCameraFOV(ICameraSceneNode* cam, float fov);
EXPORT float CALL asCameraFOV(ICameraSceneNode* cam);
EXPORT void CALL asSetCameraProjection(ICameraSceneNode* cam, float width, float height, float near_, float far_, bool_t ortho);
EXPORT ISceneNode* CALL asPickEntity(ICameraSceneNode* camera, int x, int y, int group);


#ifdef __cplusplus
} /* extern "C" */
#endif
