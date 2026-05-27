/** @file
 * Logging functions for debug output.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Logs an informational message to the engine output log.
   *
   * @param msg The message to log.
   */
  EXPORT void CALL LogInfo(const char *msg);

#ifdef __cplusplus
} /* extern "C" */
#endif
