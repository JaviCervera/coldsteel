#include "color.h"
#include "core.h"
#include "font.h"

static Font *_AsFont(IGUIFont *gui_font)
{
  return reinterpret_cast<Font *>(gui_font);
}

static IGUIFont *_AsIrrFont(Font *font)
{
  return font ? reinterpret_cast<IGUIFont *>(font) : _Device()->getGUIEnvironment()->getBuiltInFont();
}

extern "C"
{

  EXPORT Font *CALL LoadFont(const char *filename)
  {
    return _AsFont(_Device()->getGUIEnvironment()->getFont(_WSTR(filename)));
  }

  EXPORT void CALL FreeFont(Font *font)
  {
    if (font)
    {
      _AsIrrFont(font)->drop();
    }
  }

  EXPORT void CALL DrawText(Font *font, const char *text, int x, int y, int color)
  {
    IGUIFont *f = _AsIrrFont(font);
    dimension2du dim = f->getDimension(_WSTR(text));
    f->draw(_WSTR(text), rect<s32>(x, y, x + (s32)dim.Width, y + (s32)dim.Height), _Color(color), false, false, NULL);
  }

  EXPORT int CALL TextWidth(Font *font, const char *text)
  {
    return (int)_AsIrrFont(font)->getDimension(_WSTR(text)).Width;
  }

  EXPORT int CALL TextHeight(Font *font, const char *text)
  {
    return (int)_AsIrrFont(font)->getDimension(_WSTR(text)).Height;
  }

} // extern "C"
