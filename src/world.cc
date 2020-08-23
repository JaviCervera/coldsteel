#include "core.h"
#include "graphics.h"
#include "math.h"
#include "world.h"


static ISceneNode* _skybox = NULL;
static ISceneNode* _skydome = NULL;


extern "C" {


EXPORT void CALL csSetAmbient(int color) {
    _csDevice()->getSceneManager()->setAmbientLight(_csColor(color));
}


EXPORT void CALL csSetShadowColor(int color) {
    _csDevice()->getSceneManager()->setShadowColor(_csColor(color));
}


EXPORT void CALL csSetFog(int color, float near_, float far_) {
    _csDevice()->getVideoDriver()->setFog(_csColor(color), EFT_FOG_LINEAR, near_, far_);
}


EXPORT void CALL csSetSkybox(ITexture* top, ITexture* bottom, ITexture* left, ITexture* right, ITexture* front, ITexture* back) {
    if (_skybox) {
        _skybox->remove();
        _skybox = NULL;
    }
    if (top) {
        _skybox = _csDevice()->getSceneManager()->addSkyBoxSceneNode(top, bottom, left, right, front, back);
        _skybox->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
        _skybox->setMaterialFlag(EMF_TEXTURE_WRAP, false);
    }
}


EXPORT void CALL csSetSkydome(ITexture* tex, float texScale, bool_t fullSphere) {
    if (_skydome) {
        _skydome->remove();
        _skydome = NULL;
    }
    if (tex) {
        _skydome = _csDevice()->getSceneManager()->addSkyDomeSceneNode(tex, 16, 8, texScale, fullSphere ? 2 : 1);
        _skydome->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    }
}


EXPORT SMaterial* CALL csSkyboxMaterial() {
    return (_skybox != NULL) ? &_skybox->getMaterial(0) : NULL;
}


EXPORT SMaterial* CALL csSkydomeMaterial() {
    return (_skydome != NULL) ? &_skydome->getMaterial(0) : NULL;
}


EXPORT void CALL csDrawWorld(ICameraSceneNode* camera) {
    const recti& viewport = _csDevice()->getVideoDriver()->getViewPort();
    vector3df dest(0, 0, 100);
    matrix4 matrix = camera->getAbsoluteTransformation();
    matrix.transformVect(dest);
    camera->setTarget(dest);
    camera->setAspectRatio(viewport.getWidth() * 1.0f / viewport.getHeight());
    _csDevice()->getSceneManager()->setActiveCamera(camera);
    _csDevice()->getSceneManager()->drawAll();
}


EXPORT void CALL csWorldToScreen(ICameraSceneNode* camera, float x, float y, float z) {
    const vector2di coords = _csDevice()->getSceneManager()->getSceneCollisionManager()
        ->getScreenCoordinatesFrom3DPosition(vector3df(x, y, z), camera);
    _csSetPoint(coords.X, coords.Y, 0);
}


EXPORT void CALL csScreenToWorld(ICameraSceneNode* camera, int x, int y, float depth) {
    const line3df line = _csDevice()->getSceneManager()->getSceneCollisionManager()
        ->getRayFromScreenCoordinates(vector2di(x, y), camera);
    const vector3df coords = lerp(line.start, line.end, depth);
    _csSetPoint(coords.X, coords.Y, coords.Z);
}


EXPORT ISceneNode* CALL csRaycast(float x1, float y1, float z1, float x2, float y2, float z2, int group) {
    vector3df position;
    triangle3df triangle;
    ISceneNode* entity = _csDevice()->getSceneManager()->getSceneCollisionManager()
        ->getSceneNodeAndCollisionPointFromRay(
            line3df(x1, y1, z1, x2, y2, z2),
            position,
            triangle,
            group);
    const vector3df normal = triangle.getNormal();
    _csSetPoint(position.X, position.Y, position.Z);
    _csSetNormal(normal.X, normal.Y, normal.Z);
    return entity;
}


} // extern "C"
