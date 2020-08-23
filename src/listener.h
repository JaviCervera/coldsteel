#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csSetListener(float x, float y, float z, float yaw);
EXPORT float CALL csListenerX();
EXPORT float CALL csListenerY();
EXPORT float CALL csListenerZ();
EXPORT float CALL csListenerYaw();


#ifdef __cplusplus
} /* extern "C" */
#endif
