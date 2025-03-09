#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags);
  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win);
  EXPORT void CALL CloseScreen();
  EXPORT void CALL SetScreenTitle(const char *caption);
  EXPORT void CALL SetScreenSize(int width, int height);
  EXPORT bool_t CALL ScreenActive();
  EXPORT int CALL ScreenWidth();
  EXPORT int CALL ScreenHeight();
  EXPORT void CALL SetScreenFPS(int fps);
  EXPORT int CALL ScreenFPS();
  EXPORT int CALL ScreenNumModes();
  EXPORT int CALL ScreenModeWidth(int index);
  EXPORT int CALL ScreenModeHeight(int index);
  EXPORT int CALL ScreenModeDepth(int index);
  EXPORT int CALL DesktopWidth();
  EXPORT int CALL DesktopHeight();
  EXPORT int CALL DesktopDepth();
  EXPORT bool_t CALL FeatureSupported(int feature);

#ifndef SWIG
  int _ScreenFrameMsecs();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
