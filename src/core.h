#pragma once

#include "common.h"

float DeltaTime();
int Millisecs();
void Exit(int code);
int System(const char *command);

#ifndef SWIG
void _Init();
void _Finish();
bool_t _Run();
void _SetDevice(IrrlichtDevice *device);
IrrlichtDevice *_Device();
#endif
