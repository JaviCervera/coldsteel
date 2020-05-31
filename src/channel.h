#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL asStopChannel(int channel);
EXPORT void CALL asPauseChannel(int channel);
EXPORT void CALL asResumeChannel(int channel);
EXPORT void CALL asSetChannelPosition(int channel, float x, float y, float z, float radius);
EXPORT float CALL asChannelX(int channel);
EXPORT float CALL asChannelY(int channel);
EXPORT float CALL asChannelZ(int channel);
EXPORT float CALL asChannelRadius(int channel);
EXPORT void CALL asSetChannelVolume(int channel, float volume);
EXPORT void CALL asSetChannelPan(int channel, float pan);
EXPORT bool_t CALL asChannelPlaying(int channel);


void _asInitChannel(int channel);
void _asUpdateChannels();

#ifdef __cplusplus
} /* extern "C" */
#endif
