/**
 * @file
 * A Sprite is a billboard entity that always faces the camera, typically used for particles,
 * icons, or effects.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a sprite using the given texture and material type.
   *
   * @param texture The texture to apply to the sprite.
   * @param materialType The material type to use (MATERIAL_*).
   * @return A new sprite entity.
   */
  EXPORT IBillboardSceneNode *CALL CreateSprite(ITexture *texture, int materialType);

  /**
   * Sets the sprite dimensions in world units.
   *
   * @param sprite The sprite to resize.
   * @param width The new sprite width in world units.
   * @param height The new sprite height in world units.
   */
  EXPORT void CALL SetSpriteSize(IBillboardSceneNode *sprite, float width, float height);

  /**
   * Returns the sprite width.
   *
   * @param sprite The sprite to query.
   * @return The sprite width in world units.
   */
  EXPORT float CALL SpriteWidth(IBillboardSceneNode *sprite);

  /**
   * Returns the sprite height.
   *
   * @param sprite The sprite to query.
   * @return The sprite height in world units.
   */
  EXPORT float CALL SpriteHeight(IBillboardSceneNode *sprite);

#ifdef __cplusplus
} /* extern "C" */
#endif
