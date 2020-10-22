#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL SetListener(float x, float y, float z, float yaw);
EXPORT float CALL ListenerX();
EXPORT float CALL ListenerY();
EXPORT float CALL ListenerZ();
EXPORT float CALL ListenerYaw();


#ifdef __cplusplus
} /* extern "C" */
#endif
