#pragma once

#include "common.h"

ILightSceneNode *CreateLight(int type);
int LightType(ILightSceneNode *light);
void SetLightDiffuse(ILightSceneNode *light, int color);
int LightDiffuse(ILightSceneNode *light);
void SetLightAmbient(ILightSceneNode *light, int color);
int LightAmbient(ILightSceneNode *light);
void SetLightSpecular(ILightSceneNode *light, int color);
int LightSpecular(ILightSceneNode *light);
void SetLightCastShadows(ILightSceneNode *light, bool_t enable);
bool_t LightCastShadows(ILightSceneNode *light);
void SetLightRadius(ILightSceneNode *light, float radius);
float LightRadius(ILightSceneNode *light);
void SetLightAttenuation(ILightSceneNode *light, float constant, float linear, float quadratic);
float LightConstantAttenuation(ILightSceneNode *light);
float LightLinearAttenuation(ILightSceneNode *light);
float LightQuadraticAttenuation(ILightSceneNode *light);
void SetLightCone(ILightSceneNode *light, float inner, float outer);
float LightInnerCone(ILightSceneNode *light);
float LightOuterCone(ILightSceneNode *light);
void SetLightFalloff(ILightSceneNode *light, float falloff);
float LightFalloff(ILightSceneNode *light);
