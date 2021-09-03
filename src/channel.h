#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL StopChannel(int channel);
EXPORT void CALL PauseChannel(int channel);
EXPORT void CALL ResumeChannel(int channel);
EXPORT void CALL SetChannelPosition(int channel, float x, float y, float z, float radius);
EXPORT float CALL ChannelX(int channel);
EXPORT float CALL ChannelY(int channel);
EXPORT float CALL ChannelZ(int channel);
EXPORT float CALL ChannelRadius(int channel);
EXPORT void CALL SetChannelVolume(int channel, float volume);
EXPORT void CALL SetChannelPan(int channel, float pan);
EXPORT bool_t CALL ChannelPlaying(int channel);


#ifdef __cplusplus
} /* extern "C" */
#endif
