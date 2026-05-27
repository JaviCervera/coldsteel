/** @file
 * Functions for loading and drawing bitmap fonts.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Font;

  /**
   * Loads a bitmap font from an Irrlicht XML font file (.xml).
   * Use the fonttool utility to generate .xml/.png font files from TTF sources.
   *
   * @param filename The path to the Irrlicht XML font file.
   * @return The loaded font, or NULL on failure.
   */
  EXPORT Font *CALL LoadFont(const char *filename);

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
