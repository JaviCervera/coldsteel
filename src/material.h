#pragma once

#include "common.h"

void SetMaterialTexture(SMaterial *material, int index, ITexture *texture);
ITexture *MaterialTexture(SMaterial *material, int index);
void SetMaterialTextureTransform(SMaterial *material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);
void SetMaterialType(SMaterial *material, int type);
int MaterialType(SMaterial *material);
void SetMaterialDiffuse(SMaterial *material, int color);
int MaterialDiffuse(SMaterial *material);
void SetMaterialEmissive(SMaterial *material, int color);
int MaterialEmissive(SMaterial *material);
void SetMaterialAmbient(SMaterial *material, int color);
int MaterialAmbient(SMaterial *material);
void SetMaterialSpecular(SMaterial *material, int color);
int MaterialSpecular(SMaterial *material);
void SetMaterialShininess(SMaterial *material, float shininess);
float MaterialShininess(SMaterial *material);
void SetMaterialRenderMode(SMaterial *material, int mode);
int MaterialRenderMode(SMaterial *material);
void SetMaterialFilterMode(SMaterial *material, int mode);
int MaterialFilterMode(SMaterial *material);
void SetMaterialLightingMode(SMaterial *material, int mode);
int MaterialLightingMode(SMaterial *material);
void SetMaterialFogEnabled(SMaterial *material, bool_t enabled);
bool_t MaterialFogEnabled(SMaterial *material);
void SetMaterialDepthReadingEnabled(SMaterial *material, bool_t enabled);
bool_t MaterialDepthReadingEnabled(SMaterial *material);
void SetMaterialDepthWritingEnabled(SMaterial *material, bool_t enabled);
bool_t MaterialDepthWritingEnabled(SMaterial *material);
void SetMaterialCullingEnabled(SMaterial *material, bool_t enabled);
bool_t MaterialCullingEnabled(SMaterial *material);
void SetMaterialVertexColorsEnabled(SMaterial *material, bool_t enabled);
bool_t MaterialVertexColorsEnabled(SMaterial *material);
