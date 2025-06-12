/** @file */
#pragma once

#include "common.h"

// Drivers
#define DRIVER_SOFTWAREFAST 0
#define DRIVER_SOFTWARE 1
#define DRIVER_OPENGL 2
#define DRIVER_DIRECT3D 3

// Flags
#define SCREEN_FULLSCREEN 1
#define SCREEN_RESIZABLE 2
#define SCREEN_VSYNC 4

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetDriver(int driver);
  EXPORT const char *CALL DriverName();
  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags);
  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win);
  EXPORT void CALL CloseScreen();
  EXPORT void CALL ClearScreen(int color);
  EXPORT void CALL RefreshScreen();
  EXPORT void CALL SetScreenTitle(const char *caption);
  EXPORT void CALL SetScreenSize(int width, int height);
  EXPORT bool_t CALL ScreenActive();
  EXPORT bool_t CALL ScreenShouldClose();
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
  EXPORT void CALL MinimizeWindow();
  EXPORT void CALL MaximizeWindow();
  EXPORT void CALL RestoreWindow();

#ifndef SWIG
  int _ScreenFrameMsecs();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
