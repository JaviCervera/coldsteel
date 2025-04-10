#include "core.h"
#include "graphics.h"
#include "math.h"
#include "screen.h"
#include "world.h"

typedef map<ICameraSceneNode *, CameraData> CameraDataMap;

static ISceneNode *_skybox = NULL;
static ISceneNode *_skydome = NULL;
static bool _hasSetAmbient = false;
static array<ICameraSceneNode *> _cameras;
static CameraDataMap _cameraDatas;

extern "C"
{

  EXPORT void CALL SetAmbient(int color)
  {
    _Device()->getSceneManager()->setAmbientLight(_Color(color));
    _hasSetAmbient = true;
  }

  EXPORT void CALL SetShadowColor(int color)
  {
    _Device()->getSceneManager()->setShadowColor(_Color(color));
  }

  EXPORT void CALL SetFog(int color, float near_, float far_)
  {
    _Device()->getVideoDriver()->setFog(_Color(color), EFT_FOG_LINEAR, near_, far_);
  }

  EXPORT void CALL SetSkybox(ITexture *top, ITexture *bottom, ITexture *left, ITexture *right, ITexture *front, ITexture *back)
  {
    if (_skybox)
    {
      _skybox->remove();
      _skybox = NULL;
    }
    if (top)
    {
      _skybox = _Device()->getSceneManager()->addSkyBoxSceneNode(top, bottom, left, right, front, back);
      _skybox->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
      _skybox->setMaterialFlag(EMF_TEXTURE_WRAP, false);
    }
  }

  EXPORT void CALL SetSkydome(ITexture *tex, float texScale, bool_t fullSphere)
  {
    if (_skydome)
    {
      _skydome->remove();
      _skydome = NULL;
    }
    if (tex)
    {
      _skydome = _Device()->getSceneManager()->addSkyDomeSceneNode(tex, 16, 8, texScale, fullSphere ? 2 : 1);
      _skydome->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    }
  }

  EXPORT SMaterial *CALL SkyboxMaterial()
  {
    return (_skybox != NULL) ? &_skybox->getMaterial(0) : NULL;
  }

  EXPORT SMaterial *CALL SkydomeMaterial()
  {
    return (_skydome != NULL) ? &_skydome->getMaterial(0) : NULL;
  }

  EXPORT void CALL DrawWorld()
  {
    const recti prev_viewport = _Device()->getVideoDriver()->getViewPort();
    for (u32 i = 0; i < _cameras.size(); ++i)
    {
      ICameraSceneNode *camera = _cameras[i];
      CameraData *data = _CameraData(camera);
      if (!data->active || !camera->isVisible())
        continue;
      const float old_ratio = camera->getAspectRatio();

      // Set look point
      vector3df dest(0, 0, 100);
      matrix4 matrix = camera->getAbsoluteTransformation();
      matrix.transformVect(dest);
      camera->setTarget(dest);

      // Set viewport
      recti viewport = data->viewport;
      if (viewport.getWidth() == 0)
        viewport.LowerRightCorner.X = ScreenWidth() - 1;
      if (viewport.getHeight() == 0)
        viewport.LowerRightCorner.Y = ScreenHeight() - 1;
      _Device()->getVideoDriver()->setViewPort(viewport);

      // Set render target & clear buffers
      _Device()->getVideoDriver()->setRenderTarget(data->renderTarget, false, false);
      if (data->clearFlags & 1)
        _Device()->getVideoDriver()->draw2DRectangle(
          _Color(data->clearColor),
          recti(position2di(0, 0), _Device()->getVideoDriver()->getScreenSize()));
      if (data->clearFlags & 2)
        _Device()->getVideoDriver()->clearZBuffer();

      // Set aspect ratio
      if (!camera->isOrthogonal() && camera->getAspectRatio() <= 0)
        camera->setAspectRatio(viewport.getWidth() * 1.0f / viewport.getHeight());

      // Draw
      _Device()->getSceneManager()->setActiveCamera(camera);
      _Device()->getSceneManager()->drawAll();

      // Restore aspect ratio
      camera->setAspectRatio(old_ratio);
    }
    _Device()->getVideoDriver()->setViewPort(prev_viewport);
    _Device()->getVideoDriver()->setRenderTarget(NULL, false, false);
  }

  EXPORT void CALL WorldToScreen(ICameraSceneNode *camera, float x, float y, float z)
  {
    const vector2di coords = _Device()->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(vector3df(x, y, z), camera);
    _SetPoint(coords.X, coords.Y, 0);
  }

  EXPORT void CALL ScreenToWorld(ICameraSceneNode *camera, int x, int y, float depth)
  {
    const line3df line = _Device()->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(vector2di(x, y), camera);
    const vector3df coords = lerp(line.start, line.end, depth);
    _SetPoint(coords.X, coords.Y, coords.Z);
  }

  EXPORT ISceneNode *CALL Raycast(float x1, float y1, float z1, float x2, float y2, float z2, int group)
  {
    vector3df position;
    triangle3df triangle;
    ISceneNode *entity = _Device()->getSceneManager()->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(
        line3df(x1, y1, z1, x2, y2, z2),
        position,
        triangle,
        group);
    const vector3df normal = triangle.getNormal().normalize();
    _SetPoint(position.X, position.Y, position.Z);
    _SetNormal(normal.X, normal.Y, normal.Z);
    return entity;
  }

  bool _HasSetAmbient()
  {
    return _hasSetAmbient;
  }

  void _AddCamera(ICameraSceneNode *camera)
  {
    _cameras.push_back(camera);
    _cameraDatas.set(camera, CameraData(true, recti(), 3, RGB(0, 0, 64), NULL));
  }

  void _RemoveCamera(ICameraSceneNode *camera)
  {
    for (u32 i = 0; i < _cameras.size(); ++i)
    {
      if (_cameras[i] == camera)
      {
        _cameraDatas.remove(_cameras[i]);
        _cameras.erase(i);
        return;
      }
    }
  }

  CameraData *_CameraData(ICameraSceneNode *camera)
  {
    CameraDataMap::Node *node = _cameraDatas.find(camera);
    return node ? &node->getValue() : NULL;
  }

} // extern "C"
