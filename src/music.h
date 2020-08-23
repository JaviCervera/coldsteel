#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT bool_t CALL csPlayMusic(const char* filename, bool_t loop);
EXPORT void CALL csStopMusic();
EXPORT void CALL csPauseMusic();
EXPORT void CALL csResumeMusic();
EXPORT void CALL csSetMusicVolume(float volume);
EXPORT bool_t CALL csMusicPlaying();


#ifdef __cplusplus
} /* extern "C" */
#endif
