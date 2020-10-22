#include "core.h"
#include "graphics.h"
#include "math.h"
#include "world.h"


static ISceneNode* _skybox = NULL;
static ISceneNode* _skydome = NULL;


extern "C" {


EXPORT void CALL SetAmbient(int color) {
    _Device()->getSceneManager()->setAmbientLight(_Color(color));
}


EXPORT void CALL SetShadowColor(int color) {
    _Device()->getSceneManager()->setShadowColor(_Color(color));
}


EXPORT void CALL SetFog(int color, float near_, float far_) {
    _Device()->getVideoDriver()->setFog(_Color(color), EFT_FOG_LINEAR, near_, far_);
}


EXPORT void CALL SetSkybox(ITexture* top, ITexture* bottom, ITexture* left, ITexture* right, ITexture* front, ITexture* back) {
    if (_skybox) {
        _skybox->remove();
        _skybox = NULL;
    }
    if (top) {
        _skybox = _Device()->getSceneManager()->addSkyBoxSceneNode(top, bottom, left, right, front, back);
        _skybox->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
        _skybox->setMaterialFlag(EMF_TEXTURE_WRAP, false);
    }
}


EXPORT void CALL SetSkydome(ITexture* tex, float texScale, bool_t fullSphere) {
    if (_skydome) {
        _skydome->remove();
        _skydome = NULL;
    }
    if (tex) {
        _skydome = _Device()->getSceneManager()->addSkyDomeSceneNode(tex, 16, 8, texScale, fullSphere ? 2 : 1);
        _skydome->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    }
}


EXPORT SMaterial* CALL SkyboxMaterial() {
    return (_skybox != NULL) ? &_skybox->getMaterial(0) : NULL;
}


EXPORT SMaterial* CALL SkydomeMaterial() {
    return (_skydome != NULL) ? &_skydome->getMaterial(0) : NULL;
}


EXPORT void CALL DrawWorld(ICameraSceneNode* camera) {
    const recti& viewport = _Device()->getVideoDriver()->getViewPort();
    vector3df dest(0, 0, 100);
    matrix4 matrix = camera->getAbsoluteTransformation();
    matrix.transformVect(dest);
    camera->setTarget(dest);
    camera->setAspectRatio(viewport.getWidth() * 1.0f / viewport.getHeight());
    _Device()->getSceneManager()->setActiveCamera(camera);
    _Device()->getSceneManager()->drawAll();
}


EXPORT void CALL WorldToScreen(ICameraSceneNode* camera, float x, float y, float z) {
    const vector2di coords = _Device()->getSceneManager()->getSceneCollisionManager()
        ->getScreenCoordinatesFrom3DPosition(vector3df(x, y, z), camera);
    _SetPoint(coords.X, coords.Y, 0);
}


EXPORT void CALL ScreenToWorld(ICameraSceneNode* camera, int x, int y, float depth) {
    const line3df line = _Device()->getSceneManager()->getSceneCollisionManager()
        ->getRayFromScreenCoordinates(vector2di(x, y), camera);
    const vector3df coords = lerp(line.start, line.end, depth);
    _SetPoint(coords.X, coords.Y, coords.Z);
}


EXPORT ISceneNode* CALL Raycast(float x1, float y1, float z1, float x2, float y2, float z2, int group) {
    vector3df position;
    triangle3df triangle;
    ISceneNode* entity = _Device()->getSceneManager()->getSceneCollisionManager()
        ->getSceneNodeAndCollisionPointFromRay(
            line3df(x1, y1, z1, x2, y2, z2),
            position,
            triangle,
            group);
    const vector3df normal = triangle.getNormal();
    _SetPoint(position.X, position.Y, position.Z);
    _SetNormal(normal.X, normal.Y, normal.Z);
    return entity;
}


} // extern "C"
