#include "core.h"
#include "font.h"
#include "graphics.h"

extern "C" {


EXPORT IGUIFont* CALL csLoadFont(const char* filename) {
    return _csDevice()->getGUIEnvironment()->getFont(filename);
}


EXPORT void CALL csFreeFont(IGUIFont* font) {
    _csDevice()->getGUIEnvironment()->removeFont(font);
}


EXPORT void CALL csDrawText(IGUIFont* font, const char* text, int x, int y, int color) {
    if (!font) font = _csDevice()->getGUIEnvironment()->getBuiltInFont();
    const wchar_t* wtext = _WSTR(text);
    recti rect(vector2di(x, y), font->getDimension(wtext));
    font->draw(wtext, rect, _csColor(color));
}


EXPORT int CALL csTextWidth(IGUIFont* font, const char* text) {
    if (!font) font = _csDevice()->getGUIEnvironment()->getBuiltInFont();
    return font->getDimension(_WSTR(text)).Width;
}


EXPORT int CALL csTextHeight(IGUIFont* font, const char* text) {
    if (!font) font = _csDevice()->getGUIEnvironment()->getBuiltInFont();
    return font->getDimension(_WSTR(text)).Height;
}


} // extern "C"
