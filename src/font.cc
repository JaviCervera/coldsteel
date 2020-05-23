#include "core.h"
#include "font.h"
#include "graphics.h"

extern "C" {


EXPORT IGUIFont* CALL asLoadFont(const char* filename) {
    return _asDevice()->getGUIEnvironment()->getFont(filename);
}


EXPORT void CALL asFreeFont(IGUIFont* font) {
    _asDevice()->getGUIEnvironment()->removeFont(font);
}


EXPORT void CALL asDrawText(IGUIFont* font, const char* text, int x, int y, int color) {
    if (!font) font = _asDevice()->getGUIEnvironment()->getBuiltInFont();
    const wchar_t* wtext = _WSTR(text);
    recti rect(vector2di(x, y), font->getDimension(wtext));
    font->draw(wtext, rect, _asColor(color));
}


EXPORT int CALL asTextWidth(IGUIFont* font, const char* text) {
    if (!font) font = _asDevice()->getGUIEnvironment()->getBuiltInFont();
    return font->getDimension(_WSTR(text)).Width;
}


EXPORT int CALL asTextHeight(IGUIFont* font, const char* text) {
    if (!font) font = _asDevice()->getGUIEnvironment()->getBuiltInFont();
    return font->getDimension(_WSTR(text)).Width;
}


} // extern "C"
