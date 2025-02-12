#pragma once

#include "common.h"

int RGB(int red, int green, int blue);
int RGBA(int red, int green, int blue, int alpha);
int RedVal(int color);
int GreenVal(int color);
int BlueVal(int color);
int AlphaVal(int color);
void BeginDrawing(bool_t clear, bool_t clearDepth, int color);
void EndDrawing();
void SetRenderTarget(ITexture *tex, bool_t clear, bool_t clearDepth, int color);
void SetViewport(int x, int y, int width, int height);
void DrawPoint(int x, int y, int color);
void DrawLine(int x1, int y1, int x2, int y2, int color);
void DrawRect(int x, int y, int width, int height, int color);
void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2, int color);

#ifndef SWIG
SColor _Color(int color);
int _IntColor(const SColor &color);
#endif
