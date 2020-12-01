#include "camera.h"
#include "core.h"
#include "math.h"

extern "C" {


EXPORT ICameraSceneNode* CALL CreateCamera() {
    return _Device()->getSceneManager()->addCameraSceneNode();
}


EXPORT void CALL SetCameraRange(ICameraSceneNode* cam, float near_, float far_) {
    cam->setNearValue(near_);
    cam->setFarValue(far_);
}


EXPORT float CALL CameraNearRange(ICameraSceneNode* cam) {
    return cam->getNearValue();
}


EXPORT float CALL CameraFarRange(ICameraSceneNode* cam) {
    return cam->getFarValue();
}


EXPORT void CALL SetCameraFOV(ICameraSceneNode* cam, float fov) {
    cam->setFOV(Rad(fov));
}


EXPORT float CALL CameraFOV(ICameraSceneNode* cam) {
    return Deg(cam->getFOV());
}


EXPORT void CALL SetCameraOrtho(ICameraSceneNode* cam, float width, float height, float near_, float far_) {
    const matrix4 mat = matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_);
    cam->setProjectionMatrix(mat, true);
}


EXPORT ISceneNode* CALL PickEntity(ICameraSceneNode* camera, int x, int y, int group) {
    ICameraSceneNode* activeCam = _Device()->getSceneManager()->getActiveCamera();
    _Device()->getSceneManager()->setActiveCamera(camera);
    ISceneNode* picked = _Device()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(vector2di(x, y), group);
    _Device()->getSceneManager()->setActiveCamera(activeCam);
    return picked;
}


} // extern "C"
