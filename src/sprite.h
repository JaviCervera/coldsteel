#pragma once

#include "common.h"

IBillboardSceneNode *CreateSprite(ITexture *texture, int materialType);
void SetSpriteSize(IBillboardSceneNode *sprite, float width, float height);
float SpriteWidth(IBillboardSceneNode *sprite);
float SpriteHeight(IBillboardSceneNode *sprite);
