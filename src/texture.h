/**
 * @file
 * Textures are image data loaded onto the GPU for rendering. Use TEXTURE_* flags with
 * SetTextureCreationFlags.
 */
#pragma once

#include "common.h"

// Texture flags
#define TEXTURE_MIPMAPS 1
#define TEXTURE_16BITS 2

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a blank texture with the given dimensions.
   *
   * @param width The texture width in pixels.
   * @param height The texture height in pixels.
   * @return The new texture.
   */
  EXPORT ITexture *CALL CreateTexture(int width, int height);

  /**
   * Creates a texture from a pixmap.
   *
   * @param pixmap The pixmap to upload to the GPU.
   * @return The new texture.
   */
  EXPORT ITexture *CALL CreateTextureFromPixmap(IImage *pixmap);

  /**
   * Creates a render target texture.
   *
   * @param width The texture width in pixels.
   * @param height The texture height in pixels.
   * @return The new render target texture.
   */
  EXPORT ITexture *CALL CreateRenderTexture(int width, int height);

  /**
   * Loads a texture from an image file.
   *
   * @param filename The path to the texture file to load.
   * @return The loaded texture.
   */
  EXPORT ITexture *CALL LoadTexture(const char *filename);

  /**
   * Removes all cached textures from memory.
   */
  EXPORT void CALL ClearTextureCache();

  /**
   * Draws a texture at a 2D screen position at full size and full opacity.
   *
   * @param tex The texture to draw.
   * @param x The destination X position in screen pixels.
   * @param y The destination Y position in screen pixels.
   */
  EXPORT void CALL DrawTexture(ITexture *tex, int x, int y);

  /**
   * Draws a texture scaled to a destination size with a color tint.
   *
   * @param tex The texture to draw.
   * @param x The destination X position in screen pixels.
   * @param y The destination Y position in screen pixels.
   * @param width The destination width in pixels.
   * @param height The destination height in pixels.
   * @param color The color tint and alpha to apply.
   */
  EXPORT void CALL DrawTextureEx(ITexture *tex, int x, int y, int width, int height, int color);

  /**
   * Draws a sub-region of a texture to a destination rectangle with a color tint.
   *
   * @param tex The texture to draw.
   * @param src_x The source X position in texture pixels.
   * @param src_y The source Y position in texture pixels.
   * @param src_width The source width in texture pixels.
   * @param src_height The source height in texture pixels.
   * @param dst_x The destination X position in screen pixels.
   * @param dst_y The destination Y position in screen pixels.
   * @param dst_width The destination width in screen pixels.
   * @param dst_height The destination height in screen pixels.
   * @param color The color tint and alpha to apply.
   */
  EXPORT void CALL DrawTextureEx2(ITexture *tex, int src_x, int src_y, int src_width, int src_height, int dst_x, int dst_y, int dst_width, int dst_height, int color);

  /**
   * Locks a texture for direct pixel access.
   *
   * @param tex The texture to lock.
   * @return A raw pointer to the texture pixel data.
   */
  EXPORT void *CALL LockTexture(ITexture *tex);

  /**
   * Unlocks a texture after direct pixel access.
   *
   * @param tex The texture to unlock.
   */
  EXPORT void CALL UnlockTexture(ITexture *tex);

  /**
   * Sets a color key for texture transparency.
   *
   * @param tex The texture to update.
   * @param color The color whose matching pixels should become transparent.
   */
  EXPORT void CALL SetTextureColorKey(ITexture *tex, int color);

  /**
   * Normalizes a height-map texture into a normal map.
   *
   * @param tex The texture to normalize.
   * @param amplitude The normal-map amplitude.
   */
  EXPORT void CALL NormalizeTexture(ITexture *tex, float amplitude);

  /**
   * Returns the texture width in pixels.
   *
   * @param tex The texture to query.
   * @return The texture width in pixels.
   */
  EXPORT int CALL TextureWidth(ITexture *tex);

  /**
   * Returns the texture height in pixels.
   *
   * @param tex The texture to query.
   * @return The texture height in pixels.
   */
  EXPORT int CALL TextureHeight(ITexture *tex);

  /**
   * Returns the filename a texture was loaded from.
   *
   * @param tex The texture to query.
   * @return The filename the texture was loaded from.
   */
  EXPORT const char *CALL TextureFilename(ITexture *tex);

  /**
   * Sets global texture creation flags for subsequent LoadTexture calls.
   *
   * @param flags The texture creation flags to apply (TEXTURE_*).
   */
  EXPORT void CALL SetTextureCreationFlags(int flags);

#ifdef __cplusplus
} /* extern "C" */
#endif
