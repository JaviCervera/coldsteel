#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetScreenTitle(const char *caption);
  EXPORT void CALL SetScreenSize(int width, int height);
  EXPORT bool_t CALL ScreenActive();
  EXPORT int CALL ScreenWidth();
  EXPORT int CALL ScreenHeight();
  EXPORT void CALL SetScreenFPS(int fps);
  EXPORT int CALL ScreenFPS();
  EXPORT bool_t CALL FeatureSupported(int feature);

#ifndef SWIG
  void _OpenScreen(int width, int height, int depth, int flags);
  void _OpenScreenEx(int width, int height, int depth, int flags, void *win);
  void _CloseScreen();
  int _ScreenNumModes();
  int _ScreenModeWidth(int index);
  int _ScreenModeHeight(int index);
  int _ScreenModeDepth(int index);
  int _DesktopWidth();
  int _DesktopHeight();
  int _DesktopDepth();
  int _ScreenFrameMsecs();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
