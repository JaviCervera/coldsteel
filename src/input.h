#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL SetCursorVisible(bool_t visible);
EXPORT void CALL SetCursorPosition(int x, int y);
EXPORT int CALL CursorX();
EXPORT int CALL CursorY();
EXPORT bool_t CALL ButtonDown(int button);
EXPORT bool_t CALL KeyDown(int key);
EXPORT int CALL NumJoysticks();
EXPORT const char* CALL JoystickName(int index);
EXPORT int CALL NumJoystickAxes(int index);
EXPORT int CALL NumJoystickButtons(int index);
EXPORT bool_t CALL JoystickButtonDown(int index, int button);
EXPORT float CALL JoystickAxis(int index, int axis);

void _SetCursorMoved();
void _SetButtonDown(int button, bool down);
void _SetKeyDown(int key, bool down);
void _SetJoysticks(const array<SJoystickInfo>& joysticks);
void _SetJoystickEvent(const SEvent::SJoystickEvent& event);


#ifdef __cplusplus
} /* extern "C" */
#endif
