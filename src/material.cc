#include "graphics.h"
#include "material.h"
#include "math.h"

extern "C" {


E_MATERIAL_TYPE _csIrrlichtMaterialType(int blend);
int _csMaterialType(E_MATERIAL_TYPE type);


EXPORT void CALL csSetMaterialTexture(SMaterial* material, int index, ITexture* texture) {
    material->setTexture(index, texture);
}


EXPORT ITexture* CALL csMaterialTexture(SMaterial* material, int index) {
    return material->getTexture(index);
}


EXPORT void CALL csSetMaterialTextureTransform(SMaterial* material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale) {
    matrix4 mat = matrix4().buildTextureTransform(csRad(angle), vector2df(0, 0), vector2df(xoffset, yoffset), vector2df(xscale, yscale));
    material->setTextureMatrix(index, mat);
}


EXPORT void CALL csSetMaterialType(SMaterial* material, int type) {
    material->MaterialType = _csIrrlichtMaterialType(type);
}


EXPORT int CALL csMaterialType(SMaterial* material) {
    return _csMaterialType(material->MaterialType);
}


EXPORT void CALL csSetMaterialDiffuse(SMaterial* material, int color) {
    material->DiffuseColor = _csColor(color);
}


EXPORT int CALL csMaterialDiffuse(SMaterial* material) {
    return _csIntColor(material->DiffuseColor);
}


EXPORT void CALL csSetMaterialEmissive(SMaterial* material, int color) {
    material->EmissiveColor = _csColor(color);
}


EXPORT int CALL csMaterialEmissive(SMaterial* material) {
    return _csIntColor(material->EmissiveColor);
}


EXPORT void CALL csSetMaterialAmbient(SMaterial* material, int color) {
    material->AmbientColor = _csColor(color);
}


EXPORT int CALL csMaterialAmbient(SMaterial* material) {
    return _csIntColor(material->AmbientColor);
}


EXPORT void CALL csSetMaterialSpecular(SMaterial* material, int color) {
    material->SpecularColor = _csColor(color);
}


EXPORT int CALL csMaterialSpecular(SMaterial* material) {
    return _csIntColor(material->SpecularColor);
}


EXPORT void CALL csSetMaterialShininess(SMaterial* material, float shininess) {
    material->Shininess = shininess * 128;
}


EXPORT float CALL csMaterialShininess(SMaterial* material) {
    return material->Shininess / 128;
}


EXPORT void CALL csSetMaterialRenderMode(SMaterial* material, int mode) {
    material->setFlag(EMF_WIREFRAME, mode == CSRENDER_WIREFRAME);
    material->setFlag(EMF_POINTCLOUD, mode == CSRENDER_POINTCLOUD);
}


EXPORT int CALL csMaterialRenderMode(SMaterial* material) {
    if (material->getFlag(EMF_POINTCLOUD)) return CSRENDER_POINTCLOUD;
    else if (material->getFlag(EMF_WIREFRAME)) return CSRENDER_WIREFRAME;
    else return CSRENDER_NORMAL;
}


EXPORT void CALL csSetMaterialFilterMode(SMaterial* material, int mode) {
    material->setFlag(EMF_ANISOTROPIC_FILTER, mode == CSFILTER_ANISOTROPIC);
    material->setFlag(EMF_TRILINEAR_FILTER, mode >= CSFILTER_TRILINEAR);
    material->setFlag(EMF_BILINEAR_FILTER, mode >= CSFILTER_BILINEAR);
}


EXPORT int CALL csMaterialFilterMode(SMaterial* material) {
    if (material->getFlag(EMF_ANISOTROPIC_FILTER)) return CSFILTER_ANISOTROPIC;
    if (material->getFlag(EMF_TRILINEAR_FILTER)) return CSFILTER_TRILINEAR;
    if (material->getFlag(EMF_BILINEAR_FILTER)) return CSFILTER_BILINEAR;
    else return CSFILTER_DISABLED;
}


EXPORT void CALL csSetMaterialLightingMode(SMaterial* material, int mode) {
    material->setFlag(EMF_LIGHTING, mode != CSLIGHTING_DISABLED);
    material->setFlag(EMF_GOURAUD_SHADING, mode == CSLIGHTING_GOURAUD);
}

EXPORT int CALL csMaterialLightingMode(SMaterial* material) {
    if (!material->getFlag(EMF_LIGHTING)) return CSLIGHTING_DISABLED;
    else if (material->getFlag(EMF_GOURAUD_SHADING)) return CSLIGHTING_GOURAUD;
    else return CSLIGHTING_FLAT;
}


EXPORT void CALL csSetMaterialFogEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_FOG_ENABLE, enabled);
}


