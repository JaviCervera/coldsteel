#include "core.h"
#include "graphics.h"

extern "C" {


EXPORT int CALL asRGB(int red, int green, int blue) {
    return asRGBA(red, green, blue, 255);
}


EXPORT int CALL asRGBA(int red, int green, int blue, int alpha) {
    return (alpha << 24) + (red << 16) + (green << 8) + blue;
}


EXPORT int CALL asRedVal(int color) {
    return (color & 0x00FF0000) >> 16;
}


EXPORT int CALL asGreenVal(int color) {
    return (color & 0x0000FF00) >> 8;
}


EXPORT int CALL asBlueVal(int color) {
    return (color & 0x000000FF);
}


EXPORT int CALL asAlphaVal(int color) {
    return (color & 0xFF000000) >> 24;
}


EXPORT void CALL asBeginDrawing(bool_t clear, bool_t clearDepth, int color) {
    _asDevice()->getVideoDriver()->beginScene(clear, clearDepth, _asColor(color));
}


EXPORT void CALL asEndDrawing() {
    _asDevice()->getVideoDriver()->endScene();
}


EXPORT void CALL asSetRenderTarget(ITexture* tex, bool_t clear, bool_t clearDepth, int color) {
    _asDevice()->getVideoDriver()->setRenderTarget(tex, clear, clearDepth, _asColor(color));
}


EXPORT void CALL asSetViewport(int x, int y, int width, int height) {
    _asDevice()->getVideoDriver()->setViewPort(recti(x, y, width + x, height + y));
}


EXPORT void CALL asDrawLine(int x1, int y1, int x2, int y2, int color) {
    _asDevice()->getVideoDriver()->draw2DLine(vector2di(x1,y1), vector2di(x2,y2), _asColor(color));
}


EXPORT void CALL asDrawRect(int x, int y, int width, int height, int color) {
    _asDevice()->getVideoDriver()->draw2DRectangle(_asColor(color), recti(x, y, x+width, y+height));
}


EXPORT void CALL asDraw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color) {
    _asDevice()->getVideoDriver()->draw3DLine(vector3df(x1, y1, z1), vector3df(x2, y2, z2), _asColor(color));
}


SColor _asColor(int color) {
    return SColor(asAlphaVal(color), asRedVal(color), asGreenVal(color), asBlueVal(color));
}


int _asIntColor(const SColor& color) {
    return asRGBA(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}


} // extern "C"
