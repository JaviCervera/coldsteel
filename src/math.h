#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT float CALL csFloor(float val);
EXPORT float CALL csCeil(float val);
EXPORT float CALL csAbs(float val);
EXPORT float CALL csSqr(float val);
EXPORT float CALL csSin(float val);
EXPORT float CALL csCos(float val);
EXPORT float CALL csTan(float val);
EXPORT float CALL csASin(float val);
EXPORT float CALL csACos(float val);
EXPORT float CALL csATan(float val);
EXPORT float CALL csATan2(float x, float y);
EXPORT float CALL csExp(float val);
EXPORT float CALL csLog(float val);
EXPORT float CALL csLog10(float val);
EXPORT int CALL csRand(int min, int max);
EXPORT void CALL csRandSeed(int seed);
EXPORT float CALL csDeg(float rad);
EXPORT float CALL csRad(float deg);
EXPORT float CALL csWrap(float val, float mod);
EXPORT float CALL csPointX();
EXPORT float CALL csPointY();
EXPORT float CALL csPointZ();
EXPORT float CALL csNormalX();
EXPORT float CALL csNormalY();
EXPORT float CALL csNormalZ();


void _csSetPoint(float x, float y, float z);
void _csSetNormal(float x, float y, float z);


#ifdef __cplusplus
} /* extern "C" */
#endif
