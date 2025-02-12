#include "core.h"
#include "graphics.h"
#include "math.h"

int RGB(int red, int green, int blue)
{
  return RGBA(red, green, blue, 255);
}

int RGBA(int red, int green, int blue, int alpha)
{
  return (alpha << 24) + (red << 16) + (green << 8) + blue;
}

int RedVal(int color)
{
  return (color & 0x00FF0000) >> 16;
}

int GreenVal(int color)
{
  return (color & 0x0000FF00) >> 8;
}

int BlueVal(int color)
{
  return (color & 0x000000FF);
}

int AlphaVal(int color)
{
  return (color & 0xFF000000) >> 24;
}

void BeginDrawing(bool_t clear, bool_t clearDepth, int color)
{
  _Device()->getVideoDriver()->beginScene(clear, clearDepth, _Color(color));
}

void EndDrawing()
{
  _Device()->getVideoDriver()->endScene();
}

void SetRenderTarget(ITexture *tex, bool_t clear, bool_t clearDepth, int color)
{
  _Device()->getVideoDriver()->setRenderTarget(tex, clear, clearDepth, _Color(color));
}

void SetViewport(int x, int y, int width, int height)
{
  _Device()->getVideoDriver()->setViewPort(recti(x, y, width + x, height + y));
}

void DrawPoint(int x, int y, int color)
{
  DrawLine(x, y, x, y, color);
}

void DrawLine(int x1, int y1, int x2, int y2, int color)
{
  _Device()->getVideoDriver()->draw2DLine(vector2di(x1, y1), vector2di(x2, y2), _Color(color));
}

void DrawRect(int x, int y, int width, int height, int color)
{
  _Device()->getVideoDriver()->draw2DRectangle(_Color(color), recti(x, y, x + width, y + height));
}

void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color)
{
  SMaterial mat;
  mat.setFlag(EMF_LIGHTING, false);
  _Device()->getVideoDriver()->setTransform(ETS_WORLD, IdentityMatrix);
  _Device()->getVideoDriver()->setMaterial(mat);
  _Device()->getVideoDriver()->draw3DLine(vector3df(x1, y1, z1), vector3df(x2, y2, z2), _Color(color));
}

SColor _Color(int color)
{
  return SColor(AlphaVal(color), RedVal(color), GreenVal(color), BlueVal(color));
}

int _IntColor(const SColor &color)
{
  return RGBA(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}
