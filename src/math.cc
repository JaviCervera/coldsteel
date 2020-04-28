#include <math.h>
#include "math.h"

#define DEG2RAD (0.017453292f)
#define RAD2DEG	(57.29577951f)


static vector3df _point;
static vector3df _normal;


extern "C" {


EXPORT float CALL asFloor(float val) {
    return floor(val);
}


EXPORT float CALL asCeil(float val) {
    return ceil(val);
}


EXPORT float CALL asAbs(float val) {
    return abs(val);
}


EXPORT float CALL asSqr(float val) {
    return sqrt(val);
}


EXPORT float CALL asSin(float val) {
    return sin(val * DEG2RAD);
}


EXPORT float CALL asCos(float val) {
    return cos(val * DEG2RAD);
}


EXPORT float CALL asTan(float val) {
    return tan(val * DEG2RAD);
}


EXPORT float CALL asASin(float val) {
    return asin(val * RAD2DEG);
}


EXPORT float CALL asACos(float val) {
    return acos(val * RAD2DEG);
}


EXPORT float CALL asATan(float val) {
    return atan(val * RAD2DEG);
}


EXPORT float CALL asATan2(float x, float y) {
    return atan2(x, y) * RAD2DEG;
}


EXPORT float CALL asExp(float val) {
    return exp(val);
}


EXPORT float CALL asLog(float val) {
    return log(val);
}


EXPORT float CALL asLog10(float val) {
    return log10(val);
}


EXPORT int CALL asRand(int min, int max) {
    return (rand() % (max - min)) + min;
}


EXPORT void CALL asRandSeed(int seed) {
    return srand(seed);
}


EXPORT float CALL asDeg(float rad) {
    return rad * RAD2DEG;
}


EXPORT float CALL asRad(float deg) {
    return deg * DEG2RAD;
}


EXPORT float CALL asPointX() {
    return _point.X;
}


EXPORT float CALL asPointY() {
    return _point.Y;
}


EXPORT float CALL asPointZ() {
    return _point.Z;
}


EXPORT float CALL asNormalX() {
    return _normal.X;
}


EXPORT float CALL asNormalY() {
    return _normal.Y;
}


EXPORT float CALL asNormalZ() {
    return _normal.Z;
}


void _asSetPoint(float x, float y, float z) {
    _point.set(x, y, z);
}


void _asSetNormal(float x, float y, float z) {
    _normal.set(x, y, z);
}


} // extern "C"
