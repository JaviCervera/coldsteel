#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT int CALL BitAnd(int a, int b);
EXPORT int CALL BitOr(int a, int b);
EXPORT int CALL BitXor(int a, int b);
EXPORT int CALL BitNot(int val);
EXPORT int CALL Shl(int val, int count);
EXPORT int CALL Shr(int val, int count);


#ifdef __cplusplus
} /* extern "C" */
#endif
