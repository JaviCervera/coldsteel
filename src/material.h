#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL SetMaterialTexture(SMaterial* material, int index, ITexture* texture);
EXPORT ITexture* CALL MaterialTexture(SMaterial* material, int index);
EXPORT void CALL SetMaterialTextureTransform(SMaterial* material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);
EXPORT void CALL SetMaterialType(SMaterial* material, int type);
EXPORT int CALL MaterialType(SMaterial* material);
EXPORT void CALL SetMaterialDiffuse(SMaterial* material, int color);
EXPORT int CALL MaterialDiffuse(SMaterial* material);
EXPORT void CALL SetMaterialEmissive(SMaterial* material, int color);
EXPORT int CALL MaterialEmissive(SMaterial* material);
EXPORT void CALL SetMaterialAmbient(SMaterial* material, int color);
EXPORT int CALL MaterialAmbient(SMaterial* material);
EXPORT void CALL SetMaterialSpecular(SMaterial* material, int color);
EXPORT int CALL MaterialSpecular(SMaterial* material);
EXPORT void CALL SetMaterialShininess(SMaterial* material, float shininess);
EXPORT float CALL MaterialShininess(SMaterial* material);
EXPORT void CALL SetMaterialRenderMode(SMaterial* material, int mode);
EXPORT int CALL MaterialRenderMode(SMaterial* material);
EXPORT void CALL SetMaterialFilterMode(SMaterial* material, int mode);
EXPORT int CALL MaterialFilterMode(SMaterial* material);
EXPORT void CALL SetMaterialLightingMode(SMaterial* material, int mode);
EXPORT int CALL MaterialLightingMode(SMaterial* material);
EXPORT void CALL SetMaterialFogEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL MaterialFogEnabled(SMaterial* material);
EXPORT void CALL SetMaterialDepthReadingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL MaterialDepthReadingEnabled(SMaterial* material);
EXPORT void CALL SetMaterialDepthWritingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL MaterialDepthWritingEnabled(SMaterial* material);
EXPORT void CALL SetMaterialCullingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL MaterialCullingEnabled(SMaterial* material);
EXPORT void CALL SetMaterialVertexColorsEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL MaterialVertexColorsEnabled(SMaterial* material);


#ifdef __cplusplus
} /* extern "C" */
#endif
