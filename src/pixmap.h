#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT IImage *CALL CreatePixmap(int width, int height);
  EXPORT IImage *CALL CreatePixmapFromTexture(ITexture *texture);
  EXPORT IImage *CALL LoadPixmap(const char *filename);
  EXPORT void CALL FreePixmap(IImage *pixmap);
  EXPORT void CALL SavePixmap(IImage *pixmap, const char *filename);
  EXPORT int CALL ReadPixel(IImage *pixmap, int x, int y);
  EXPORT void CALL WritePixel(IImage *pixmap, int x, int y, int color);
  EXPORT int CALL PixmapWidth(IImage *pixmap);
  EXPORT int CALL PixmapHeight(IImage *pixmap);

#ifdef __cplusplus
} /* extern "C" */
#endif
