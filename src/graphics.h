#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetRenderTarget(ITexture *tex, bool_t clear, bool_t clearDepth, int color);
  EXPORT void CALL SetViewport(int x, int y, int width, int height);
  EXPORT void CALL DrawPoint(int x, int y, int color);
  EXPORT void CALL DrawLine(int x1, int y1, int x2, int y2, int color);
  EXPORT void CALL DrawRect(int x, int y, int width, int height, int color);
  EXPORT void CALL Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color);

#ifdef __cplusplus
} /* extern "C" */
#endif
