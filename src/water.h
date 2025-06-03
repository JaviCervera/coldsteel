/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ISceneNode *CALL CreateWaterVolume(float width, float depth, int tiles_width, int tiles_depth, float wave_height, float wave_speed, float wave_length);

#ifdef __cplusplus
} /* extern "C" */
#endif
