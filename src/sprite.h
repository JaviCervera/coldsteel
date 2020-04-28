#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IBillboardSceneNode* CALL asCreateSprite(ITexture* texture, int blend);
EXPORT void CALL asSetSpriteSize(IBillboardSceneNode* sprite, float width, float height);
EXPORT float CALL asSpriteWidth(IBillboardSceneNode* sprite);
EXPORT float CALL asSpriteHeight(IBillboardSceneNode* sprite);


#ifdef __cplusplus
} /* extern "C" */
#endif
