#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT bool_t CALL csAddZip(const char* filename);
EXPORT const char* CALL csDirContents(const char* dir);
EXPORT void CALL csChangeDir(const char* dir);
EXPORT const char* CALL csCurrentDir();


#ifdef __cplusplus
} /* extern "C" */
#endif
