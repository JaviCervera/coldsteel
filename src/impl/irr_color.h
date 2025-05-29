#include "../color.h"

inline irr::video::SColor IrrColor(int color)
{
  return irr::video::SColor(AlphaVal(color), RedVal(color), GreenVal(color), BlueVal(color));
}

inline int IntColor(const irr::video::SColor &color)
{
  return RGBA(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}
