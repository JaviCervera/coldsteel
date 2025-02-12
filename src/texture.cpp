#include "core.h"
#include "graphics.h"
#include "texture.h"

ITexture *CreateTexture(int width, int height)
{
  const ECOLOR_FORMAT format =
      _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
          ? ECF_A8R8G8B8
          : ECF_A1R5G5B5;
  return _Device()->getVideoDriver()->addTexture(dimension2du(width, height), "", format);
}

ITexture *CreateRenderTexture(int width, int height)
{
  const ECOLOR_FORMAT format =
      _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
          ? ECF_A8R8G8B8
          : ECF_A1R5G5B5;
  return _Device()->getVideoDriver()->addRenderTargetTexture(dimension2du(width, height), "", format);
}

ITexture *LoadTexture(const char *filename)
{
  return _Device()->getVideoDriver()->getTexture(filename);
}

void ClearTextureCache()
{
  _Device()->getVideoDriver()->removeAllTextures();
}

void DrawTexture(ITexture *tex, int x, int y)
{
  _Device()->getVideoDriver()->draw2DImage(tex, vector2di(x, y));
  DrawTextureEx(tex, x, y, tex->getOriginalSize().Width, tex->getOriginalSize().Height, RGB(255, 255, 255));
}

void DrawTextureEx(ITexture *tex, int x, int y, int width, int height, int color)
{
  SColor col = _Color(color);
  SColor colors[] = {col, col, col, col};
  _Device()->getVideoDriver()->draw2DImage(tex, recti(x, y, x + width, y + height), recti(0, 0, tex->getSize().Width, tex->getSize().Height), NULL, colors, true);
}

void *LockTexture(ITexture *tex)
{
  return tex->lock();
}

void UnlockTexture(ITexture *tex)
{
  tex->unlock();
  if (tex->hasMipMaps())
    tex->regenerateMipMapLevels();
}

void SetTextureColorKey(ITexture *tex, int color)
{
  _Device()->getVideoDriver()->makeColorKeyTexture(tex, _Color(color));
}

void NormalizeTexture(ITexture *tex, float amplitude)
{
  _Device()->getVideoDriver()->makeNormalMapTexture(tex, amplitude);
}

int TextureWidth(ITexture *tex)
{
  return tex->getOriginalSize().Width;
}

int TextureHeight(ITexture *tex)
{
  return tex->getOriginalSize().Height;
}

const char *TextureFilename(ITexture *tex)
{
  return stringc(tex->getName()).c_str();
}

void SetTextureCreationFlags(int flags)
{
  _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_16_BIT, (flags & TEXTURE_16BITS) == TEXTURE_16BITS);
  _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, (flags & TEXTURE_16BITS) == 0);
  _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, (flags & TEXTURE_MIPMAPS) == TEXTURE_MIPMAPS);
}
