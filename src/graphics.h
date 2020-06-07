#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT int CALL asRGB(int red, int green, int blue);
EXPORT int CALL asRGBA(int red, int green, int blue, int alpha);
EXPORT int CALL asRedVal(int color);
EXPORT int CALL asGreenVal(int color);
EXPORT int CALL asBlueVal(int color);
EXPORT int CALL asAlphaVal(int color);
EXPORT void CALL asBeginDrawing(bool_t clear, bool_t clearDepth, int color);
EXPORT void CALL asEndDrawing();
EXPORT void CALL asSetRenderTarget(ITexture* tex, bool_t clear, bool_t clearDepth, int color);
EXPORT void CALL asSetViewport(int x, int y, int width, int height);
EXPORT void CALL asDrawLine(int x1, int y1, int x2, int y2, int color);
EXPORT void CALL asDrawRect(int x, int y, int width, int height, int color);
EXPORT void CALL asDraw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color);

SColor _asColor(int color);
int _asIntColor(const SColor& color);


#ifdef __cplusplus
} /* extern "C" */
#endif
