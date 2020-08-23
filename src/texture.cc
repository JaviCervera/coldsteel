#include "core.h"
#include "graphics.h"
#include "texture.h"

extern "C" {


EXPORT ITexture* CALL csCreateTexture(int width, int height) {
    const ECOLOR_FORMAT format = _csDevice()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
        ? ECF_A8R8G8B8
        : ECF_A1R5G5B5;
    return _csDevice()->getVideoDriver()->addTexture(dimension2du(width, height), "", format);
}


EXPORT ITexture* CALL csCreateRenderTexture(int width, int height) {
    const ECOLOR_FORMAT format = _csDevice()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
        ? ECF_A8R8G8B8
        : ECF_A1R5G5B5;
    return _csDevice()->getVideoDriver()->addRenderTargetTexture(dimension2du(width, height), "", format);
}


EXPORT ITexture* CALL csLoadTexture(const char* filename) {
    return _csDevice()->getVideoDriver()->getTexture(filename);
}


EXPORT void CALL csFreeTexture(ITexture* tex) {
    _csDevice()->getVideoDriver()->removeTexture(tex);
}


EXPORT void CALL csDrawTexture(ITexture* tex, int x, int y) {
    _csDevice()->getVideoDriver()->draw2DImage(tex, vector2di(x, y));
    csDrawTextureEx(tex, x, y, tex->getOriginalSize().Width, tex->getOriginalSize().Height, csRGB(255, 255, 255));
}


EXPORT void CALL csDrawTextureEx(ITexture* tex, int x, int y, int width, int height, int color) {
    SColor col = _csColor(color);
    SColor colors[] = {col, col, col, col};
    _csDevice()->getVideoDriver()->draw2DImage(tex, recti(x, y, x+width, y+height), recti(0, 0, tex->getSize().Width, tex->getSize().Height), NULL, colors, true);
}


EXPORT void* CALL csLockTexture(ITexture* tex) {
    return tex->lock();
}

EXPORT void CALL csUnlockTexture(ITexture* tex) {
    tex->unlock();
    if (tex->hasMipMaps()) tex->regenerateMipMapLevels();
}


EXPORT void CALL csSetTextureColorKey(ITexture* tex, int color) {
    _csDevice()->getVideoDriver()->makeColorKeyTexture(tex, _csColor(color));
}


EXPORT void CALL csNormalizeTexture(ITexture* tex, float amplitude) {
    _csDevice()->getVideoDriver()->makeNormalMapTexture(tex, amplitude);
}


EXPORT int CALL csTextureWidth(ITexture* tex) {
    return tex->getOriginalSize().Width;
}


EXPORT int CALL csTextureHeight(ITexture* tex) {
    return tex->getOriginalSize().Height;
}


EXPORT const char* CALL csTextureFilename(ITexture* tex) {
    return stringc(tex->getName()).c_str(); 
}


EXPORT void CALL csSetTextureCreationFlags(int flags) {
    _csDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_16_BIT, (flags & CSTEXTURE_16BITS) == CSTEXTURE_16BITS);
    _csDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, (flags & CSTEXTURE_16BITS) == 0);
    _csDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, (flags & CSTEXTURE_MIPMAPS) == CSTEXTURE_MIPMAPS);
}


} // extern "C"
