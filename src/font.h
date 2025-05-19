/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Font;

  EXPORT Font *CALL LoadFont(const char *filename, int height);
  EXPORT void CALL FreeFont(Font *font);
  EXPORT void CALL DrawText(Font *font, const char *text, int x, int y, int color);
  EXPORT int CALL TextWidth(Font *font, const char *text);
  EXPORT int CALL TextHeight(Font *font, const char *text);

#ifdef __cplusplus
} /* extern "C" */
#endif
