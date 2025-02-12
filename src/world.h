#pragma once

#include "common.h"

void SetAmbient(int color);
void SetShadowColor(int color);
void SetFog(int color, float near_, float far_);
void SetSkybox(ITexture *top, ITexture *bottom, ITexture *left, ITexture *right, ITexture *front, ITexture *back);
void SetSkydome(ITexture *tex, float texScale, bool_t fullSphere);
SMaterial *SkyboxMaterial();
SMaterial *SkydomeMaterial();
void DrawWorld(ICameraSceneNode *camera);
void WorldToScreen(ICameraSceneNode *camera, float x, float y, float z);
void ScreenToWorld(ICameraSceneNode *camera, int x, int y, float depth);
ISceneNode *Raycast(float x1, float y1, float z1, float x2, float y2, float z2, int group);

#ifndef SWIG
bool _HasSetAmbient();
#endif
