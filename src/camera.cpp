#include "camera.h"
#include "core.h"
#include "math.h"
#include "world.h"

extern "C"
{

  EXPORT ICameraSceneNode *CALL CreateCamera()
  {
    ICameraSceneNode *cam = _Device()->getSceneManager()->addCameraSceneNode();
    cam->setAspectRatio(0);
    _AddCamera(cam);
    return cam;
  }

  EXPORT void CALL SetCameraActive(ICameraSceneNode *cam, bool_t active)
  {
    _CameraData(cam)->active = active;
  }

  EXPORT bool_t CALL CameraActive(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->active;
  }

  EXPORT void CALL SetCameraViewport(ICameraSceneNode *cam, int x, int y, int width, int height)
  {
    _CameraData(cam)->viewport = recti(position2di(x, y), dimension2di(width, height));
  }

  EXPORT int CALL CameraViewportX(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->viewport.UpperLeftCorner.X;
  }

  EXPORT int CALL CameraViewportY(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->viewport.UpperLeftCorner.Y;
  }

  EXPORT int CALL CameraViewportWidth(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->viewport.getWidth();
  }

  EXPORT int CALL CameraViewportHeight(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->viewport.getHeight();
  }

  EXPORT void CALL SetCameraClearMode(ICameraSceneNode *cam, bool_t clear_color, bool_t clear_depth)
  {
    int flags = 0;
    if (clear_color)
      flags |= 1;
    if (clear_depth)
      flags |= 2;
    _CameraData(cam)->clearFlags = flags;
  }

  EXPORT bool_t CALL CameraClearColorEnabled(ICameraSceneNode *cam)
  {
    return (_CameraData(cam)->clearFlags & 1) == 1;
  }

  EXPORT bool_t CALL CameraClearDepthEnabled(ICameraSceneNode *cam)
  {
    return (_CameraData(cam)->clearFlags & 2) == 2;
  }

  EXPORT void CALL SetCameraClearColor(ICameraSceneNode *cam, int color)
  {
    _CameraData(cam)->clearColor = color;
  }

  EXPORT int CALL CameraClearColor(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->clearColor;
  }

  EXPORT void CALL SetCameraRenderTarget(ICameraSceneNode *cam, ITexture *target)
  {
    _CameraData(cam)->renderTarget = target;
  }

  EXPORT ITexture *CALL CameraRenderTarget(ICameraSceneNode *cam)
  {
    return _CameraData(cam)->renderTarget;
  }

  EXPORT void CALL SetCameraRange(ICameraSceneNode *cam, float near_, float far_)
  {
    cam->setNearValue(near_);
    cam->setFarValue(far_);
  }

  EXPORT float CALL CameraNearRange(ICameraSceneNode *cam)
  {
    return cam->getNearValue();
  }

  EXPORT float CALL CameraFarRange(ICameraSceneNode *cam)
  {
    return cam->getFarValue();
  }

  EXPORT void CALL SetCameraAspectRatio(ICameraSceneNode *cam, float ratio)
  {
    cam->setAspectRatio(ratio);
  }

  EXPORT float CALL CameraAspectRatio(ICameraSceneNode *cam)
  {
    return cam->getAspectRatio();
  }

  EXPORT void CALL SetCameraFOV(ICameraSceneNode *cam, float fov)
  {
    cam->setFOV(Rad(fov));
  }

  EXPORT float CALL CameraFOV(ICameraSceneNode *cam)
  {
    return Deg(cam->getFOV());
  }

  EXPORT void CALL SetCameraOrtho(ICameraSceneNode *cam, float width, float height, float near_, float far_)
  {
    const matrix4 mat = matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_);
    cam->setProjectionMatrix(mat, true);
  }

  EXPORT ISceneNode *CALL PickEntity(ICameraSceneNode *camera, int x, int y, int group)
  {
    ICameraSceneNode *activeCam = _Device()->getSceneManager()->getActiveCamera();
    _Device()->getSceneManager()->setActiveCamera(camera);
    ISceneNode *picked = _Device()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(vector2di(x, y), group);
    _Device()->getSceneManager()->setActiveCamera(activeCam);
    return picked;
  }

} // extern "C"
