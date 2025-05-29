/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT const char *CALL LoadString(const char *filename);
  EXPORT void CALL SaveString(const char *str, const char *filename, bool_t append);

#ifdef __cplusplus
} /* extern "C" */
#endif
