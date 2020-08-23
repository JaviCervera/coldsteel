#include "core.h"
#include "material.h"
#include "sprite.h"

extern "C" {


EXPORT IBillboardSceneNode* CALL csCreateSprite(ITexture* texture, int materialType) {
    IBillboardSceneNode* spr = _csDevice()->getSceneManager()->addBillboardSceneNode();
    csSetMaterialTexture(&spr->getMaterial(0), 0, texture);
    csSetMaterialType(&spr->getMaterial(0), materialType);
    if (texture) csSetSpriteSize(spr, texture->getOriginalSize().Width, texture->getOriginalSize().Height);
    else csSetSpriteSize(spr, 1, 1);
    return spr;
}


EXPORT void CALL csSetSpriteSize(IBillboardSceneNode* sprite, float width, float height) {
    sprite->setSize(dimension2df(width, height));
}


EXPORT float CALL csSpriteWidth(IBillboardSceneNode* sprite) {
    return sprite->getSize().Width;
}

EXPORT float CALL csSpriteHeight(IBillboardSceneNode* sprite) {
    return sprite->getSize().Height;
}


} // extern "C"
