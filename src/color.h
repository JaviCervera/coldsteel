#pragma once

#include "common.h"

#define COLOR_RED -65536
#define COLOR_GREEN -16711936
#define COLOR_BLUE -16776961
#define COLOR_CYAN -16711681
#define COLOR_MAGENTA -65281
#define COLOR_YELLOW -256
#define COLOR_BLACK -16777216
#define COLOR_WHITE -1
#define COLOR_GRAY -8355712
#define COLOR_LIGHTGRAY -4210753
#define COLOR_DARKGRAY -12566464
#define COLOR_ORANGE -23296
#define COLOR_BROWN -7650029

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT int CALL RGB(int red, int green, int blue);
  EXPORT int CALL RGBA(int red, int green, int blue, int alpha);
  EXPORT int CALL RGBF(float red, float green, float blue);
  EXPORT int CALL RGBAF(float red, float green, float blue, float alpha);
  EXPORT int CALL RedVal(int color);
  EXPORT int CALL GreenVal(int color);
  EXPORT int CALL BlueVal(int color);
  EXPORT int CALL AlphaVal(int color);
  EXPORT float CALL RedValF(int color);
  EXPORT float CALL GreenValF(int color);
  EXPORT float CALL BlueValF(int color);
  EXPORT float CALL AlphaValF(int color);
  EXPORT int CALL FadeColor(int color, int new_alpha);
  EXPORT int CALL MultiplyColor(int color, float factor);
  EXPORT int CALL SwapColor(int color);

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifndef SWIG
SColor _Color(int color);
int _IntColor(const SColor &color);
#endif
