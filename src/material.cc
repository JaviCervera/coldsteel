#include "graphics.h"
#include "material.h"

extern "C" {


E_MATERIAL_TYPE _asIrrlichtMaterialType(int blend);
int _asBlendType(E_MATERIAL_TYPE type);


EXPORT void CALL asSetMaterialTexture(SMaterial* material, int index, ITexture* texture) {
    material->setTexture(index, texture);
}


EXPORT ITexture* CALL asMaterialTexture(SMaterial* material, int index) {
    return material->getTexture(index);
}


EXPORT void CALL asSetMaterialBlend(SMaterial* material, int blend) {
    material->MaterialType = _asIrrlichtMaterialType(blend);
}


EXPORT int CALL asMaterialBlend(SMaterial* material) {
    return _asBlendType(material->MaterialType);
}


EXPORT void CALL asSetMaterialDiffuse(SMaterial* material, int color) {
    material->DiffuseColor = _asColor(color);
}


EXPORT int CALL asMaterialDiffuse(SMaterial* material) {
    return _asIntColor(material->DiffuseColor);
}


EXPORT void CALL asSetMaterialEmissive(SMaterial* material, int color) {
    material->EmissiveColor = _asColor(color);
}


EXPORT int CALL asMaterialEmissive(SMaterial* material) {
    return _asIntColor(material->EmissiveColor);
}


EXPORT void CALL asSetMaterialAmbient(SMaterial* material, int color) {
    material->AmbientColor = _asColor(color);
}


EXPORT int CALL asMaterialAmbient(SMaterial* material) {
    return _asIntColor(material->AmbientColor);
}


EXPORT void CALL asSetMaterialSpecular(SMaterial* material, int color) {
    material->SpecularColor = _asColor(color);
}


EXPORT int CALL asMaterialSpecular(SMaterial* material) {
    return _asIntColor(material->SpecularColor);
}


EXPORT void CALL asSetMaterialShininess(SMaterial* material, float shininess) {
    material->Shininess = shininess * 128;
}


EXPORT float CALL asMaterialShininess(SMaterial* material) {
    return material->Shininess / 128;
}


EXPORT void CALL asSetMaterialRenderMode(SMaterial* material, int mode) {
    material->setFlag(EMF_WIREFRAME, mode == ASRENDER_WIREFRAME);
    material->setFlag(EMF_POINTCLOUD, mode == ASRENDER_POINTCLOUD);
}


EXPORT int CALL asMaterialRenderMode(SMaterial* material) {
    if (material->getFlag(EMF_POINTCLOUD)) return ASRENDER_POINTCLOUD;
    else if (material->getFlag(EMF_WIREFRAME)) return ASRENDER_WIREFRAME;
    else return ASRENDER_NORMAL;
}


EXPORT void CALL asSetMaterialFilterMode(SMaterial* material, int mode) {
    material->setFlag(EMF_ANISOTROPIC_FILTER, mode == ASFILTER_ANISOTROPIC);
    material->setFlag(EMF_TRILINEAR_FILTER, mode >= ASFILTER_TRILINEAR);
    material->setFlag(EMF_BILINEAR_FILTER, mode >= ASFILTER_BILINEAR);
}


EXPORT int CALL asMaterialFilterMode(SMaterial* material) {
    if (material->getFlag(EMF_ANISOTROPIC_FILTER)) return ASFILTER_ANISOTROPIC;
    if (material->getFlag(EMF_TRILINEAR_FILTER)) return ASFILTER_TRILINEAR;
    if (material->getFlag(EMF_BILINEAR_FILTER)) return ASFILTER_BILINEAR;
    else return ASFILTER_DISABLED;
}


EXPORT void CALL asSetMaterialLightingMode(SMaterial* material, int mode) {
    material->setFlag(EMF_LIGHTING, mode != ASLIGHTING_DISABLED);
    material->setFlag(EMF_GOURAUD_SHADING, mode == ASLIGHTING_GOURAUD);
}

EXPORT int CALL asMaterialLightingMode(SMaterial* material) {
    if (!material->getFlag(EMF_LIGHTING)) return ASLIGHTING_DISABLED;
    else if (material->getFlag(EMF_GOURAUD_SHADING)) return ASLIGHTING_GOURAUD;
    else return ASLIGHTING_FLAT;
}


EXPORT void CALL asSetMaterialFogEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_FOG_ENABLE, enabled);
}


