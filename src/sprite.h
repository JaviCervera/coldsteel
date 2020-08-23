#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IBillboardSceneNode* CALL csCreateSprite(ITexture* texture, int materialType);
EXPORT void CALL csSetSpriteSize(IBillboardSceneNode* sprite, float width, float height);
EXPORT float CALL csSpriteWidth(IBillboardSceneNode* sprite);
EXPORT float CALL csSpriteHeight(IBillboardSceneNode* sprite);


#ifdef __cplusplus
} /* extern "C" */
#endif
