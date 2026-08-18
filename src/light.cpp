#include "core.h"
#include "graphics.h"
#include "light.h"
#include "world.h"

static const float kLightCutoff = 0.01f; // light strength at the radius edge

static float QuadraticFromRadius(float radius)
{
  return (1.f / kLightCutoff - 1.f) / (radius * radius);
}

static float RadiusFromAttenuation(float constant, float linear, float quadratic)
{
  if (quadratic > 0.f)
  {
    const float disc = linear * linear + 4.f * quadratic * (1.f / kLightCutoff - constant);
    if (disc <= 0.f)
      return 0.f;
    const float r = (-linear + sqrtf(disc)) / (2.f * quadratic);
    return r > 0.f ? r : 0.f;
  }
  if (linear > 0.f)
  {
    const float r = (1.f / kLightCutoff - constant) / linear;
    return r > 0.f ? r : 0.f;
  }
  return 0.f;
}

extern "C"
{

  EXPORT ILightSceneNode *CALL CreateLight(int type)
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

  EXPORT int CALL LightType(ILightSceneNode *light)
  {
    map<E_LIGHT_TYPE, int> types;
    types[ELT_DIRECTIONAL] = LIGHT_DIRECTIONAL;
    types[ELT_POINT] = LIGHT_POINT;
    types[ELT_SPOT] = LIGHT_SPOT;
    return types[light->getLightType()];
  }

  EXPORT void CALL SetLightDiffuse(ILightSceneNode *light, int color)
  {
    light->getLightData().DiffuseColor = _Color(color);
  }

  EXPORT int CALL LightDiffuse(ILightSceneNode *light)
  {
    return _IntColor(light->getLightData().DiffuseColor.toSColor());
  }

  EXPORT void CALL SetLightAmbient(ILightSceneNode *light, int color)
  {
    light->getLightData().AmbientColor = _Color(color);
  }

  EXPORT int CALL LightAmbient(ILightSceneNode *light)
  {
    return _IntColor(light->getLightData().AmbientColor.toSColor());
  }

  EXPORT void CALL SetLightSpecular(ILightSceneNode *light, int color)
  {
    light->getLightData().SpecularColor = _Color(color);
  }

  EXPORT int CALL LightSpecular(ILightSceneNode *light)
  {
    return _IntColor(light->getLightData().SpecularColor.toSColor());
  }

  EXPORT void CALL SetLightCastShadows(ILightSceneNode *light, bool_t enable)
  {
    light->enableCastShadow(enable);
  }

  EXPORT bool_t CALL LightCastShadows(ILightSceneNode *light)
  {
    return light->getCastShadow();
  }

  EXPORT void CALL SetLightRadius(ILightSceneNode *light, float radius)
  {
    if (radius <= 0.f)
      return;
    light->setRadius(radius);
    light->getLightData().Attenuation = vector3df(1.f, 0.f, QuadraticFromRadius(radius));
  }

  EXPORT float CALL LightRadius(ILightSceneNode *light)
  {
    return light->getRadius();
  }

  EXPORT void CALL SetLightAttenuation(ILightSceneNode *light, float constant, float linear, float quadratic)
  {
    const float radius = RadiusFromAttenuation(constant, linear, quadratic);
    if (radius > 0.f)
      light->setRadius(radius);
    light->getLightData().Attenuation = vector3df(constant, linear, quadratic);
  }

  EXPORT float CALL LightConstantAttenuation(ILightSceneNode *light)
  {
    return light->getLightData().Attenuation.X;
  }

  EXPORT float CALL LightLinearAttenuation(ILightSceneNode *light)
  {
    return light->getLightData().Attenuation.Y;
  }

  EXPORT float CALL LightQuadraticAttenuation(ILightSceneNode *light)
  {
    return light->getLightData().Attenuation.Z;
  }

  EXPORT void CALL SetLightCone(ILightSceneNode *light, float inner, float outer)
  {
    light->getLightData().InnerCone = inner;
    light->getLightData().OuterCone = outer;
  }

  EXPORT float CALL LightInnerCone(ILightSceneNode *light)
  {
    return light->getLightData().InnerCone;
  }

  EXPORT float CALL LightOuterCone(ILightSceneNode *light)
  {
    return light->getLightData().OuterCone;
  }

  EXPORT void CALL SetLightFalloff(ILightSceneNode *light, float falloff)
  {
    light->getLightData().Falloff = falloff;
  }

  EXPORT float CALL LightFalloff(ILightSceneNode *light)
  {
    return light->getLightData().Falloff;
  }

} // extern "C"
