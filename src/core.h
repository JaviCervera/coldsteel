#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csInit();
EXPORT bool_t CALL csRun();
EXPORT float CALL csDeltaTime();
EXPORT int CALL csMillisecs();

void _csSetDevice(IrrlichtDevice* device);
IrrlichtDevice* _csDevice();


#ifdef __cplusplus
} /* extern "C" */
#endif
