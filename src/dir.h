#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT bool_t CALL asAddZip(const char* filename);
EXPORT const char* CALL asDirContents(const char* dir);
EXPORT void CALL asChangeDir(const char* dir);
EXPORT const char* CALL asCurrentDir();


#ifdef __cplusplus
} /* extern "C" */
#endif
