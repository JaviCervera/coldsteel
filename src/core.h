/**
 * @file
 * Core engine functions including timing and application lifecycle.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Returns the time in seconds elapsed since the last frame.
   *
   * @return The time in seconds elapsed since the last frame.
   */
  EXPORT float CALL DeltaTime();

  /**
   * Returns the number of milliseconds since the engine started.
   *
   * @return The number of milliseconds since the engine started.
   */
  EXPORT int CALL Millisecs();

  /**
   * Exits the application.
   *
   * @param code The exit code to return.
   */
  EXPORT void CALL Exit(int code);

  /**
   * Executes a system shell command.
   *
   * @param command The command to execute.
   * @return The exit code returned by the command.
   */
  EXPORT int CALL System(const char *command);

#ifndef SWIG
  void _Init(const char *workingDir);
  void _Finish();
  bool_t _Run();
  void _SetDevice(IrrlichtDevice *device, const char *workingDir);
  IrrlichtDevice *_Device();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
