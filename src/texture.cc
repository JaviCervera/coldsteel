#include "core.h"
#include "graphics.h"
#include "texture.h"

extern "C" {


EXPORT ITexture* CALL asCreateTexture(int width, int height) {
    const ECOLOR_FORMAT format = _asDevice()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
        ? ECF_A8R8G8B8
        : ECF_A1R5G5B5;
    return _asDevice()->getVideoDriver()->addTexture(dimension2du(width, height), "", format);
}


EXPORT ITexture* CALL asCreateRenderTexture(int width, int height) {
    const ECOLOR_FORMAT format = _asDevice()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
        ? ECF_A8R8G8B8
        : ECF_A1R5G5B5;
    return _asDevice()->getVideoDriver()->addRenderTargetTexture(dimension2du(width, height), "", format);
}


EXPORT ITexture* CALL asLoadTexture(const char* filename) {
    return _asDevice()->getVideoDriver()->getTexture(filename);
}


EXPORT void CALL asFreeTexture(ITexture* tex) {
    _asDevice()->getVideoDriver()->removeTexture(tex);
}


EXPORT void CALL asDrawTexture(ITexture* tex, int x, int y) {
    _asDevice()->getVideoDriver()->draw2DImage(tex, vector2di(x, y));
    asDrawTextureEx(tex, x, y, tex->getOriginalSize().Width, tex->getOriginalSize().Height, asRGB(255, 255, 255));
}


EXPORT void CALL asDrawTextureEx(ITexture* tex, int x, int y, int width, int height, int color) {
    SColor col = _asColor(color);
    SColor colors[] = {col, col, col, col};
    _asDevice()->getVideoDriver()->draw2DImage(tex, recti(x, y, x+width, y+height), recti(0, 0, tex->getSize().Width, tex->getSize().Height), NULL, colors, true);
}


EXPORT void* CALL asLockTexture(ITexture* tex) {
    return tex->lock();
}

EXPORT void CALL asUnlockTexture(ITexture* tex) {
    tex->unlock();
    if (tex->hasMipMaps()) tex->regenerateMipMapLevels();
}


EXPORT void CALL asSetTextureColorKey(ITexture* tex, int color) {
    _asDevice()->getVideoDriver()->makeColorKeyTexture(tex, _asColor(color));
}


EXPORT void CALL asNormalizeTexture(ITexture* tex, float amplitude) {
    _asDevice()->getVideoDriver()->makeNormalMapTexture(tex, amplitude);
}


EXPORT int CALL asTextureWidth(ITexture* tex) {
    return tex->getOriginalSize().Width;
}


EXPORT int CALL asTextureHeight(ITexture* tex) {
    return tex->getOriginalSize().Height;
}


EXPORT const char* CALL asTextureFilename(ITexture* tex) {
    return stringc(tex->getName()).c_str(); 
}


EXPORT void CALL asSetTextureCreationFlags(int flags) {
    _asDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_16_BIT, (flags & ASTEXTURE_16BITS) == ASTEXTURE_16BITS);
    _asDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, (flags & ASTEXTURE_16BITS) == 0);
    _asDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, (flags & ASTEXTURE_MIPMAPS) == ASTEXTURE_MIPMAPS);
}


} // extern "C"
