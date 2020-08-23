#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csOpenScreen(int width, int height, int depth, int flags);
EXPORT void CALL csOpenScreenEx(int width, int height, int depth, int flags, void* win);
EXPORT void CALL csCloseScreen();
EXPORT void CALL csSetScreenTitle(const char* caption);
EXPORT void CALL csSetScreenSize(int width, int height);
EXPORT bool_t CALL csScreenActive();
EXPORT int CALL csScreenWidth();
EXPORT int CALL csScreenHeight();
EXPORT void CALL csSetScreenFPS(int fps);
EXPORT int CALL csScreenFPS();
EXPORT bool_t CALL csFeatureSupported(int feature);
EXPORT int CALL csScreenNumModes();
EXPORT int CALL csScreenModeWidth(int index);
EXPORT int CALL csScreenModeHeight(int index);
EXPORT int CALL csScreenModeDepth(int index);
EXPORT int CALL csDesktopWidth();
EXPORT int CALL csDesktopHeight();
EXPORT int CALL csDesktopDepth();


int _csScreenFrameMsecs();


#ifdef __cplusplus
} /* extern "C" */
#endif
