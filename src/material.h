#pragma once

#include "common.h"

// Blend modes
#define MATERIAL_SOLID 0
#define MATERIAL_LIGHTMAP 1
#define MATERIAL_LIGHTMAP2X 2
#define MATERIAL_LIGHTMAP4X 3
#define MATERIAL_DETAIL 4
#define MATERIAL_ADD 5
#define MATERIAL_ALPHA 6
#define MATERIAL_VERTEXALPHA 7
#define MATERIAL_MASKED 8
#define MATERIAL_SPHERE 9
#define MATERIAL_REFLECTION 10
#define MATERIAL_NORMAL 11
#define MATERIAL_NORMALADD 12
#define MATERIAL_PARALLAX 13
#define MATERIAL_PARALLAXADD 14

// Render modes
#define RENDER_FILLED 0
#define RENDER_WIREFRAME 1
#define RENDER_POINTCLOUD 2

// Lighting modes
#define LIGHTING_DISABLED 0
#define LIGHTING_GOURAUD 1
#define LIGHTING_FLAT 2

// Filtering modes
#define FILTER_DISABLED 0
#define FILTER_BILINEAR 1
#define FILTER_TRILINEAR 2
#define FILTER_ANISOTROPIC 3

// Material flags
#define FLAG_LIGHTING 1
#define FLAG_FOG 2
#define FLAG_ZREAD 4
#define FLAG_ZWRITE 8
#define FLAG_BACKFACECULLING 16
#define FLAG_NORMALIZE 32
#define FLAG_VERTEXCOLORS 64

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetMaterialTexture(SMaterial *material, int index, ITexture *texture);
  EXPORT ITexture *CALL MaterialTexture(SMaterial *material, int index);
  EXPORT void CALL SetMaterialTextureTransform(SMaterial *material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);
  EXPORT void CALL SetMaterialType(SMaterial *material, int type);
  EXPORT int CALL MaterialType(SMaterial *material);
  EXPORT void CALL SetMaterialDiffuse(SMaterial *material, int color);
  EXPORT int CALL MaterialDiffuse(SMaterial *material);
  EXPORT void CALL SetMaterialEmissive(SMaterial *material, int color);
  EXPORT int CALL MaterialEmissive(SMaterial *material);
  EXPORT void CALL SetMaterialAmbient(SMaterial *material, int color);
  EXPORT int CALL MaterialAmbient(SMaterial *material);
  EXPORT void CALL SetMaterialSpecular(SMaterial *material, int color);
  EXPORT int CALL MaterialSpecular(SMaterial *material);
  EXPORT void CALL SetMaterialShininess(SMaterial *material, float shininess);
  EXPORT float CALL MaterialShininess(SMaterial *material);
  EXPORT void CALL SetMaterialRenderMode(SMaterial *material, int mode);
  EXPORT int CALL MaterialRenderMode(SMaterial *material);
  EXPORT void CALL SetMaterialFilterMode(SMaterial *material, int mode);
  EXPORT int CALL MaterialFilterMode(SMaterial *material);
  EXPORT void CALL SetMaterialLightingMode(SMaterial *material, int mode);
  EXPORT int CALL MaterialLightingMode(SMaterial *material);
  EXPORT void CALL SetMaterialFlag(SMaterial *material, int flag, bool_t enable);
  EXPORT bool_t CALL MaterialFlag(SMaterial *material, int flag);

#ifdef __cplusplus
} /* extern "C" */
#endif