EXPORT bool_t CALL asMaterialFogEnabled(SMaterial* material) {
    return material->getFlag(EMF_FOG_ENABLE);
}


EXPORT void CALL asSetMaterialDepthWritingEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_ZWRITE_ENABLE, enabled);
}


EXPORT bool_t CALL asMaterialDepthWritingEnabled(SMaterial* material) {
    return material->getFlag(EMF_ZWRITE_ENABLE);
}


EXPORT void CALL asSetMaterialCullingEnabled(SMaterial* material, bool_t enabled) {
    material->setFlag(EMF_BACK_FACE_CULLING, enabled);
}


EXPORT bool_t CALL asMaterialCullingEnabled(SMaterial* material) {
    return material->getFlag(EMF_BACK_FACE_CULLING);
}


E_MATERIAL_TYPE _asIrrlichtMaterialType(int blend) {
    map<int, E_MATERIAL_TYPE> types;
    types.set(ASBLEND_SOLID, EMT_SOLID);
    types.set(ASBLEND_LIGHTMAP, EMT_LIGHTMAP_LIGHTING);
    types.set(ASBLEND_LIGHTMAP2X, EMT_LIGHTMAP_LIGHTING_M2);
    types.set(ASBLEND_LIGHTMAP4X, EMT_LIGHTMAP_LIGHTING_M4);
    types.set(ASBLEND_DETAIL, EMT_DETAIL_MAP);
    types.set(ASBLEND_ADD, EMT_TRANSPARENT_ADD_COLOR);
    types.set(ASBLEND_ALPHA, EMT_TRANSPARENT_ALPHA_CHANNEL);
    types.set(ASBLEND_VERTEXALPHA, EMT_TRANSPARENT_VERTEX_ALPHA);
    types.set(ASBLEND_MASKED, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    types.set(ASBLEND_SPHERE, EMT_SPHERE_MAP);
    types.set(ASBLEND_REFLECTION, EMT_TRANSPARENT_REFLECTION_2_LAYER);
    types.set(ASBLEND_NORMAL, EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(ASBLEND_NORMALADD, EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR);
    types.set(ASBLEND_PARALLAX, EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(ASBLEND_PARALLAXADD, EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR);
    return types[blend];
}


int _asBlendType(E_MATERIAL_TYPE type) {
    map<E_MATERIAL_TYPE, int> types;
    types.set(EMT_SOLID, ASBLEND_SOLID);
    types.set(EMT_LIGHTMAP_LIGHTING, ASBLEND_LIGHTMAP);
    types.set(EMT_LIGHTMAP_LIGHTING_M2, ASBLEND_LIGHTMAP2X);
    types.set(EMT_LIGHTMAP_LIGHTING_M4, ASBLEND_LIGHTMAP4X);
    types.set(EMT_DETAIL_MAP, ASBLEND_DETAIL);
    types.set(EMT_TRANSPARENT_ADD_COLOR, ASBLEND_ADD);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL, ASBLEND_ALPHA);
    types.set(EMT_TRANSPARENT_VERTEX_ALPHA, ASBLEND_VERTEXALPHA);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL_REF, ASBLEND_MASKED);
    types.set(EMT_SPHERE_MAP, ASBLEND_SPHERE);
    types.set(EMT_TRANSPARENT_REFLECTION_2_LAYER, ASBLEND_REFLECTION);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA, ASBLEND_NORMAL);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR, ASBLEND_NORMALADD);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA, ASBLEND_PARALLAX);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR, ASBLEND_PARALLAXADD);
    return types[type];
}


} // extern "C"
