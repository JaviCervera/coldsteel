#include "core.h"
#include "material.h"
#include "sprite.h"

extern "C" {


EXPORT IBillboardSceneNode* CALL asCreateSprite(ITexture* texture, int blend) {
    IBillboardSceneNode* spr = _asDevice()->getSceneManager()->addBillboardSceneNode();
    asSetMaterialTexture(&spr->getMaterial(0), 0, texture);
    asSetMaterialBlend(&spr->getMaterial(0), blend);
    if (texture) asSetSpriteSize(spr, texture->getOriginalSize().Width, texture->getOriginalSize().Height);
    else asSetSpriteSize(spr, 1, 1);
    return spr;
}


EXPORT void CALL asSetSpriteSize(IBillboardSceneNode* sprite, float width, float height) {
    sprite->setSize(dimension2df(width, height));
}


EXPORT float CALL asSpriteWidth(IBillboardSceneNode* sprite) {
    return sprite->getSize().Width;
}

EXPORT float CALL asSpriteHeight(IBillboardSceneNode* sprite) {
    return sprite->getSize().Height;
}


} // extern "C"
