#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL Init();
EXPORT void CALL Finish();
EXPORT bool_t CALL Run();
EXPORT float CALL DeltaTime();
EXPORT int CALL Millisecs();

void _SetDevice(IrrlichtDevice* device);
IrrlichtDevice* _Device();


#ifdef __cplusplus
} /* extern "C" */
#endif
