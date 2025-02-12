#pragma once

#include "common.h"

void SetCursorVisible(bool_t visible);
void SetCursorPosition(int x, int y);
int CursorX();
int CursorY();
bool_t ButtonDown(int button);
bool_t KeyDown(int key);
int NumJoysticks();
const char *JoystickName(int index);
int NumJoystickAxes(int index);
int NumJoystickButtons(int index);
bool_t JoystickButtonDown(int index, int button);
float JoystickAxis(int index, int axis);

void _SetCursorMoved();
void _SetButtonDown(int button, bool down);
void _SetKeyDown(int key, bool down);
void _SetJoysticks(const array<SJoystickInfo> &joysticks);
void _SetJoystickEvent(const SEvent::SJoystickEvent &event);
