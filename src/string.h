#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT int CALL csVal(const char* str);
EXPORT float CALL csValF(const char* str);
EXPORT const char* CALL csStr(int number);
EXPORT const char* CALL csStrF(float number);
EXPORT const char* CALL csLeft(const char* str, int num);
EXPORT const char* CALL csRight(const char* str, int num);
EXPORT const char* CALL csMid(const char* str, int pos, int num);
EXPORT const char* CALL csReplace(const char* str, const char* find, const char* replace);
EXPORT int CALL csFind(const char* str, const char* find, int offset);
EXPORT const char* CALL csUpper(const char* str);
EXPORT const char* CALL csLower(const char* str);
EXPORT const char* CALL csTrim(const char* str);
EXPORT const char* CALL csChar(int code);
EXPORT int CALL csCode(const char* str);
EXPORT int CALL csLen(const char* str);
EXPORT const char* CALL csField(const char* str, const char* delimiter, int index);
EXPORT const char* CALL csLoadString(const char* filename);
EXPORT void CALL csSaveString(const char* str, const char* filename, bool_t append);


#ifdef __cplusplus
} /* extern "C" */
#endif
