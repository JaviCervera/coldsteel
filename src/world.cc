#include "core.h"
#include "graphics.h"
#include "math.h"
#include "world.h"


static ISceneNode* _skybox = NULL;
static ISceneNode* _skydome = NULL;


extern "C" {


EXPORT void CALL asSetAmbient(int color) {
    _asDevice()->getSceneManager()->setAmbientLight(_asColor(color));
}


EXPORT void CALL asSetShadowColor(int color) {
    _asDevice()->getSceneManager()->setShadowColor(_asColor(color));
}


EXPORT void CALL asSetFog(int color, float near_, float far_) {
    _asDevice()->getVideoDriver()->setFog(_asColor(color), EFT_FOG_LINEAR, near_, far_);
}


EXPORT void CALL asSetSkybox(ITexture* top, ITexture* bottom, ITexture* left, ITexture* right, ITexture* front, ITexture* back) {
    if (_skybox) {
        _skybox->remove();
        _skybox = NULL;
    }
    if (top) {
        _skybox = _asDevice()->getSceneManager()->addSkyBoxSceneNode(top, bottom, left, right, front, back);
        _skybox->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
        _skybox->setMaterialFlag(EMF_TEXTURE_WRAP, false);
    }
}


EXPORT void CALL asSetSkydome(ITexture* tex, float texScale, bool_t fullSphere) {
    if (_skydome) {
        _skydome->remove();
        _skydome = NULL;
    }
    if (tex) {
        _skydome = _asDevice()->getSceneManager()->addSkyDomeSceneNode(tex, 16, 8, texScale, fullSphere ? 2 : 1);
        _skydome->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    }
}


EXPORT SMaterial* CALL asSkyboxMaterial() {
    return (_skybox != NULL) ? &_skybox->getMaterial(0) : NULL;
}


EXPORT SMaterial* CALL asSkydomeMaterial() {
    return (_skydome != NULL) ? &_skydome->getMaterial(0) : NULL;
}


EXPORT void CALL asDrawWorld(ICameraSceneNode* camera) {
    const recti& viewport = _asDevice()->getVideoDriver()->getViewPort();
    vector3df dest(0, 0, 100);
    matrix4 matrix = camera->getAbsoluteTransformation();
    matrix.transformVect(dest);
    camera->setTarget(dest);
    camera->setAspectRatio(viewport.getWidth() * 1.0f / viewport.getHeight());
    _asDevice()->getSceneManager()->setActiveCamera(camera);
    _asDevice()->getSceneManager()->drawAll();
}


EXPORT void CALL asWorldToScreen(ICameraSceneNode* camera, float x, float y, float z) {
    const vector2di coords = _asDevice()->getSceneManager()->getSceneCollisionManager()
        ->getScreenCoordinatesFrom3DPosition(vector3df(x, y, z), camera);
    _asSetPoint(coords.X, coords.Y, 0);
}


EXPORT void CALL asScreenToWorld(ICameraSceneNode* camera, int x, int y, float depth) {
    const line3df line = _asDevice()->getSceneManager()->getSceneCollisionManager()
        ->getRayFromScreenCoordinates(vector2di(x, y), camera);
    const vector3df coords = lerp(line.start, line.end, depth);
    _asSetPoint(coords.X, coords.Y, coords.Z);
}


EXPORT ISceneNode* CALL asRaycast(float x1, float y1, float z1, float x2, float y2, float z2, int group) {
    vector3df position;
    triangle3df triangle;
    ISceneNode* entity = _asDevice()->getSceneManager()->getSceneCollisionManager()
        ->getSceneNodeAndCollisionPointFromRay(
            line3df(x1, y1, z1, x2, y2, z2),
            position,
            triangle,
            group);
    const vector3df normal = triangle.getNormal();
    _asSetPoint(position.X, position.Y, position.Z);
    _asSetNormal(normal.X, normal.Y, normal.Z);
    return entity;
}


} // extern "C"
