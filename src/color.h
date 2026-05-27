/**
 * @file
 * Functions for creating and decomposing colors. Colors are 32-bit ARGB integers.
 */
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

  /**
   * Creates an opaque color from red, green, and blue components.
   *
   * @param red The red component from 0 to 255.
   * @param green The green component from 0 to 255.
   * @param blue The blue component from 0 to 255.
   * @return A 32-bit ARGB color with full opacity.
   */
  EXPORT int CALL RGB(int red, int green, int blue);

  /**
   * Creates a color from red, green, blue, and alpha components.
   *
   * @param red The red component from 0 to 255.
   * @param green The green component from 0 to 255.
   * @param blue The blue component from 0 to 255.
   * @param alpha The alpha component from 0 to 255.
   * @return A 32-bit ARGB color.
   */
  EXPORT int CALL RGBA(int red, int green, int blue, int alpha);

  /**
   * Creates an opaque color from floating-point red, green, and blue components.
   *
   * @param red The red component from 0.0 to 1.0.
   * @param green The green component from 0.0 to 1.0.
   * @param blue The blue component from 0.0 to 1.0.
   * @return A 32-bit ARGB color with full opacity.
   */
  EXPORT int CALL RGBF(float red, float green, float blue);

  /**
   * Creates a color from floating-point red, green, blue, and alpha components.
   *
   * @param red The red component from 0.0 to 1.0.
   * @param green The green component from 0.0 to 1.0.
   * @param blue The blue component from 0.0 to 1.0.
   * @param alpha The alpha component from 0.0 to 1.0.
   * @return A 32-bit ARGB color.
   */
  EXPORT int CALL RGBAF(float red, float green, float blue, float alpha);

  /**
   * Returns the red component of a color.
   *
   * @param color The color to query.
   * @return The red component from 0 to 255.
   */
  EXPORT int CALL RedVal(int color);

  /**
   * Returns the green component of a color.
   *
   * @param color The color to query.
   * @return The green component from 0 to 255.
   */
  EXPORT int CALL GreenVal(int color);

  /**
   * Returns the blue component of a color.
   *
   * @param color The color to query.
   * @return The blue component from 0 to 255.
   */
  EXPORT int CALL BlueVal(int color);

  /**
   * Returns the alpha component of a color.
   *
   * @param color The color to query.
   * @return The alpha component from 0 to 255.
   */
  EXPORT int CALL AlphaVal(int color);

  /**
   * Returns the red component of a color as a floating-point value.
   *
   * @param color The color to query.
   * @return The red component from 0.0 to 1.0.
   */
  EXPORT float CALL RedValF(int color);

  /**
   * Returns the green component of a color as a floating-point value.
   *
   * @param color The color to query.
   * @return The green component from 0.0 to 1.0.
   */
  EXPORT float CALL GreenValF(int color);

  /**
   * Returns the blue component of a color as a floating-point value.
   *
   * @param color The color to query.
   * @return The blue component from 0.0 to 1.0.
   */
  EXPORT float CALL BlueValF(int color);

  /**
   * Returns the alpha component of a color as a floating-point value.
   *
   * @param color The color to query.
   * @return The alpha component from 0.0 to 1.0.
   */
  EXPORT float CALL AlphaValF(int color);

  /**
   * Returns a color with a replaced alpha component.
   *
   * @param color The original color.
   * @param new_alpha The new alpha component from 0 to 255.
   * @return The color with its alpha replaced by new_alpha.
   */
  EXPORT int CALL FadeColor(int color, int new_alpha);

  /**
   * Returns a color with its RGB components multiplied by a factor.
   *
   * @param color The original color.
   * @param factor The factor applied to the red, green, and blue components.
   * @return The color with its RGB components multiplied by factor.
   */
  EXPORT int CALL MultiplyColor(int color, float factor);

  /**
   * Returns a color with its red and blue channels swapped.
   *
   * @param color The color to convert.
   * @return The color with red and blue channels swapped.
   */
  EXPORT int CALL SwapColor(int color);

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifndef SWIG
SColor _Color(int color);
int _IntColor(const SColor &color);
#endif
