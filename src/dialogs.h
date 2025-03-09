#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT bool_t CALL Confirm(const char *title, const char *text, bool_t serious);
  EXPORT void CALL Notify(const char *title, const char *text, bool_t serious);
  EXPORT int CALL Proceed(const char *title, const char *text, bool_t serious);
  EXPORT int CALL RequestColor(const char *title, int color);
  EXPORT const char *CALL RequestDir(const char *title, const char *dir);
  EXPORT const char *CALL RequestFile(const char *title, const char *filters, bool_t save, const char *file);
  EXPORT const char *CALL RequestInput(const char *title, const char *text, const char *def, bool_t password);

#ifdef __cplusplus
} /* extern "C" */
#endif
