#include "core.h"
#include "graphics.h"
#include "texture.h"

extern "C"
{

  EXPORT ITexture *CALL CreateTexture(int width, int height)
  {
    const ECOLOR_FORMAT format = _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
                                     ? ECF_A8R8G8B8
                                     : ECF_A1R5G5B5;
    return _Device()->getVideoDriver()->addTexture(dimension2du(width, height), "", format);
  }

  EXPORT ITexture *CALL CreateRenderTexture(int width, int height)
  {
    const ECOLOR_FORMAT format = _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
                                     ? ECF_A8R8G8B8
                                     : ECF_A1R5G5B5;
    return _Device()->getVideoDriver()->addRenderTargetTexture(dimension2du(width, height), "", format);
  }

  EXPORT ITexture *CALL LoadTexture(const char *filename)
  {
    return _Device()->getVideoDriver()->getTexture(filename);
  }

  EXPORT void CALL ClearTextureCache()
  {
    _Device()->getVideoDriver()->removeAllTextures();
  }

  EXPORT void CALL DrawTexture(ITexture *tex, int x, int y)
  {
    _Device()->getVideoDriver()->draw2DImage(tex, vector2di(x, y));
    DrawTextureEx(tex, x, y, tex->getOriginalSize().Width, tex->getOriginalSize().Height, RGB(255, 255, 255));
  }

  EXPORT void CALL DrawTextureEx(ITexture *tex, int x, int y, int width, int height, int color)
  {
    SColor col = _Color(color);
    SColor colors[] = {col, col, col, col};
    _Device()->getVideoDriver()->draw2DImage(tex, recti(x, y, x + width, y + height), recti(0, 0, tex->getSize().Width, tex->getSize().Height), NULL, colors, true);
  }

  EXPORT void *CALL LockTexture(ITexture *tex)
  {
    return tex->lock();
  }

  EXPORT void CALL UnlockTexture(ITexture *tex)
  {
    tex->unlock();
    if (tex->hasMipMaps())
      tex->regenerateMipMapLevels();
  }

  EXPORT void CALL SetTextureColorKey(ITexture *tex, int color)
  {
    _Device()->getVideoDriver()->makeColorKeyTexture(tex, _Color(color));
  }

  EXPORT void CALL NormalizeTexture(ITexture *tex, float amplitude)
  {
    _Device()->getVideoDriver()->makeNormalMapTexture(tex, amplitude);
  }

  EXPORT int CALL TextureWidth(ITexture *tex)
  {
    return tex->getOriginalSize().Width;
  }

  EXPORT int CALL TextureHeight(ITexture *tex)
  {
    return tex->getOriginalSize().Height;
  }

  EXPORT const char *CALL TextureFilename(ITexture *tex)
  {
    return stringc(tex->getName()).c_str();
  }

  EXPORT void CALL SetTextureCreationFlags(int flags)
  {
    _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_16_BIT, (flags & TEXTURE_16BITS) == TEXTURE_16BITS);
    _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, (flags & TEXTURE_16BITS) == 0);
    _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, (flags & TEXTURE_MIPMAPS) == TEXTURE_MIPMAPS);
  }

} // extern "C"
