#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT bool_t CALL asPlayMusic(const char* filename, bool_t loop);
EXPORT void CALL asStopMusic();
EXPORT void CALL asPauseMusic();
EXPORT void CALL asResumeMusic();
EXPORT void CALL asSetMusicVolume(float volume);
EXPORT bool_t CALL asMusicPlaying();


#ifdef __cplusplus
} /* extern "C" */
#endif
