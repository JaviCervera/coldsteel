/** @file
 * Functions for creating and configuring scene lights. Supports directional, point, and spot
 * lights (LIGHT_* constants).
 */
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

  /**
   * Creates a new light.
   *
   * @param type The light type as one of the LIGHT_* constants.
   * @return A new light.
   */
  EXPORT ILightSceneNode *CALL CreateLight(int type);

  /**
   * Returns the type of a light.
   *
   * @param light The light to query.
   * @return The light type.
   */
  EXPORT int CALL LightType(ILightSceneNode *light);

  /**
   * Sets the diffuse color of a light.
   *
   * @param light The light whose diffuse color will be set.
   * @param color The new diffuse color.
   */
  EXPORT void CALL SetLightDiffuse(ILightSceneNode *light, int color);

  /**
   * Returns the diffuse color of a light.
   *
   * @param light The light to query.
   * @return The diffuse color.
   */
  EXPORT int CALL LightDiffuse(ILightSceneNode *light);

  /**
   * Sets the ambient color contribution of a light.
   *
   * @param light The light whose ambient color will be set.
   * @param color The new ambient color.
   */
  EXPORT void CALL SetLightAmbient(ILightSceneNode *light, int color);

  /**
   * Returns the ambient color of a light.
   *
   * @param light The light to query.
   * @return The ambient color.
   */
  EXPORT int CALL LightAmbient(ILightSceneNode *light);

  /**
   * Sets the specular color of a light.
   *
   * @param light The light whose specular color will be set.
   * @param color The new specular color.
   */
  EXPORT void CALL SetLightSpecular(ILightSceneNode *light, int color);

  /**
   * Returns the specular color of a light.
   *
   * @param light The light to query.
   * @return The specular color.
   */
  EXPORT int CALL LightSpecular(ILightSceneNode *light);

  /**
   * Enables or disables shadow casting for a light.
   *
   * @param light The light whose shadow casting state will be set.
   * @param enable The new shadow casting state.
   */
  EXPORT void CALL SetLightCastShadows(ILightSceneNode *light, bool_t enable);

  /**
   * Returns whether a light casts shadows.
   *
   * @param light The light to query.
   * @return True if shadow casting is enabled.
   */
  EXPORT bool_t CALL LightCastShadows(ILightSceneNode *light);

  /**
   * Sets the influence radius of a light.
   *
   * @param light The light whose radius will be set.
   * @param radius The new radius.
   */
  EXPORT void CALL SetLightRadius(ILightSceneNode *light, float radius);

  /**
   * Returns the influence radius of a light.
   *
   * @param light The light to query.
   * @return The light radius.
   */
  EXPORT float CALL LightRadius(ILightSceneNode *light);

  /**
   * Sets the attenuation factors of a light.
   *
   * @param light The light whose attenuation will be set.
   * @param constant The constant attenuation factor.
   * @param linear The linear attenuation factor.
   * @param quadratic The quadratic attenuation factor.
   */
  EXPORT void CALL SetLightAttenuation(ILightSceneNode *light, float constant, float linear, float quadratic);

  /**
   * Returns the constant attenuation factor of a light.
   *
   * @param light The light to query.
   * @return The constant attenuation factor.
   */
  EXPORT float CALL LightConstantAttenuation(ILightSceneNode *light);

  /**
   * Returns the linear attenuation factor of a light.
   *
   * @param light The light to query.
   * @return The linear attenuation factor.
   */
  EXPORT float CALL LightLinearAttenuation(ILightSceneNode *light);

  /**
   * Returns the quadratic attenuation factor of a light.
   *
   * @param light The light to query.
   * @return The quadratic attenuation factor.
   */
  EXPORT float CALL LightQuadraticAttenuation(ILightSceneNode *light);

  /**
   * Sets the inner and outer cone angles of a spot light.
   *
   * @param light The light whose cone angles will be set.
   * @param inner The inner cone angle in degrees.
   * @param outer The outer cone angle in degrees.
   */
  EXPORT void CALL SetLightCone(ILightSceneNode *light, float inner, float outer);

  /**
   * Returns the inner cone angle of a spot light.
   *
   * @param light The light to query.
   * @return The inner cone angle in degrees.
   */
  EXPORT float CALL LightInnerCone(ILightSceneNode *light);

  /**
   * Returns the outer cone angle of a spot light.
   *
   * @param light The light to query.
   * @return The outer cone angle in degrees.
   */
  EXPORT float CALL LightOuterCone(ILightSceneNode *light);

  /**
   * Sets the falloff exponent of a spot light.
   *
   * @param light The light whose falloff will be set.
   * @param falloff The new falloff exponent.
   */
  EXPORT void CALL SetLightFalloff(ILightSceneNode *light, float falloff);

  /**
   * Returns the falloff exponent of a spot light.
   *
   * @param light The light to query.
   * @return The falloff exponent.
   */
  EXPORT float CALL LightFalloff(ILightSceneNode *light);

#ifdef __cplusplus
} /* extern "C" */
#endif
