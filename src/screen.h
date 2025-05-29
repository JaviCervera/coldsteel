/** @file */
#pragma once

#include "common.h"

// Flags
#define SCREEN_FULLSCREEN 1
#define SCREEN_RESIZABLE 2
#define SCREEN_VSYNC 4

// Features
#define FEATURE_RENDERTOTEXTURE 0
#define FEATURE_BILINEARFILTER 1
#define FEATURE_HARDWARETNL 2
#define FEATURE_MIPMAP 3
#define FEATURE_STENCILBUFFER 4
#define FEATURE_VERTEXSHADER11 5
#define FEATURE_VERTEXSHADER20 6
#define FEATURE_VERTEXSHADER30 7
#define FEATURE_PIXELSHADER11 8
#define FEATURE_PIXELSHADER12 9
#define FEATURE_PIXELSHADER13 10
#define FEATURE_PIXELSHADER14 11
#define FEATURE_PIXELSHADER20 12
#define FEATURE_PIXELSHADER30 13
#define FEATURE_ARB_VERTEXPROGRAM1 14
#define FEATURE_ARB_FRAGMENTPROGRAM1 15

#ifdef __cplusplus
extern "C"
{
#endif

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
  EXPORT bool_t CALL FeatureSupported(int feature);

#ifdef __cplusplus
} /* extern "C" */
#endif
