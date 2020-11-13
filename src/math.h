#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT float CALL Floor(float val);
EXPORT float CALL Ceil(float val);
EXPORT float CALL Abs(float val);
EXPORT float CALL Sqr(float val);
EXPORT float CALL Sin(float val);
EXPORT float CALL Cos(float val);
EXPORT float CALL Tan(float val);
EXPORT float CALL ASin(float val);
EXPORT float CALL ACos(float val);
EXPORT float CALL ATan(float val);
EXPORT float CALL ATan2(float x, float y);
EXPORT float CALL Exp(float val);
EXPORT float CALL Log(float val);
EXPORT float CALL Log10(float val);
EXPORT int CALL Int(float val);
EXPORT int CALL Rand(int min, int max);
EXPORT void CALL RandSeed(int seed);
EXPORT float CALL Deg(float rad);
EXPORT float CALL Rad(float deg);
EXPORT float CALL Wrap(float val, float mod);
EXPORT float CALL PointX();
EXPORT float CALL PointY();
EXPORT float CALL PointZ();
EXPORT float CALL NormalX();
EXPORT float CALL NormalY();
EXPORT float CALL NormalZ();


void _SetPoint(float x, float y, float z);
void _SetNormal(float x, float y, float z);


#ifdef __cplusplus
} /* extern "C" */
#endif
