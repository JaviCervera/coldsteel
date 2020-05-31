#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asSetListener(float x, float y, float z, float yaw);
EXPORT float CALL asListenerX();
EXPORT float CALL asListenerY();
EXPORT float CALL asListenerZ();
EXPORT float CALL asListenerYaw();


#ifdef __cplusplus
} /* extern "C" */
#endif
