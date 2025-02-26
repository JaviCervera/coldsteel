#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT bool_t CALL AddZip(const char *filename);
  EXPORT const char *CALL DirContents(const char *dir);
  EXPORT void CALL ChangeDir(const char *dir);
  EXPORT const char *CALL CurrentDir();

#ifdef __cplusplus
} /* extern "C" */
#endif
