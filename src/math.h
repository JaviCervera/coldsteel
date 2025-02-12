#pragma once

#include "common.h"

float Floor(float val);
float Ceil(float val);
float Abs(float val);
float Sqr(float val);
float Sin(float val);
float Cos(float val);
float Tan(float val);
float ASin(float val);
float ACos(float val);
float ATan(float val);
float ATan2(float x, float y);
float Exp(float val);
float Pow(float base, float exp);
float Log(float val);
float Log10(float val);
float Min(float a, float b);
float Max(float a, float b);
float Clamp(float val, float min, float max);
int BoolToInt(bool_t val);
int Int(float val);
int Rand(int min, int max);
void RandSeed(int seed);
float Deg(float rad);
float Rad(float deg);
float Wrap(float val, float mod);
float PointX();
float PointY();
float PointZ();
float NormalX();
float NormalY();
float NormalZ();

void _SetPoint(float x, float y, float z);
void _SetNormal(float x, float y, float z);
