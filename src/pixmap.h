/**
 * @file
 * A Pixmap is a CPU-side image used for pixel manipulation. Pixmaps can be loaded from files or
 * created programmatically and then converted to textures.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a new blank pixmap.
   *
   * @param width The width in pixels.
   * @param height The height in pixels.
   * @return A new pixmap.
   */
  EXPORT IImage *CALL CreatePixmap(int width, int height);

  /**
   * Creates a pixmap from the contents of a texture.
   *
   * @param texture The texture to copy.
   * @return A new pixmap containing the texture data.
   */
  EXPORT IImage *CALL CreatePixmapFromTexture(ITexture *texture);

  /**
   * Creates a pixmap from the current screen contents.
   *
   * @return A new pixmap containing the current screen contents.
   */
  EXPORT IImage *CALL CreatePixmapFromScreen();

  /**
   * Loads a pixmap from an image file.
   *
   * @param filename The image file to load.
   * @return The loaded pixmap.
   */
  EXPORT IImage *CALL LoadPixmap(const char *filename);

  /**
   * Frees a pixmap.
   *
   * @param pixmap The pixmap to free.
   */
  EXPORT void CALL FreePixmap(IImage *pixmap);

  /**
   * Saves a pixmap to an image file.
   *
   * @param pixmap The pixmap to save.
   * @param filename The destination filename.
   */
  EXPORT void CALL SavePixmap(IImage *pixmap, const char *filename);

  /**
   * Returns the color of a pixel.
   *
   * @param pixmap The pixmap to read from.
   * @param x The X coordinate in pixels.
   * @param y The Y coordinate in pixels.
   * @return The color of the pixel at the given coordinates.
   */
  EXPORT int CALL ReadPixel(IImage *pixmap, int x, int y);

  /**
   * Sets the color of a pixel.
   *
   * @param pixmap The pixmap to modify.
   * @param x The X coordinate in pixels.
   * @param y The Y coordinate in pixels.
   * @param color The color to write.
   */
  EXPORT void CALL WritePixel(IImage *pixmap, int x, int y, int color);

  /**
   * Returns the width of a pixmap in pixels.
   *
   * @param pixmap The pixmap to query.
   * @return The pixmap width in pixels.
   */
  EXPORT int CALL PixmapWidth(IImage *pixmap);

  /**
   * Returns the height of a pixmap in pixels.
   *
   * @param pixmap The pixmap to query.
   * @return The pixmap height in pixels.
   */
  EXPORT int CALL PixmapHeight(IImage *pixmap);

#ifdef __cplusplus
} /* extern "C" */
#endif
