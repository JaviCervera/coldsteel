#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ILightSceneNode* CALL asCreateLight(int type);
EXPORT int CALL asLightType(ILightSceneNode* light);
EXPORT void CALL asSetLightDiffuse(ILightSceneNode* light, int color);
EXPORT int CALL asLightDiffuse(ILightSceneNode* light);
EXPORT void CALL asSetLightAmbient(ILightSceneNode* light, int color);
EXPORT int CALL asLightAmbient(ILightSceneNode* light);
EXPORT void CALL asSetLightSpecular(ILightSceneNode* light, int color);
EXPORT int CALL asLightSpecular(ILightSceneNode* light);
EXPORT void CALL asSetLightCastShadows(ILightSceneNode* light, bool_t enable);
EXPORT bool_t CALL asLightCastShadows(ILightSceneNode* light);
EXPORT void CALL asSetLightRadius(ILightSceneNode* light, float radius);
EXPORT float CALL asLightRadius(ILightSceneNode* light);
EXPORT void CALL asSetLightAttenuation(ILightSceneNode* light, float constant, float linear, float quadratic);
EXPORT float CALL asLightConstantAttenuation(ILightSceneNode* light);
EXPORT float CALL asLightLinearAttenuation(ILightSceneNode* light);
EXPORT float CALL asLightQuadraticAttenuation(ILightSceneNode* light);
EXPORT void CALL asSetLightCone(ILightSceneNode* light, float inner, float outer);
EXPORT float CALL asLightInnerCone(ILightSceneNode* light);
EXPORT float CALL asLightOuterCone(ILightSceneNode* light);
EXPORT void CALL asSetLightFalloff(ILightSceneNode* light, float falloff);
EXPORT float CALL asLightFalloff(ILightSceneNode* light);


#ifdef __cplusplus
} /* extern "C" */
#endif
