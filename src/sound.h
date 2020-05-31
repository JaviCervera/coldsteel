#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Sound;


EXPORT Sound* CALL asLoadSound(const char* filename);
EXPORT void CALL asFreeSound(Sound* sound);
EXPORT int CALL asPlaySound(Sound* sound, bool_t loop);


#ifdef __cplusplus
} /* extern "C" */
#endif
