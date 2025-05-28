/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL SetListenerPosition(float x, float y, float z);
  EXPORT void CALL SetListenerVelocity(float x, float y, float z);
  EXPORT void CALL SetListenerYaw(float yaw);

#ifdef __cplusplus
} /* extern "C" */
#endif
