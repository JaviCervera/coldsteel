#include "color.h"
#include "math.h"

extern "C"
{

  EXPORT int CALL RGB(int red, int green, int blue)
  {
    return RGBA(red, green, blue, 255);
  }

  EXPORT int CALL RGBA(int red, int green, int blue, int alpha)
  {
    return (alpha << 24) + (red << 16) + (green << 8) + blue;
  }

  EXPORT int CALL RGBF(float red, float green, float blue)
  {
    return RGBAF(red, green, blue, 1);
  }

  EXPORT int CALL RGBAF(float red, float green, float blue, float alpha)
  {
    red = Clamp(red, 0, 1);
    green = Clamp(green, 0, 1);
    blue = Clamp(blue, 0, 1);
    alpha = Clamp(alpha, 0, 1);
    return RGBA(int(red * 255), int(green * 255), int(blue * 255), int(alpha * 255));
  }

  EXPORT int CALL RedVal(int color)
  {
    return (color & 0x00FF0000) >> 16;
  }

  EXPORT int CALL GreenVal(int color)
  {
    return (color & 0x0000FF00) >> 8;
  }

  EXPORT int CALL BlueVal(int color)
  {
    return (color & 0x000000FF);
  }

  EXPORT int CALL AlphaVal(int color)
  {
    return (color & 0xFF000000) >> 24;
  }

  EXPORT float CALL RedValF(int color)
  {
    return RedVal(color) / 255.0f;
  }

  EXPORT float CALL GreenValF(int color)
  {
    return GreenVal(color) / 255.0f;
  }

  EXPORT float CALL BlueValF(int color)
  {
    return BlueVal(color) / 255.0f;
  }

  EXPORT float CALL AlphaValF(int color)
  {
    return AlphaVal(color) / 255.0f;
  }

  EXPORT int CALL FadeColor(int color, int new_alpha)
  {
    return (int(Clamp(new_alpha, 0, 255)) << 24) | (color & 0x00ffffff);
  }

  EXPORT int CALL MultiplyColor(int color, float factor)
  {
    return RGBAF(RedValF(color) * factor, GreenValF(color) * factor, BlueValF(color) * factor, AlphaValF(color));
  }

  EXPORT int CALL SwapColor(int color)
  {
    return RGBA(BlueVal(color), GreenVal(color), RedVal(color), AlphaVal(color));
  }

} // extern "C"

SColor _Color(int color)
{
  return SColor(AlphaVal(color), RedVal(color), GreenVal(color), BlueVal(color));
}

int _IntColor(const SColor &color)
{
  return RGBA(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}
