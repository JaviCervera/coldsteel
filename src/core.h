#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asInit();
EXPORT bool_t CALL asRun();
EXPORT float CALL asDeltaTime();
EXPORT int CALL asMillisecs();

void _asSetDevice(IrrlichtDevice* device);
IrrlichtDevice* _asDevice();


#ifdef __cplusplus
} /* extern "C" */
#endif
