#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asOpenScreen(int width, int height, int depth, int flags);
EXPORT void CALL asOpenScreenEx(int width, int height, int depth, int flags, void* win);
EXPORT void CALL asCloseScreen();
EXPORT void CALL asSetScreenTitle(const char* caption);
EXPORT void CALL asSetScreenSize(int width, int height);
EXPORT bool_t CALL asScreenActive();
EXPORT int CALL asScreenWidth();
EXPORT int CALL asScreenHeight();
EXPORT void CALL asSetScreenFPS(int fps);
EXPORT int CALL asScreenFPS();
EXPORT bool_t CALL asFeatureSupported(int feature);
EXPORT int CALL asScreenNumModes();
EXPORT int CALL asScreenModeWidth(int index);
EXPORT int CALL asScreenModeHeight(int index);
EXPORT int CALL asScreenModeDepth(int index);
EXPORT int CALL asDesktopWidth();
EXPORT int CALL asDesktopHeight();
EXPORT int CALL asDesktopDepth();


int _asScreenFrameMsecs();


#ifdef __cplusplus
} /* extern "C" */
#endif
