#include "core.h"
#include "graphics.h"
#include "light.h"

extern "C" {


EXPORT ILightSceneNode* CALL csCreateLight(int type) {
    map<int, E_LIGHT_TYPE> types;
    types[CSLIGHT_DIRECTIONAL] = ELT_DIRECTIONAL;
    types[CSLIGHT_POINT] = ELT_POINT;
    types[CSLIGHT_SPOT] = ELT_SPOT;
    ILightSceneNode* light = _csDevice()->getSceneManager()->addLightSceneNode();
    light->setLightType(types[type]);
    return light;
}


EXPORT int CALL csLightType(ILightSceneNode* light) {
    map<E_LIGHT_TYPE, int> types;
    types[ELT_DIRECTIONAL] = CSLIGHT_DIRECTIONAL;
    types[ELT_POINT] = CSLIGHT_POINT;
    types[ELT_SPOT] = CSLIGHT_SPOT;
    return types[light->getLightType()];
}


EXPORT void CALL csSetLightDiffuse(ILightSceneNode* light, int color) {
    light->getLightData().DiffuseColor = _csColor(color);
}


EXPORT int CALL csLightDiffuse(ILightSceneNode* light) {
    return _csIntColor(light->getLightData().DiffuseColor.toSColor());
}


EXPORT void CALL csSetLightAmbient(ILightSceneNode* light, int color) {
    light->getLightData().AmbientColor = _csColor(color);
}


EXPORT int CALL csLightAmbient(ILightSceneNode* light) {
    return _csIntColor(light->getLightData().AmbientColor.toSColor());
}


EXPORT void CALL csSetLightSpecular(ILightSceneNode* light, int color) {
    light->getLightData().SpecularColor = _csColor(color);
}


EXPORT int CALL csLightSpecular(ILightSceneNode* light) {
    return _csIntColor(light->getLightData().SpecularColor.toSColor());
}


EXPORT void CALL csSetLightCastShadows(ILightSceneNode* light, bool_t enable) {
    light->enableCastShadow(enable);
}


EXPORT bool_t CALL csLightCastShadows(ILightSceneNode* light) {
    return light->getCastShadow();
}


EXPORT void CALL csSetLightRadius(ILightSceneNode* light, float radius) {
    light->setRadius(radius);
}


EXPORT float CALL csLightRadius(ILightSceneNode* light) {
    return light->getRadius();
}


EXPORT void CALL csSetLightAttenuation(ILightSceneNode* light, float constant, float linear, float quadratic) {
    light->getLightData().Attenuation = vector3df(constant, linear, quadratic);
}


EXPORT float CALL csLightConstantAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.X;
}


EXPORT float CALL csLightLinearAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.Y;
}


EXPORT float CALL csLightQuadraticAttenuation(ILightSceneNode* light) {
    return light->getLightData().Attenuation.Z;
}


EXPORT void CALL csSetLightCone(ILightSceneNode* light, float inner, float outer) {
    light->getLightData().InnerCone = inner;
    light->getLightData().OuterCone = outer;
}


EXPORT float CALL csLightInnerCone(ILightSceneNode* light) {
    return light->getLightData().InnerCone;
}


EXPORT float CALL csLightOuterCone(ILightSceneNode* light) {
    return light->getLightData().OuterCone;
}


EXPORT void CALL csSetLightFalloff(ILightSceneNode* light, float falloff) {
    light->getLightData().Falloff = falloff;
}


EXPORT float CALL csLightFalloff(ILightSceneNode* light) {
    return light->getLightData().Falloff;
}


} // extern "C"
