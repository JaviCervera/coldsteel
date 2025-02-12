#include "camera.h"
#include "core.h"
#include "math.h"

ICameraSceneNode *CreateCamera()
{
  return _Device()->getSceneManager()->addCameraSceneNode();
}

void SetCameraRange(ICameraSceneNode *cam, float near_, float far_)
{
  cam->setNearValue(near_);
  cam->setFarValue(far_);
}

float CameraNearRange(ICameraSceneNode *cam)
{
  return cam->getNearValue();
}

float CameraFarRange(ICameraSceneNode *cam)
{
  return cam->getFarValue();
}

void SetCameraFOV(ICameraSceneNode *cam, float fov)
{
  cam->setFOV(Rad(fov));
}

float CameraFOV(ICameraSceneNode *cam)
{
  return Deg(cam->getFOV());
}

void SetCameraOrtho(ICameraSceneNode *cam, float width, float height, float near_, float far_)
{
  const matrix4 mat = matrix4().buildProjectionMatrixOrthoLH(width, height, near_, far_);
  cam->setProjectionMatrix(mat, true);
}

ISceneNode *PickEntity(ICameraSceneNode *camera, int x, int y, int group)
{
  ICameraSceneNode *activeCam = _Device()->getSceneManager()->getActiveCamera();
  _Device()->getSceneManager()->setActiveCamera(camera);
  ISceneNode *picked = _Device()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(vector2di(x, y), group);
  _Device()->getSceneManager()->setActiveCamera(activeCam);
  return picked;
}
