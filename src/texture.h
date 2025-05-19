/** @file */
#pragma once

#include "common.h"

// Texture flags
#define TEXTURE_MIPMAPS 1
#define TEXTURE_16BITS 2

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ITexture *CALL CreateTexture(int width, int height);
  EXPORT ITexture *CALL CreateTextureFromPixmap(IImage *pixmap);
  EXPORT ITexture *CALL CreateRenderTexture(int width, int height);
  EXPORT ITexture *CALL LoadTexture(const char *filename);
  EXPORT void CALL ClearTextureCache();
  EXPORT void CALL DrawTexture(ITexture *tex, int x, int y);
  EXPORT void CALL DrawTextureEx(ITexture *tex, int x, int y, int width, int height, int color);
  EXPORT void CALL DrawTextureEx2(ITexture *tex, int src_x, int src_y, int src_width, int src_height, int dst_x, int dst_y, int dst_width, int dst_height, int color);
  EXPORT void *CALL LockTexture(ITexture *tex);
  EXPORT void CALL UnlockTexture(ITexture *tex);
  EXPORT void CALL SetTextureColorKey(ITexture *tex, int color);
  EXPORT void CALL NormalizeTexture(ITexture *tex, float amplitude);
  EXPORT int CALL TextureWidth(ITexture *tex);
  EXPORT int CALL TextureHeight(ITexture *tex);
  EXPORT const char *CALL TextureFilename(ITexture *tex);
  EXPORT void CALL SetTextureCreationFlags(int flags);

#ifdef __cplusplus
} /* extern "C" */
#endif
