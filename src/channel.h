#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL csStopChannel(int channel);
EXPORT void CALL csPauseChannel(int channel);
EXPORT void CALL csResumeChannel(int channel);
EXPORT void CALL csSetChannelPosition(int channel, float x, float y, float z, float radius);
EXPORT float CALL csChannelX(int channel);
EXPORT float CALL csChannelY(int channel);
EXPORT float CALL csChannelZ(int channel);
EXPORT float CALL csChannelRadius(int channel);
EXPORT void CALL csSetChannelVolume(int channel, float volume);
EXPORT void CALL csSetChannelPan(int channel, float pan);
EXPORT bool_t CALL csChannelPlaying(int channel);


void _csInitChannel(int channel);
void _csUpdateChannels();

#ifdef __cplusplus
} /* extern "C" */
#endif
