/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT float CALL DeltaTime();
  EXPORT int CALL Millisecs();
  EXPORT void CALL Exit(int code);
  EXPORT int CALL System(const char *command);

#ifndef SWIG
  IrrlichtDevice *_Device();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
