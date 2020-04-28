#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asSetAmbient(int color);
EXPORT void CALL asSetShadowColor(int color);
EXPORT void CALL asSetFog(int color, float near_, float far_);
EXPORT void CALL asSetSkybox(ITexture* top, ITexture* bottom, ITexture* left, ITexture* right, ITexture* front, ITexture* back);
EXPORT void CALL asSetSkydome(ITexture* tex, float texScale, bool_t fullSphere);
EXPORT SMaterial* CALL asSkyboxMaterial();
EXPORT SMaterial* CALL asSkydomeMaterial();
EXPORT void CALL asDrawWorld(ICameraSceneNode* camera);
EXPORT void CALL asWorldToScreen(ICameraSceneNode* camera, float x, float y, float z);
EXPORT void CALL asScreenToWorld(ICameraSceneNode* camera, int x, int y, float depth);
EXPORT ISceneNode* CALL asRaycast(float x1, float y1, float z1, float x2, float y2, float z2, int group);


#ifdef __cplusplus
} /* extern "C" */
#endif
