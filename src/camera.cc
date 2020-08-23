#include "camera.h"
#include "core.h"
#include "math.h"

extern "C" {


EXPORT ICameraSceneNode* CALL csCreateCamera() {
    return _csDevice()->getSceneManager()->addCameraSceneNode();
}


EXPORT void CALL csSetCameraRange(ICameraSceneNode* cam, float near_, float far_) {
    cam->setNearValue(near_);
    cam->setFarValue(far_);
}


EXPORT float CALL csCameraNearRange(ICameraSceneNode* cam) {
    return cam->getNearValue();
}


EXPORT float CALL csCameraFarRange(ICameraSceneNode* cam) {
    return cam->getFarValue();
}


EXPORT void CALL csSetCameraFOV(ICameraSceneNode* cam, float fov) {
    cam->setFOV(csRad(fov));
}


EXPORT float CALL csCameraFOV(ICameraSceneNode* cam) {
    return csDeg(cam->getFOV());
}


EXPORT void CALL csSetCameraProjection(ICameraSceneNode* cam, float width, float height, float near_, float far_, bool_t ortho) {
    const matrix4 mat = (ortho)
        ? matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_)
        : matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_);
    cam->setProjectionMatrix(mat);
}


EXPORT ISceneNode* CALL csPickEntity(ICameraSceneNode* camera, int x, int y, int group) {
    ICameraSceneNode* activeCam = _csDevice()->getSceneManager()->getActiveCamera();
    _csDevice()->getSceneManager()->setActiveCamera(camera);
    ISceneNode* picked = _csDevice()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(vector2di(x, y), group);
    _csDevice()->getSceneManager()->setActiveCamera(activeCam);
    return picked;
}


} // extern "C"
