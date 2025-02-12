#include <math.h>
#include "math.h"

#define DEG2RAD (0.017453292f)
#define RAD2DEG (57.29577951f)

static vector3df _point;
static vector3df _normal;

float Floor(float val)
{
  return floor(val);
}

float Ceil(float val)
{
  return ceil(val);
}

float Abs(float val)
{
  return abs(val);
}

float Sqr(float val)
{
  return sqrt(val);
}

float Sin(float val)
{
  return sin(val * DEG2RAD);
}

float Cos(float val)
{
  return cos(val * DEG2RAD);
}

float Tan(float val)
{
  return tan(val * DEG2RAD);
}

float ASin(float val)
{
  return asin(val * RAD2DEG);
}

float ACos(float val)
{
  return acos(val * RAD2DEG);
}

float ATan(float val)
{
  return atan(val * RAD2DEG);
}

float ATan2(float x, float y)
{
  return atan2(x, y) * RAD2DEG;
}

float Exp(float val)
{
  return exp(val);
}

float Pow(float base, float exp)
{
  return pow(base, exp);
}

float Log(float val)
{
  return log(val);
}

float Log10(float val)
{
  return log10(val);
}

float Min(float a, float b)
{
  return (a < b) ? a : b;
}

float Max(float a, float b)
{
  return (a > b) ? a : b;
}

float Clamp(float val, float min, float max)
{
  return Min(Max(val, min), max);
}

int BoolToInt(bool_t val)
{
  return val ? 1 : 0;
}

int Int(float val)
{
  return (int)val;
}

int Rand(int min, int max)
{
  return (rand() % (max - min)) + min;
}

void RandSeed(int seed)
{
  return srand(seed);
}

float Deg(float rad)
{
  return rad * RAD2DEG;
}

float Rad(float deg)
{
  return deg * DEG2RAD;
}

float Wrap(float val, float mod)
{
  return (mod != 0)
             ? (val - mod * floor(val / mod))
             : val;
}

float PointX()
{
  return _point.X;
}

float PointY()
{
  return _point.Y;
}

float PointZ()
{
  return _point.Z;
}

float NormalX()
{
  return _normal.X;
}

float NormalY()
{
  return _normal.Y;
}

float NormalZ()
{
  return _normal.Z;
}

void _SetPoint(float x, float y, float z)
{
  _point.set(x, y, z);
}

void _SetNormal(float x, float y, float z)
{
  _normal.set(x, y, z);
}
