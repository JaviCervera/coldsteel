#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ITexture* CALL asCreateTexture(int width, int height);
EXPORT ITexture* CALL asCreateRenderTexture(int width, int height);
EXPORT ITexture* CALL asLoadTexture(const char* filename);
EXPORT void CALL asFreeTexture(ITexture* tex);
EXPORT void CALL asDrawTexture(ITexture* tex, int x, int y);
EXPORT void CALL asDrawTextureEx(ITexture* tex, int x, int y, int width, int height, int color);
EXPORT void* CALL asLockTexture(ITexture* tex);
EXPORT void CALL asUnlockTexture(ITexture* tex);
EXPORT void CALL asSetTextureColorKey(ITexture* tex, int color);
EXPORT void CALL asNormalizeTexture(ITexture* tex, float amplitude);
EXPORT int CALL asTextureWidth(ITexture* tex);
EXPORT int CALL asTextureHeight(ITexture* tex);
EXPORT const char* CALL asTextureFilename(ITexture* tex);
EXPORT void CALL asSetTextureCreationFlags(int flags);


#ifdef __cplusplus
} /* extern "C" */
#endif
