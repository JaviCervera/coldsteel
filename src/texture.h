#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ITexture* CALL csCreateTexture(int width, int height);
EXPORT ITexture* CALL csCreateRenderTexture(int width, int height);
EXPORT ITexture* CALL csLoadTexture(const char* filename);
EXPORT void CALL csFreeTexture(ITexture* tex);
EXPORT void CALL csDrawTexture(ITexture* tex, int x, int y);
EXPORT void CALL csDrawTextureEx(ITexture* tex, int x, int y, int width, int height, int color);
EXPORT void* CALL csLockTexture(ITexture* tex);
EXPORT void CALL csUnlockTexture(ITexture* tex);
EXPORT void CALL csSetTextureColorKey(ITexture* tex, int color);
EXPORT void CALL csNormalizeTexture(ITexture* tex, float amplitude);
EXPORT int CALL csTextureWidth(ITexture* tex);
EXPORT int CALL csTextureHeight(ITexture* tex);
EXPORT const char* CALL csTextureFilename(ITexture* tex);
EXPORT void CALL csSetTextureCreationFlags(int flags);


#ifdef __cplusplus
} /* extern "C" */
#endif
