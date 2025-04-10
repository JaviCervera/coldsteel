#pragma once

#include "common.h"

// Light types
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ILightSceneNode *CALL CreateLight(int type);
  EXPORT int CALL LightType(ILightSceneNode *light);
  EXPORT void CALL SetLightDiffuse(ILightSceneNode *light, int color);
  EXPORT int CALL LightDiffuse(ILightSceneNode *light);
  EXPORT void CALL SetLightAmbient(ILightSceneNode *light, int color);
  EXPORT int CALL LightAmbient(ILightSceneNode *light);
  EXPORT void CALL SetLightSpecular(ILightSceneNode *light, int color);
  EXPORT int CALL LightSpecular(ILightSceneNode *light);
  EXPORT void CALL SetLightCastShadows(ILightSceneNode *light, bool_t enable);
  EXPORT bool_t CALL LightCastShadows(ILightSceneNode *light);
  EXPORT void CALL SetLightRadius(ILightSceneNode *light, float radius);
  EXPORT float CALL LightRadius(ILightSceneNode *light);
  EXPORT void CALL SetLightAttenuation(ILightSceneNode *light, float constant, float linear, float quadratic);
  EXPORT float CALL LightConstantAttenuation(ILightSceneNode *light);
  EXPORT float CALL LightLinearAttenuation(ILightSceneNode *light);
  EXPORT float CALL LightQuadraticAttenuation(ILightSceneNode *light);
  EXPORT void CALL SetLightCone(ILightSceneNode *light, float inner, float outer);
  EXPORT float CALL LightInnerCone(ILightSceneNode *light);
  EXPORT float CALL LightOuterCone(ILightSceneNode *light);
  EXPORT void CALL SetLightFalloff(ILightSceneNode *light, float falloff);
  EXPORT float CALL LightFalloff(ILightSceneNode *light);

#ifdef __cplusplus
} /* extern "C" */
#endif
