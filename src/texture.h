#pragma once

#include "common.h"

ITexture *CreateTexture(int width, int height);
ITexture *CreateRenderTexture(int width, int height);
ITexture *LoadTexture(const char *filename);
void ClearTextureCache();
void DrawTexture(ITexture *tex, int x, int y);
void DrawTextureEx(ITexture *tex, int x, int y, int width, int height, int color);
void *LockTexture(ITexture *tex);
void UnlockTexture(ITexture *tex);
void SetTextureColorKey(ITexture *tex, int color);
void NormalizeTexture(ITexture *tex, float amplitude);
int TextureWidth(ITexture *tex);
int TextureHeight(ITexture *tex);
const char *TextureFilename(ITexture *tex);
void SetTextureCreationFlags(int flags);
