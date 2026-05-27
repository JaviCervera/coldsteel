/** @file
 * 2D and 3D immediate-mode drawing functions.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Sets the render target texture.
   *
   * @param tex The render target texture, or NULL to render to the screen.
   * @param clear True to clear the color buffer.
   * @param clearDepth True to clear the depth buffer.
   * @param color The clear color.
   */
  EXPORT void CALL SetRenderTarget(ITexture *tex, bool_t clear, bool_t clearDepth, int color);

  /**
   * Sets the viewport rectangle for subsequent rendering.
   *
   * @param x The viewport X position.
   * @param y The viewport Y position.
   * @param width The viewport width.
   * @param height The viewport height.
   */
  EXPORT void CALL SetViewport(int x, int y, int width, int height);

  /**
   * Draws a single pixel.
   *
   * @param x The screen X position.
   * @param y The screen Y position.
   * @param color The pixel color.
   */
  EXPORT void CALL DrawPoint(int x, int y, int color);

  /**
   * Draws a 2D line between two points.
   *
   * @param x1 The first point X coordinate.
   * @param y1 The first point Y coordinate.
   * @param x2 The second point X coordinate.
   * @param y2 The second point Y coordinate.
   * @param color The line color.
   */
  EXPORT void CALL DrawLine(int x1, int y1, int x2, int y2, int color);

  /**
   * Draws a filled 2D rectangle.
   *
   * @param x The rectangle X position.
   * @param y The rectangle Y position.
   * @param width The rectangle width.
   * @param height The rectangle height.
   * @param color The rectangle color.
   */
  EXPORT void CALL DrawRect(int x, int y, int width, int height, int color);

  /**
   * Draws a 3D line between two world-space points.
   *
   * @param x1 The first point X coordinate.
   * @param y1 The first point Y coordinate.
   * @param z1 The first point Z coordinate.
   * @param x2 The second point X coordinate.
   * @param y2 The second point Y coordinate.
   * @param z2 The second point Z coordinate.
   * @param color The line color.
   */
  EXPORT void CALL Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color);

#ifdef __cplusplus
} /* extern "C" */
#endif
