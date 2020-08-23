#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csSetAmbient(int color);
EXPORT void CALL csSetShadowColor(int color);
EXPORT void CALL csSetFog(int color, float near_, float far_);
EXPORT void CALL csSetSkybox(ITexture* top, ITexture* bottom, ITexture* left, ITexture* right, ITexture* front, ITexture* back);
EXPORT void CALL csSetSkydome(ITexture* tex, float texScale, bool_t fullSphere);
EXPORT SMaterial* CALL csSkyboxMaterial();
EXPORT SMaterial* CALL csSkydomeMaterial();
EXPORT void CALL csDrawWorld(ICameraSceneNode* camera);
EXPORT void CALL csWorldToScreen(ICameraSceneNode* camera, float x, float y, float z);
EXPORT void CALL csScreenToWorld(ICameraSceneNode* camera, int x, int y, float depth);
EXPORT ISceneNode* CALL csRaycast(float x1, float y1, float z1, float x2, float y2, float z2, int group);


#ifdef __cplusplus
} /* extern "C" */
#endif
