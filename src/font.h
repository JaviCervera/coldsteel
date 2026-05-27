/** @file
 * Functions for loading and drawing TrueType fonts.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Font;

  /**
   * Loads a font from a TrueType file.
   *
   * @param filename The path to the TrueType font file.
   * @param height The font height in pixels.
   * @return The loaded font.
   */
  EXPORT Font *CALL LoadFont(const char *filename, int height);

  /**
   * Frees a font.
   *
   * @param font The font to free.
   */
  EXPORT void CALL FreeFont(Font *font);

  /**
   * Draws text on the screen using the given font.
   *
   * @param font The font to use for drawing.
   * @param text The text to draw.
   * @param x The screen X position.
   * @param y The screen Y position.
   * @param color The text color.
   */
  EXPORT void CALL DrawText(Font *font, const char *text, int x, int y, int color);

  /**
   * Returns the pixel width of the text when rendered with the given font.
   *
   * @param font The font to measure with.
   * @param text The text to measure.
   * @return The rendered text width in pixels.
   */
  EXPORT int CALL TextWidth(Font *font, const char *text);

  /**
   * Returns the pixel height of the text when rendered with the given font.
   *
   * @param font The font to measure with.
   * @param text The text to measure.
   * @return The rendered text height in pixels.
   */
  EXPORT int CALL TextHeight(Font *font, const char *text);

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifndef SWIG
void _UnloadDefaultFont();
#endif
