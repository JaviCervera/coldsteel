#include <math.h>
#include "math.h"

#define DEG2RAD (0.017453292f)
#define RAD2DEG	(57.29577951f)


static vector3df _point;
static vector3df _normal;


extern "C" {


EXPORT float CALL Floor(float val) {
    return floor(val);
}


EXPORT float CALL Ceil(float val) {
    return ceil(val);
}


EXPORT float CALL Abs(float val) {
    return abs(val);
}


EXPORT float CALL Sqr(float val) {
    return sqrt(val);
}


EXPORT float CALL Sin(float val) {
    return sin(val * DEG2RAD);
}


EXPORT float CALL Cos(float val) {
    return cos(val * DEG2RAD);
}


EXPORT float CALL Tan(float val) {
    return tan(val * DEG2RAD);
}


EXPORT float CALL ASin(float val) {
    return asin(val * RAD2DEG);
}


EXPORT float CALL ACos(float val) {
    return acos(val * RAD2DEG);
}


EXPORT float CALL ATan(float val) {
    return atan(val * RAD2DEG);
}


EXPORT float CALL ATan2(float x, float y) {
    return atan2(x, y) * RAD2DEG;
}


EXPORT float CALL Exp(float val) {
    return exp(val);
}


EXPORT float CALL Pow(float base, float exp) {
    return pow(base, exp);
}


EXPORT float CALL Log(float val) {
    return log(val);
}


EXPORT float CALL Log10(float val) {
    return log10(val);
}


EXPORT float CALL Min(float a, float b) {
    return (a < b) ? a : b;
}


EXPORT float CALL Max(float a, float b) {
    return (a > b) ? a : b;
}


EXPORT float CALL Clamp(float val, float min, float max) {
    return Min(Max(val, min), max);
}


EXPORT int CALL BoolToInt(bool_t val) {
    return val ? 1 : 0;
}


EXPORT int CALL Int(float val) {
    return (int)val;
}


EXPORT int CALL Rand(int min, int max) {
    return (rand() % (max - min)) + min;
}


EXPORT void CALL RandSeed(int seed) {
    return srand(seed);
}


EXPORT float CALL Deg(float rad) {
    return rad * RAD2DEG;
}


EXPORT float CALL Rad(float deg) {
    return deg * DEG2RAD;
}


EXPORT float CALL Wrap(float val, float mod) {
    return (mod != 0)
        ? (val - mod*floor(val/mod))
        : val;
}


EXPORT float CALL PointX() {
    return _point.X;
}


EXPORT float CALL PointY() {
    return _point.Y;
}


EXPORT float CALL PointZ() {
    return _point.Z;
}


EXPORT float CALL NormalX() {
    return _normal.X;
}


EXPORT float CALL NormalY() {
    return _normal.Y;
}


EXPORT float CALL NormalZ() {
    return _normal.Z;
}


void _SetPoint(float x, float y, float z) {
    _point.set(x, y, z);
}


void _SetNormal(float x, float y, float z) {
    _normal.set(x, y, z);
}


} // extern "C"
