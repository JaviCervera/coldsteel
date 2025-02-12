#include "core.h"
#include "graphics.h"
#include "light.h"
#include "world.h"

ILightSceneNode *CreateLight(int type)
{
  map<int, E_LIGHT_TYPE> types;
  types[LIGHT_DIRECTIONAL] = ELT_DIRECTIONAL;
  types[LIGHT_POINT] = ELT_POINT;
  types[LIGHT_SPOT] = ELT_SPOT;
  ILightSceneNode *light = _Device()->getSceneManager()->addLightSceneNode();
  light->setLightType(types[type]);
  if (!_HasSetAmbient())
    _Device()->getSceneManager()->setAmbientLight(_Color(RGB(30, 30, 30)));
  return light;
}

int LightType(ILightSceneNode *light)
{
  map<E_LIGHT_TYPE, int> types;
  types[ELT_DIRECTIONAL] = LIGHT_DIRECTIONAL;
  types[ELT_POINT] = LIGHT_POINT;
  types[ELT_SPOT] = LIGHT_SPOT;
  return types[light->getLightType()];
}

void SetLightDiffuse(ILightSceneNode *light, int color)
{
  light->getLightData().DiffuseColor = _Color(color);
}

int LightDiffuse(ILightSceneNode *light)
{
  return _IntColor(light->getLightData().DiffuseColor.toSColor());
}

void SetLightAmbient(ILightSceneNode *light, int color)
{
  light->getLightData().AmbientColor = _Color(color);
}

int LightAmbient(ILightSceneNode *light)
{
  return _IntColor(light->getLightData().AmbientColor.toSColor());
}

void SetLightSpecular(ILightSceneNode *light, int color)
{
  light->getLightData().SpecularColor = _Color(color);
}

int LightSpecular(ILightSceneNode *light)
{
  return _IntColor(light->getLightData().SpecularColor.toSColor());
}

void SetLightCastShadows(ILightSceneNode *light, bool_t enable)
{
  light->enableCastShadow(enable);
}

bool_t LightCastShadows(ILightSceneNode *light)
{
  return light->getCastShadow();
}

void SetLightRadius(ILightSceneNode *light, float radius)
{
  light->setRadius(radius);
}

float LightRadius(ILightSceneNode *light)
{
  return light->getRadius();
}

void SetLightAttenuation(ILightSceneNode *light, float constant, float linear, float quadratic)
{
  light->getLightData().Attenuation = vector3df(constant, linear, quadratic);
}

float LightConstantAttenuation(ILightSceneNode *light)
{
  return light->getLightData().Attenuation.X;
}

float LightLinearAttenuation(ILightSceneNode *light)
{
  return light->getLightData().Attenuation.Y;
}

float LightQuadraticAttenuation(ILightSceneNode *light)
{
  return light->getLightData().Attenuation.Z;
}

void SetLightCone(ILightSceneNode *light, float inner, float outer)
{
  light->getLightData().InnerCone = inner;
  light->getLightData().OuterCone = outer;
}

float LightInnerCone(ILightSceneNode *light)
{
  return light->getLightData().InnerCone;
}

float LightOuterCone(ILightSceneNode *light)
{
  return light->getLightData().OuterCone;
}

void SetLightFalloff(ILightSceneNode *light, float falloff)
{
  light->getLightData().Falloff = falloff;
}

float LightFalloff(ILightSceneNode *light)
{
  return light->getLightData().Falloff;
}
