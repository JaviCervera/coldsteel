/** @file */
#pragma once

#include "common.h"
#ifdef LoadString
#undef LoadString
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT int CALL Val(const char *str);
  EXPORT float CALL ValF(const char *str);
  EXPORT const char *CALL Str(int number);
  EXPORT const char *CALL StrF(float number);
  EXPORT const char *CALL Left(const char *str, int num);
  EXPORT const char *CALL Right(const char *str, int num);
  EXPORT const char *CALL Mid(const char *str, int pos, int num);
  EXPORT const char *CALL Replace(const char *str, const char *find, const char *replace);
  EXPORT int CALL Find(const char *str, const char *find, int offset);
  EXPORT const char *CALL Upper(const char *str);
  EXPORT const char *CALL Lower(const char *str);
  EXPORT const char *CALL Trim(const char *str);
  EXPORT const char *CALL Char(int code);
  EXPORT int CALL Code(const char *str);
  EXPORT int CALL Len(const char *str);
  EXPORT int CALL SplitCount(const char *str, const char *delimiter);
  EXPORT const char *CALL SplitIndex(const char *str, const char *delimiter, int index);
  EXPORT const char *CALL StripExt(const char *filename);
  EXPORT const char *CALL StripDir(const char *filename);
  EXPORT const char *CALL ExtractExt(const char *filename);
  EXPORT const char *CALL ExtractDir(const char *filename);
  EXPORT const char *CALL LoadString(const char *filename);
  EXPORT void CALL SaveString(const char *str, const char *filename, bool_t append);
  EXPORT void CALL CopyToClipboard(const char *str);
  EXPORT const char *CALL PasteFromClipboard();

#ifdef __cplusplus
} /* extern "C" */
#endif
