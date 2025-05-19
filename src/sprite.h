/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT IBillboardSceneNode *CALL CreateSprite(ITexture *texture, int materialType);
  EXPORT void CALL SetSpriteSize(IBillboardSceneNode *sprite, float width, float height);
  EXPORT float CALL SpriteWidth(IBillboardSceneNode *sprite);
  EXPORT float CALL SpriteHeight(IBillboardSceneNode *sprite);

#ifdef __cplusplus
} /* extern "C" */
#endif
