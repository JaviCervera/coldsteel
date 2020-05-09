#include "camera.h"
#include "core.h"
#include "math.h"

extern "C" {


EXPORT ICameraSceneNode* CALL asCreateCamera() {
    return _asDevice()->getSceneManager()->addCameraSceneNode();
}


EXPORT void CALL asSetCameraRange(ICameraSceneNode* cam, float near_, float far_) {
    cam->setNearValue(near_);
    cam->setFarValue(far_);
}


EXPORT float CALL asCameraNearRange(ICameraSceneNode* cam) {
    return cam->getNearValue();
}


EXPORT float CALL asCameraFarRange(ICameraSceneNode* cam) {
    return cam->getFarValue();
}


EXPORT void CALL asSetCameraFOV(ICameraSceneNode* cam, float fov) {
    cam->setFOV(asRad(fov));
}


EXPORT float CALL asCameraFOV(ICameraSceneNode* cam) {
    return asDeg(cam->getFOV());
}


EXPORT void CALL asSetCameraProjection(ICameraSceneNode* cam, float width, float height, float near_, float far_, bool_t ortho) {
    const matrix4 mat = (ortho)
        ? matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_)
        : matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_);
    cam->setProjectionMatrix(mat);
}


EXPORT ISceneNode* CALL asPickEntity(ICameraSceneNode* camera, int x, int y, int group) {
    ICameraSceneNode* activeCam = _asDevice()->getSceneManager()->getActiveCamera();
    _asDevice()->getSceneManager()->setActiveCamera(camera);
    ISceneNode* picked = _asDevice()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(vector2di(x, y), group);
    _asDevice()->getSceneManager()->setActiveCamera(activeCam);
    return picked;
}


} // extern "C"
