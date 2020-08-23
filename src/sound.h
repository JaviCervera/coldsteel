#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Sound;


EXPORT Sound* CALL csLoadSound(const char* filename);
EXPORT void CALL csFreeSound(Sound* sound);
EXPORT int CALL csPlaySound(Sound* sound, bool_t loop);


#ifdef __cplusplus
} /* extern "C" */
#endif
