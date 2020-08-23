#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csSetMaterialTexture(SMaterial* material, int index, ITexture* texture);
EXPORT ITexture* CALL csMaterialTexture(SMaterial* material, int index);
EXPORT void CALL csSetMaterialTextureTransform(SMaterial* material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);
EXPORT void CALL csSetMaterialType(SMaterial* material, int type);
EXPORT int CALL csMaterialType(SMaterial* material);
EXPORT void CALL csSetMaterialDiffuse(SMaterial* material, int color);
EXPORT int CALL csMaterialDiffuse(SMaterial* material);
EXPORT void CALL csSetMaterialEmissive(SMaterial* material, int color);
EXPORT int CALL csMaterialEmissive(SMaterial* material);
EXPORT void CALL csSetMaterialAmbient(SMaterial* material, int color);
EXPORT int CALL csMaterialAmbient(SMaterial* material);
EXPORT void CALL csSetMaterialSpecular(SMaterial* material, int color);
EXPORT int CALL csMaterialSpecular(SMaterial* material);
EXPORT void CALL csSetMaterialShininess(SMaterial* material, float shininess);
EXPORT float CALL csMaterialShininess(SMaterial* material);
EXPORT void CALL csSetMaterialRenderMode(SMaterial* material, int mode);
EXPORT int CALL csMaterialRenderMode(SMaterial* material);
EXPORT void CALL csSetMaterialFilterMode(SMaterial* material, int mode);
EXPORT int CALL csMaterialFilterMode(SMaterial* material);
EXPORT void CALL csSetMaterialLightingMode(SMaterial* material, int mode);
EXPORT int CALL csMaterialLightingMode(SMaterial* material);
EXPORT void CALL csSetMaterialFogEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL csMaterialFogEnabled(SMaterial* material);
EXPORT void CALL csSetMaterialDepthReadingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL csMaterialDepthReadingEnabled(SMaterial* material);
EXPORT void CALL csSetMaterialDepthWritingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL csMaterialDepthWritingEnabled(SMaterial* material);
EXPORT void CALL csSetMaterialCullingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL csMaterialCullingEnabled(SMaterial* material);
EXPORT void CALL csSetMaterialVertexColorsEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL csMaterialVertexColorsEnabled(SMaterial* material);


#ifdef __cplusplus
} /* extern "C" */
#endif
