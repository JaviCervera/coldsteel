/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT void CALL CopyToClipboard(const char *str);
  EXPORT const char *CALL PasteFromClipboard();

#ifdef __cplusplus
} /* extern "C" */
#endif
