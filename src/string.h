#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT int CALL asVal(const char* str);
EXPORT float CALL asValF(const char* str);
EXPORT const char* CALL asStr(int number);
EXPORT const char* CALL asStrF(float number);
EXPORT const char* CALL asLeft(const char* str, int num);
EXPORT const char* CALL asRight(const char* str, int num);
EXPORT const char* CALL asMid(const char* str, int pos, int num);
EXPORT const char* CALL asReplace(const char* str, const char* find, const char* replace);
EXPORT int CALL asFind(const char* str, const char* find, int offset);
EXPORT const char* CALL asUpper(const char* str);
EXPORT const char* CALL asLower(const char* str);
EXPORT const char* CALL asTrim(const char* str);
EXPORT const char* CALL asChar(int code);
EXPORT int CALL asCode(const char* str);
EXPORT int CALL asLen(const char* str);
EXPORT const char* CALL asField(const char* str, const char* delimiter, int index);
EXPORT const char* CALL asLoadString(const char* filename);
EXPORT void CALL asSaveString(const char* str, const char* filename, bool_t append);


#ifdef __cplusplus
} /* extern "C" */
#endif
