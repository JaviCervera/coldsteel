#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asSetMaterialTexture(SMaterial* material, int index, ITexture* texture);
EXPORT ITexture* CALL asMaterialTexture(SMaterial* material, int index);
EXPORT void CALL asSetMaterialTextureTransform(SMaterial* material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);
EXPORT void CALL asSetMaterialType(SMaterial* material, int type);
EXPORT int CALL asMaterialType(SMaterial* material);
EXPORT void CALL asSetMaterialDiffuse(SMaterial* material, int color);
EXPORT int CALL asMaterialDiffuse(SMaterial* material);
EXPORT void CALL asSetMaterialEmissive(SMaterial* material, int color);
EXPORT int CALL asMaterialEmissive(SMaterial* material);
EXPORT void CALL asSetMaterialAmbient(SMaterial* material, int color);
EXPORT int CALL asMaterialAmbient(SMaterial* material);
EXPORT void CALL asSetMaterialSpecular(SMaterial* material, int color);
EXPORT int CALL asMaterialSpecular(SMaterial* material);
EXPORT void CALL asSetMaterialShininess(SMaterial* material, float shininess);
EXPORT float CALL asMaterialShininess(SMaterial* material);
EXPORT void CALL asSetMaterialRenderMode(SMaterial* material, int mode);
EXPORT int CALL asMaterialRenderMode(SMaterial* material);
EXPORT void CALL asSetMaterialFilterMode(SMaterial* material, int mode);
EXPORT int CALL asMaterialFilterMode(SMaterial* material);
EXPORT void CALL asSetMaterialLightingMode(SMaterial* material, int mode);
EXPORT int CALL asMaterialLightingMode(SMaterial* material);
EXPORT void CALL asSetMaterialFogEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL asMaterialFogEnabled(SMaterial* material);
EXPORT void CALL asSetMaterialDepthWritingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL asMaterialDepthWritingEnabled(SMaterial* material);
EXPORT void CALL asSetMaterialCullingEnabled(SMaterial* material, bool_t enabled);
EXPORT bool_t CALL asMaterialCullingEnabled(SMaterial* material);


#ifdef __cplusplus
} /* extern "C" */
#endif
