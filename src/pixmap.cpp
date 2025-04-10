#include "core.h"
#include "graphics.h"
#include "pixmap.h"

extern "C"
{

  EXPORT IImage *CALL CreatePixmap(int width, int height)
  {
    const ECOLOR_FORMAT format =
        _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
            ? ECF_A8R8G8B8
            : ECF_A1R5G5B5;
    return _Device()->getVideoDriver()->createImage(format, dimension2du(width, height));
  }

  EXPORT IImage *CALL CreatePixmapFromTexture(ITexture *texture)
  {
    return _Device()->getVideoDriver()->createImage(texture, vector2di(0, 0), texture->getSize());
  }

  EXPORT IImage *CALL CreatePixmapFromScreen()
  {
    const ECOLOR_FORMAT format =
        _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
            ? ECF_A8R8G8B8
            : ECF_A1R5G5B5;
    return _Device()->getVideoDriver()->createScreenShot(format);
  }

  EXPORT IImage *CALL LoadPixmap(const char *filename)
  {
    return _Device()->getVideoDriver()->createImageFromFile(filename);
  }

  EXPORT void CALL FreePixmap(IImage *pixmap)
  {
    if (pixmap)
      pixmap->drop();
  }

  EXPORT void CALL SavePixmap(IImage *pixmap, const char *filename)
  {
    if (pixmap && filename)
      _Device()->getVideoDriver()->writeImageToFile(pixmap, filename, 9);
  }

  EXPORT int CALL ReadPixel(IImage *pixmap, int x, int y)
  {
    return pixmap ? _IntColor(pixmap->getPixel(x, y)) : 0;
  }

  EXPORT void CALL WritePixel(IImage *pixmap, int x, int y, int color)
  {
    if (pixmap)
      pixmap->setPixel(x, y, _Color(color));
  }

  EXPORT int CALL PixmapWidth(IImage *pixmap)
  {
    return pixmap ? pixmap->getDimension().Width : 0;
  }

  EXPORT int CALL PixmapHeight(IImage *pixmap)
  {
    return pixmap ? pixmap->getDimension().Height : 0;
  }

} /* extern "C" */
