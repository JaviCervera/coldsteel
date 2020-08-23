#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT int CALL csRGB(int red, int green, int blue);
EXPORT int CALL csRGBA(int red, int green, int blue, int alpha);
EXPORT int CALL csRedVal(int color);
EXPORT int CALL csGreenVal(int color);
EXPORT int CALL csBlueVal(int color);
EXPORT int CALL csAlphaVal(int color);
EXPORT void CALL csBeginDrawing(bool_t clear, bool_t clearDepth, int color);
EXPORT void CALL csEndDrawing();
EXPORT void CALL csSetRenderTarget(ITexture* tex, bool_t clear, bool_t clearDepth, int color);
EXPORT void CALL csSetViewport(int x, int y, int width, int height);
EXPORT void CALL csDrawLine(int x1, int y1, int x2, int y2, int color);
EXPORT void CALL csDrawRect(int x, int y, int width, int height, int color);
EXPORT void CALL csDraw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color);

SColor _csColor(int color);
int _csIntColor(const SColor& color);


#ifdef __cplusplus
} /* extern "C" */
#endif
