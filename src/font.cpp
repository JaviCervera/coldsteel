#include "core.h"
#include "font.h"
#include "graphics.h"

IGUIFont *LoadFont(const char *filename)
{
  return _Device()->getGUIEnvironment()->getFont(filename);
}

void FreeFont(IGUIFont *font)
{
  _Device()->getGUIEnvironment()->removeFont(font);
}

void DrawText(IGUIFont *font, const char *text, int x, int y, int color)
{
  if (!font)
    font = _Device()->getGUIEnvironment()->getBuiltInFont();
  const wchar_t *wtext = _WSTR(text);
  recti rect(vector2di(x, y), font->getDimension(wtext));
  font->draw(wtext, rect, _Color(color));
}

int TextWidth(IGUIFont *font, const char *text)
{
  if (!font)
    font = _Device()->getGUIEnvironment()->getBuiltInFont();
  return font->getDimension(_WSTR(text)).Width;
}

int TextHeight(IGUIFont *font, const char *text)
{
  if (!font)
    font = _Device()->getGUIEnvironment()->getBuiltInFont();
  return font->getDimension(_WSTR(text)).Height;
}