EXPORT bool_t CALL csMaterialFogEnabled(SMaterial* material) {
    return material->getFlag(EMF_FOG_ENABLE);
}


EXPORT void CALL csSetMaterialDepthReadingEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_ZBUFFER, enabled);
}


EXPORT bool_t CALL csMaterialDepthReadingEnabled(SMaterial* material) {
    return material->getFlag(EMF_ZBUFFER);
}


EXPORT void CALL csSetMaterialDepthWritingEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_ZWRITE_ENABLE, enabled);
}


EXPORT bool_t CALL csMaterialDepthWritingEnabled(SMaterial* material) {
    return material->getFlag(EMF_ZWRITE_ENABLE);
}


EXPORT void CALL csSetMaterialCullingEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_BACK_FACE_CULLING, enabled);
}


EXPORT bool_t CALL csMaterialCullingEnabled(SMaterial* material) {
    return material->getFlag(EMF_BACK_FACE_CULLING);
}


EXPORT void CALL csSetMaterialVertexColorsEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_COLOR_MATERIAL, enabled);
}


EXPORT bool_t CALL csMaterialVertexColorsEnabled(SMaterial* material) {
    return material->getFlag(EMF_COLOR_MATERIAL);
}


E_MATERIAL_TYPE _csIrrlichtMaterialType(int blend) {
    map<int, E_MATERIAL_TYPE> types;
    types.set(CSMATERIAL_SOLID, EMT_SOLID);
    types.set(CSMATERIAL_LIGHTMAP, EMT_LIGHTMAP_LIGHTING);
    types.set(CSMATERIAL_LIGHTMAP2X, EMT_LIGHTMAP_LIGHTING_M2);
    types.set(CSMATERIAL_LIGHTMAP4X, EMT_LIGHTMAP_LIGHTING_M4);
    types.set(CSMATERIAL_DETAIL, EMT_DETAIL_MAP);
    types.set(CSMATERIAL_ADD, EMT_TRANSPARENT_ADD_COLOR);
    types.set(CSMATERIAL_ALPHA, EMT_TRANSPARENT_ALPHA_CHANNEL);
    types.set(CSMATERIAL_VERTEXALPHA, EMT_TRANSPARENT_VERTEX_ALPHA);
    types.set(CSMATERIAL_MASKED, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    types.set(CSMATERIAL_SPHERE, EMT_SPHERE_MAP);
    types.set(CSMATERIAL_REFLECTION, EMT_TRANSPARENT_REFLECTION_2_LAYER);
    types.set(CSMATERIAL_NORMAL, EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(CSMATERIAL_NORMALADD, EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR);
    types.set(CSMATERIAL_PARALLAX, EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(CSMATERIAL_PARALLAXADD, EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR);
    return types[blend];
}


int _csMaterialType(E_MATERIAL_TYPE type) {
    map<E_MATERIAL_TYPE, int> types;
    types.set(EMT_SOLID, CSMATERIAL_SOLID);
    types.set(EMT_LIGHTMAP_LIGHTING, CSMATERIAL_LIGHTMAP);
    types.set(EMT_LIGHTMAP_LIGHTING_M2, CSMATERIAL_LIGHTMAP2X);
    types.set(EMT_LIGHTMAP_LIGHTING_M4, CSMATERIAL_LIGHTMAP4X);
    types.set(EMT_DETAIL_MAP, CSMATERIAL_DETAIL);
    types.set(EMT_TRANSPARENT_ADD_COLOR, CSMATERIAL_ADD);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL, CSMATERIAL_ALPHA);
    types.set(EMT_TRANSPARENT_VERTEX_ALPHA, CSMATERIAL_VERTEXALPHA);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL_REF, CSMATERIAL_MASKED);
    types.set(EMT_SPHERE_MAP, CSMATERIAL_SPHERE);
    types.set(EMT_TRANSPARENT_REFLECTION_2_LAYER, CSMATERIAL_REFLECTION);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA, CSMATERIAL_NORMAL);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR, CSMATERIAL_NORMALADD);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA, CSMATERIAL_PARALLAX);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR, CSMATERIAL_PARALLAXADD);
    return types[type];
}


} // extern "C"
