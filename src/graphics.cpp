#include "color.h"
#include "core.h"
#include "graphics.h"
#include "math.h"

extern "C"
{

  EXPORT void CALL SetRenderTarget(ITexture *tex, bool_t clear, bool_t clearDepth, int color)
  {
    _Device()->getVideoDriver()->setRenderTarget(tex, clear, clearDepth, _Color(color));
  }

  EXPORT void CALL SetViewport(int x, int y, int width, int height)
  {
    _Device()->getVideoDriver()->setViewPort(recti(x, y, width + x, height + y));
  }

  EXPORT void CALL DrawPoint(int x, int y, int color)
  {
    DrawLine(x, y, x, y, color);
  }

  EXPORT void CALL DrawLine(int x1, int y1, int x2, int y2, int color)
  {
    _Device()->getVideoDriver()->draw2DLine(vector2di(x1, y1), vector2di(x2, y2), _Color(color));
  }

  EXPORT void CALL DrawRect(int x, int y, int width, int height, int color)
  {
    _Device()->getVideoDriver()->draw2DRectangle(_Color(color), recti(x, y, x + width, y + height));
  }

  EXPORT void CALL Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color)
  {
    SMaterial mat;
    mat.setFlag(EMF_LIGHTING, false);
    _Device()->getVideoDriver()->setTransform(ETS_WORLD, IdentityMatrix);
    _Device()->getVideoDriver()->setMaterial(mat);
    _Device()->getVideoDriver()->draw3DLine(vector3df(x1, y1, z1), vector3df(x2, y2, z2), _Color(color));
  }

} // extern "C"
