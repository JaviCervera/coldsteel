#pragma once

#include "common.h"

void SetScreenTitle(const char *caption);
void SetScreenSize(int width, int height);
bool_t ScreenActive();
int ScreenWidth();
int ScreenHeight();
void SetScreenFPS(int fps);
int ScreenFPS();
bool_t FeatureSupported(int feature);

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
