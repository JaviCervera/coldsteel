#include "color.h"
#include "core.h"
#include "font.h"

static Font *_AsFont(IGUIFont *gui_font)
{
  return reinterpret_cast<Font *>(gui_font);
}

static IGUIFont *_AsIrrFont(Font *font)
{
  if (font)
    return reinterpret_cast<IGUIFont *>(font);
  IGUIEnvironment *env = _Device()->getGUIEnvironment();
  return env ? env->getBuiltInFont() : NULL;
}

extern "C"
{

  EXPORT Font *CALL LoadFont(const char *filename)
  {
    IGUIEnvironment *env = _Device()->getGUIEnvironment();
    return env ? _AsFont(env->getFont(_WSTR(filename))) : NULL;
  }

  EXPORT void CALL FreeFont(Font *font)
  {
    if (font)
    {
      reinterpret_cast<IGUIFont *>(font)->drop();
    }
  }

  EXPORT void CALL DrawText(Font *font, const char *text, int x, int y, int color)
  {
    IGUIFont *f = _AsIrrFont(font);
    if (!f) return;
    dimension2du dim = f->getDimension(_WSTR(text));
    f->draw(_WSTR(text), rect<s32>(x, y, x + (s32)dim.Width, y + (s32)dim.Height), _Color(color), false, false, NULL);
  }

  EXPORT int CALL TextWidth(Font *font, const char *text)
  {
    IGUIFont *f = _AsIrrFont(font);
    if (!f) return 0;
    return (int)f->getDimension(_WSTR(text)).Width;
  }

  EXPORT int CALL TextHeight(Font *font, const char *text)
  {
    IGUIFont *f = _AsIrrFont(font);
    if (!f) return 0;
    return (int)f->getDimension(_WSTR(text)).Height;
  }

} // extern "C"
