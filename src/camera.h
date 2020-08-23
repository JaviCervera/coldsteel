#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ICameraSceneNode* CALL csCreateCamera();
EXPORT void CALL csSetCameraRange(ICameraSceneNode* cam, float near_, float far_);
EXPORT float CALL csCameraNearRange(ICameraSceneNode* cam);
EXPORT float CALL csCameraFarRange(ICameraSceneNode* cam);
EXPORT void CALL csSetCameraFOV(ICameraSceneNode* cam, float fov);
EXPORT float CALL csCameraFOV(ICameraSceneNode* cam);
EXPORT void CALL csSetCameraProjection(ICameraSceneNode* cam, float width, float height, float near_, float far_, bool_t ortho);
EXPORT ISceneNode* CALL csPickEntity(ICameraSceneNode* camera, int x, int y, int group);


#ifdef __cplusplus
} /* extern "C" */
#endif
