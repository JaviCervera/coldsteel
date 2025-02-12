#pragma once

#include "common.h"

ICameraSceneNode *CreateCamera();
void SetCameraRange(ICameraSceneNode *cam, float near_, float far_);
float CameraNearRange(ICameraSceneNode *cam);
float CameraFarRange(ICameraSceneNode *cam);
void SetCameraFOV(ICameraSceneNode *cam, float fov);
float CameraFOV(ICameraSceneNode *cam);
void SetCameraOrtho(ICameraSceneNode *cam, float width, float height, float near_, float far_);
ISceneNode *PickEntity(ICameraSceneNode *camera, int x, int y, int group);
