#include "core.h"
#include "graphics.h"
#include "light.h"

extern "C" {


EXPORT ILightSceneNode* CALL asCreateLight(int type) {
    map<int, E_LIGHT_TYPE> types;
    types[ASLIGHT_DIRECTIONAL] = ELT_DIRECTIONAL;
    types[ASLIGHT_POINT] = ELT_POINT;
    types[ASLIGHT_SPOT] = ELT_SPOT;
    ILightSceneNode* light = _asDevice()->getSceneManager()->addLightSceneNode();
    light->setLightType(types[type]);
    return light;
}


EXPORT int CALL asLightType(ILightSceneNode* light) {
    map<E_LIGHT_TYPE, int> types;
    types[ELT_DIRECTIONAL] = ASLIGHT_DIRECTIONAL;
    types[ELT_POINT] = ASLIGHT_POINT;
    types[ELT_SPOT] = ASLIGHT_SPOT;
    return types[light->getLightType()];
}


EXPORT void CALL asSetLightDiffuse(ILightSceneNode* light, int color) {
    light->getLightData().DiffuseColor = _asColor(color);
}


EXPORT int CALL asLightDiffuse(ILightSceneNode* light) {
    return _asIntColor(light->getLightData().DiffuseColor.toSColor());
}


EXPORT void CALL asSetLightAmbient(ILightSceneNode* light, int color) {
    light->getLightData().AmbientColor = _asColor(color);
}


EXPORT int CALL asLightAmbient(ILightSceneNode* light) {
    return _asIntColor(light->getLightData().AmbientColor.toSColor());
}


EXPORT void CALL asSetLightSpecular(ILightSceneNode* light, int color) {
    light->getLightData().SpecularColor = _asColor(color);
}


EXPORT int CALL asLightSpecular(ILightSceneNode* light) {
    return _asIntColor(light->getLightData().SpecularColor.toSColor());
}


EXPORT void CALL asSetLightCastShadows(ILightSceneNode* light, bool_t enable) {
    light->enableCastShadow(enable);
}


EXPORT bool_t CALL asLightCastShadows(ILightSceneNode* light) {
    return light->getCastShadow();
}


EXPORT void CALL asSetLightRadius(ILightSceneNode* light, float radius) {
    light->setRadius(radius);
}


EXPORT float CALL asLightRadius(ILightSceneNode* light) {
    return light->getRadius();
}


EXPORT void CALL asSetLightAttenuation(ILightSceneNode* light, float constant, float linear, float quadratic) {
    light->getLightData().Attenuation = vector3df(constant, linear, quadratic);
}


EXPORT float CALL asLightConstantAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.X;
}


EXPORT float CALL asLightLinearAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.Y;
}


EXPORT float CALL asLightQuadraticAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.Z;
}


EXPORT void CALL asSetLightCone(ILightSceneNode* light, float inner, float outer) {
    light->getLightData().InnerCone = inner;
    light->getLightData().OuterCone = outer;
}


EXPORT float CALL asLightInnerCone(ILightSceneNode* light) {
    return light->getLightData().InnerCone;
}


EXPORT float CALL asLightOuterCone(ILightSceneNode* light) {
    return light->getLightData().OuterCone;
}


EXPORT void CALL asSetLightFalloff(ILightSceneNode* light, float falloff) {
    light->getLightData().Falloff = falloff;
}


EXPORT float CALL asLightFalloff(ILightSceneNode* light) {
    return light->getLightData().Falloff;
}


} // extern "C"
