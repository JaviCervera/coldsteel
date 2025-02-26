#include "core.h"
#include "material.h"
#include "sprite.h"

extern "C"
{

  EXPORT IBillboardSceneNode *CALL CreateSprite(ITexture *texture, int materialType)
  {
    IBillboardSceneNode *spr = _Device()->getSceneManager()->addBillboardSceneNode();
    SetMaterialTexture(&spr->getMaterial(0), 0, texture);
    SetMaterialType(&spr->getMaterial(0), materialType);
    if (texture)
      SetSpriteSize(spr, texture->getOriginalSize().Width, texture->getOriginalSize().Height);
    else
      SetSpriteSize(spr, 1, 1);
    return spr;
  }

  EXPORT void CALL SetSpriteSize(IBillboardSceneNode *sprite, float width, float height)
  {
    sprite->setSize(dimension2df(width, height));
  }

  EXPORT float CALL SpriteWidth(IBillboardSceneNode *sprite)
  {
    return sprite->getSize().Width;
  }

  EXPORT float CALL SpriteHeight(IBillboardSceneNode *sprite)
  {
    return sprite->getSize().Height;
  }

} // extern "C"
