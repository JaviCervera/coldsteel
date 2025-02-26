#include "graphics.h"
#include "material.h"
#include "math.h"

extern "C"
{

  E_MATERIAL_TYPE _IrrlichtMaterialType(int blend);
  int _MaterialType(E_MATERIAL_TYPE type);

  EXPORT void CALL SetMaterialTexture(SMaterial *material, int index, ITexture *texture)
  {
    material->setTexture(index, texture);
  }

  EXPORT ITexture *CALL MaterialTexture(SMaterial *material, int index)
  {
    return material->getTexture(index);
  }

  EXPORT void CALL SetMaterialTextureTransform(SMaterial *material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale)
  {
    matrix4 mat = matrix4().buildTextureTransform(Rad(angle), vector2df(0, 0), vector2df(xoffset, yoffset), vector2df(xscale, yscale));
    material->setTextureMatrix(index, mat);
  }

  EXPORT void CALL SetMaterialType(SMaterial *material, int type)
  {
    material->MaterialType = _IrrlichtMaterialType(type);
  }

  EXPORT int CALL MaterialType(SMaterial *material)
  {
    return _MaterialType(material->MaterialType);
  }

  EXPORT void CALL SetMaterialDiffuse(SMaterial *material, int color)
  {
    material->DiffuseColor = _Color(color);
  }

  EXPORT int CALL MaterialDiffuse(SMaterial *material)
  {
    return _IntColor(material->DiffuseColor);
  }

  EXPORT void CALL SetMaterialEmissive(SMaterial *material, int color)
  {
    material->EmissiveColor = _Color(color);
  }

  EXPORT int CALL MaterialEmissive(SMaterial *material)
  {
    return _IntColor(material->EmissiveColor);
  }

  EXPORT void CALL SetMaterialAmbient(SMaterial *material, int color)
  {
    material->AmbientColor = _Color(color);
  }

  EXPORT int CALL MaterialAmbient(SMaterial *material)
  {
    return _IntColor(material->AmbientColor);
  }

  EXPORT void CALL SetMaterialSpecular(SMaterial *material, int color)
  {
    material->SpecularColor = _Color(color);
  }

  EXPORT int CALL MaterialSpecular(SMaterial *material)
  {
    return _IntColor(material->SpecularColor);
  }

  EXPORT void CALL SetMaterialShininess(SMaterial *material, float shininess)
  {
    material->Shininess = shininess * 128;
  }

  EXPORT float CALL MaterialShininess(SMaterial *material)
  {
    return material->Shininess / 128;
  }

  EXPORT void CALL SetMaterialRenderMode(SMaterial *material, int mode)
  {
    material->setFlag(EMF_WIREFRAME, mode == RENDER_WIREFRAME);
    material->setFlag(EMF_POINTCLOUD, mode == RENDER_POINTCLOUD);
  }

  EXPORT int CALL MaterialRenderMode(SMaterial *material)
  {
    if (material->getFlag(EMF_POINTCLOUD))
      return RENDER_POINTCLOUD;
    else if (material->getFlag(EMF_WIREFRAME))
      return RENDER_WIREFRAME;
    else
      return RENDER_FILLED;
  }

  EXPORT void CALL SetMaterialFilterMode(SMaterial *material, int mode)
  {
    material->setFlag(EMF_ANISOTROPIC_FILTER, mode == FILTER_ANISOTROPIC);
    material->setFlag(EMF_TRILINEAR_FILTER, mode >= FILTER_TRILINEAR);
    material->setFlag(EMF_BILINEAR_FILTER, mode >= FILTER_BILINEAR);
    material->setFlag(EMF_USE_MIP_MAPS, mode >= FILTER_BILINEAR);
  }

  EXPORT int CALL MaterialFilterMode(SMaterial *material)
  {
    if (material->getFlag(EMF_ANISOTROPIC_FILTER))
      return FILTER_ANISOTROPIC;
    if (material->getFlag(EMF_TRILINEAR_FILTER))
      return FILTER_TRILINEAR;
    if (material->getFlag(EMF_BILINEAR_FILTER))
      return FILTER_BILINEAR;
    else
      return FILTER_DISABLED;
  }

  EXPORT void CALL SetMaterialLightingMode(SMaterial *material, int mode)
  {
    material->setFlag(EMF_LIGHTING, mode != LIGHTING_DISABLED);
    material->setFlag(EMF_GOURAUD_SHADING, mode == LIGHTING_GOURAUD);
  }

  EXPORT int CALL MaterialLightingMode(SMaterial *material)
  {
    if (!material->getFlag(EMF_LIGHTING))
      return LIGHTING_DISABLED;
    else if (material->getFlag(EMF_GOURAUD_SHADING))
      return LIGHTING_GOURAUD;
    else
      return LIGHTING_FLAT;
  }

  EXPORT void CALL SetMaterialFogEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_FOG_ENABLE, enabled);
  }

  EXPORT bool_t CALL MaterialFogEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_FOG_ENABLE);
  }

  EXPORT void CALL SetMaterialDepthReadingEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_ZBUFFER, enabled);
  }

  EXPORT bool_t CALL MaterialDepthReadingEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_ZBUFFER);
  }

  EXPORT void CALL SetMaterialDepthWritingEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_ZWRITE_ENABLE, enabled);
  }

  EXPORT bool_t CALL MaterialDepthWritingEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_ZWRITE_ENABLE);
  }

  EXPORT void CALL SetMaterialCullingEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_BACK_FACE_CULLING, enabled);
  }

  EXPORT bool_t CALL MaterialCullingEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_BACK_FACE_CULLING);
  }

  EXPORT void CALL SetMaterialVertexColorsEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_COLOR_MATERIAL, enabled);
  }

  EXPORT bool_t CALL MaterialVertexColorsEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_COLOR_MATERIAL);
  }

  EXPORT void CALL SetMaterialNormalizeEnabled(SMaterial *material, bool_t enabled)
  {
    material->setFlag(EMF_NORMALIZE_NORMALS, enabled);
  }

  EXPORT bool_t CALL MaterialNormalizeEnabled(SMaterial *material)
  {
    return material->getFlag(EMF_NORMALIZE_NORMALS);
  }

  E_MATERIAL_TYPE _IrrlichtMaterialType(int blend)
  {
    map<int, E_MATERIAL_TYPE> types;
    types.set(MATERIAL_SOLID, EMT_SOLID);
    types.set(MATERIAL_LIGHTMAP, EMT_LIGHTMAP_LIGHTING);
    types.set(MATERIAL_LIGHTMAP2X, EMT_LIGHTMAP_LIGHTING_M2);
    types.set(MATERIAL_LIGHTMAP4X, EMT_LIGHTMAP_LIGHTING_M4);
    types.set(MATERIAL_DETAIL, EMT_DETAIL_MAP);
    types.set(MATERIAL_ADD, EMT_TRANSPARENT_ADD_COLOR);
    types.set(MATERIAL_ALPHA, EMT_TRANSPARENT_ALPHA_CHANNEL);
    types.set(MATERIAL_VERTEXALPHA, EMT_TRANSPARENT_VERTEX_ALPHA);
    types.set(MATERIAL_MASKED, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    types.set(MATERIAL_SPHERE, EMT_SPHERE_MAP);
    types.set(MATERIAL_REFLECTION, EMT_TRANSPARENT_REFLECTION_2_LAYER);
    types.set(MATERIAL_NORMAL, EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(MATERIAL_NORMALADD, EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR);
    types.set(MATERIAL_PARALLAX, EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA);
    types.set(MATERIAL_PARALLAXADD, EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR);
    return types[blend];
  }

  int _MaterialType(E_MATERIAL_TYPE type)
  {
    map<E_MATERIAL_TYPE, int> types;
    types.set(EMT_SOLID, MATERIAL_SOLID);
    types.set(EMT_LIGHTMAP_LIGHTING, MATERIAL_LIGHTMAP);
    types.set(EMT_LIGHTMAP_LIGHTING_M2, MATERIAL_LIGHTMAP2X);
    types.set(EMT_LIGHTMAP_LIGHTING_M4, MATERIAL_LIGHTMAP4X);
    types.set(EMT_DETAIL_MAP, MATERIAL_DETAIL);
    types.set(EMT_TRANSPARENT_ADD_COLOR, MATERIAL_ADD);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL, MATERIAL_ALPHA);
    types.set(EMT_TRANSPARENT_VERTEX_ALPHA, MATERIAL_VERTEXALPHA);
    types.set(EMT_TRANSPARENT_ALPHA_CHANNEL_REF, MATERIAL_MASKED);
    types.set(EMT_SPHERE_MAP, MATERIAL_SPHERE);
    types.set(EMT_TRANSPARENT_REFLECTION_2_LAYER, MATERIAL_REFLECTION);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA, MATERIAL_NORMAL);
    types.set(EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR, MATERIAL_NORMALADD);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA, MATERIAL_PARALLAX);
    types.set(EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR, MATERIAL_PARALLAXADD);
    return types[type];
  }

} // extern "C"
