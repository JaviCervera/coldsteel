#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT float CALL asFloor(float val);
EXPORT float CALL asCeil(float val);
EXPORT float CALL asAbs(float val);
EXPORT float CALL asSqr(float val);
EXPORT float CALL asSin(float val);
EXPORT float CALL asCos(float val);
EXPORT float CALL asTan(float val);
EXPORT float CALL asASin(float val);
EXPORT float CALL asACos(float val);
EXPORT float CALL asATan(float val);
EXPORT float CALL asATan2(float x, float y);
EXPORT float CALL asExp(float val);
EXPORT float CALL asLog(float val);
EXPORT float CALL asLog10(float val);
EXPORT int CALL asRand(int min, int max);
EXPORT void CALL asRandSeed(int seed);
EXPORT float CALL asDeg(float rad);
EXPORT float CALL asRad(float deg);
EXPORT float CALL asPointX();
EXPORT float CALL asPointY();
EXPORT float CALL asPointZ();
EXPORT float CALL asNormalX();
EXPORT float CALL asNormalY();
EXPORT float CALL asNormalZ();


void _asSetPoint(float x, float y, float z);
void _asSetNormal(float x, float y, float z);


#ifdef __cplusplus
} /* extern "C" */
#endif
