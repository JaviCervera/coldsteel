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
  void _Init();
  void _Finish();
  bool_t _Run();
  void _SetDevice(IrrlichtDevice *device);
  IrrlichtDevice *_Device();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
