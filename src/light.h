#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ILightSceneNode* CALL csCreateLight(int type);
EXPORT int CALL csLightType(ILightSceneNode* light);
EXPORT void CALL csSetLightDiffuse(ILightSceneNode* light, int color);
EXPORT int CALL csLightDiffuse(ILightSceneNode* light);
EXPORT void CALL csSetLightAmbient(ILightSceneNode* light, int color);
EXPORT int CALL csLightAmbient(ILightSceneNode* light);
EXPORT void CALL csSetLightSpecular(ILightSceneNode* light, int color);
EXPORT int CALL csLightSpecular(ILightSceneNode* light);
EXPORT void CALL csSetLightCastShadows(ILightSceneNode* light, bool_t enable);
EXPORT bool_t CALL csLightCastShadows(ILightSceneNode* light);
EXPORT void CALL csSetLightRadius(ILightSceneNode* light, float radius);
EXPORT float CALL csLightRadius(ILightSceneNode* light);
EXPORT void CALL csSetLightAttenuation(ILightSceneNode* light, float constant, float linear, float quadratic);
EXPORT float CALL csLightConstantAttenuation(ILightSceneNode* light);
EXPORT float CALL csLightLinearAttenuation(ILightSceneNode* light);
EXPORT float CALL csLightQuadraticAttenuation(ILightSceneNode* light);
EXPORT void CALL csSetLightCone(ILightSceneNode* light, float inner, float outer);
EXPORT float CALL csLightInnerCone(ILightSceneNode* light);
EXPORT float CALL csLightOuterCone(ILightSceneNode* light);
EXPORT void CALL csSetLightFalloff(ILightSceneNode* light, float falloff);
EXPORT float CALL csLightFalloff(ILightSceneNode* light);


#ifdef __cplusplus
} /* extern "C" */
#endif
