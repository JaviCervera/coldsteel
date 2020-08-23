#include "core.h"
#include "graphics.h"

extern "C" {


EXPORT int CALL csRGB(int red, int green, int blue) {
    return csRGBA(red, green, blue, 255);
}


EXPORT int CALL csRGBA(int red, int green, int blue, int alpha) {
    return (alpha << 24) + (red << 16) + (green << 8) + blue;
}


EXPORT int CALL csRedVal(int color) {
    return (color & 0x00FF0000) >> 16;
}


EXPORT int CALL csGreenVal(int color) {
    return (color & 0x0000FF00) >> 8;
}


EXPORT int CALL csBlueVal(int color) {
    return (color & 0x000000FF);
}


EXPORT int CALL csAlphaVal(int color) {
    return (color & 0xFF000000) >> 24;
}


EXPORT void CALL csBeginDrawing(bool_t clear, bool_t clearDepth, int color) {
    _csDevice()->getVideoDriver()->beginScene(clear, clearDepth, _csColor(color));
}


EXPORT void CALL csEndDrawing() {
    _csDevice()->getVideoDriver()->endScene();
}


EXPORT void CALL csSetRenderTarget(ITexture* tex, bool_t clear, bool_t clearDepth, int color) {
    _csDevice()->getVideoDriver()->setRenderTarget(tex, clear, clearDepth, _csColor(color));
}


EXPORT void CALL csSetViewport(int x, int y, int width, int height) {
    _csDevice()->getVideoDriver()->setViewPort(recti(x, y, width + x, height + y));
}


EXPORT void CALL csDrawLine(int x1, int y1, int x2, int y2, int color) {
    _csDevice()->getVideoDriver()->draw2DLine(vector2di(x1,y1), vector2di(x2,y2), _csColor(color));
}


EXPORT void CALL csDrawRect(int x, int y, int width, int height, int color) {
    _csDevice()->getVideoDriver()->draw2DRectangle(_csColor(color), recti(x, y, x+width, y+height));
}


EXPORT void CALL csDraw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color) {
    _csDevice()->getVideoDriver()->draw3DLine(vector3df(x1, y1, z1), vector3df(x2, y2, z2), _csColor(color));
}


SColor _csColor(int color) {
    return SColor(csAlphaVal(color), csRedVal(color), csGreenVal(color), csBlueVal(color));
}


int _csIntColor(const SColor& color) {
    return csRGBA(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}


} // extern "C"
