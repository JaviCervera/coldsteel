#include <math.h>
#include "math.h"

#define DEG2RAD (0.017453292f)
#define RAD2DEG	(57.29577951f)


static vector3df _point;
static vector3df _normal;


extern "C" {


EXPORT float CALL csFloor(float val) {
    return floor(val);
}


EXPORT float CALL csCeil(float val) {
    return ceil(val);
}


EXPORT float CALL csAbs(float val) {
    return abs(val);
}


EXPORT float CALL csSqr(float val) {
    return sqrt(val);
}


EXPORT float CALL csSin(float val) {
    return sin(val * DEG2RAD);
}


EXPORT float CALL csCos(float val) {
    return cos(val * DEG2RAD);
}


EXPORT float CALL csTan(float val) {
    return tan(val * DEG2RAD);
}


EXPORT float CALL csASin(float val) {
    return asin(val * RAD2DEG);
}


EXPORT float CALL csACos(float val) {
    return acos(val * RAD2DEG);
}


EXPORT float CALL csATan(float val) {
    return atan(val * RAD2DEG);
}


EXPORT float CALL csATan2(float x, float y) {
    return atan2(x, y) * RAD2DEG;
}


EXPORT float CALL csExp(float val) {
    return exp(val);
}


EXPORT float CALL csLog(float val) {
    return log(val);
}


EXPORT float CALL csLog10(float val) {
    return log10(val);
}


EXPORT int CALL csRand(int min, int max) {
    return (rand() % (max - min)) + min;
}


EXPORT void CALL csRandSeed(int seed) {
    return srand(seed);
}


EXPORT float CALL csDeg(float rad) {
    return rad * RAD2DEG;
}


EXPORT float CALL csRad(float deg) {
    return deg * DEG2RAD;
}


EXPORT float CALL csWrap(float val, float mod) {
    return (mod != 0)
        ? (val - mod*floor(val/mod))
        : val;
}


EXPORT float CALL csPointX() {
    return _point.X;
}


EXPORT float CALL csPointY() {
    return _point.Y;
}


EXPORT float CALL csPointZ() {
    return _point.Z;
}


EXPORT float CALL csNormalX() {
    return _normal.X;
}


EXPORT float CALL csNormalY() {
    return _normal.Y;
}


EXPORT float CALL csNormalZ() {
    return _normal.Z;
}


void _csSetPoint(float x, float y, float z) {
    _point.set(x, y, z);
}


void _csSetNormal(float x, float y, float z) {
    _normal.set(x, y, z);
}


} // extern "C"
