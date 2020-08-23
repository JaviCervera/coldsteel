#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL csSetCursorVisible(bool_t visible);
EXPORT void CALL csSetCursorPosition(int x, int y);
EXPORT int CALL csCursorX();
EXPORT int CALL csCursorY();
EXPORT bool_t CALL csButtonDown(int button);
EXPORT bool_t CALL csKeyDown(int key);
EXPORT int CALL csNumJoysticks();
EXPORT const char* CALL csJoystickName(int index);
EXPORT int CALL csNumJoystickAxes(int index);
EXPORT int CALL csNumJoystickButtons(int index);
EXPORT bool_t CALL csJoystickButtonDown(int index, int button);
EXPORT float CALL csJoystickAxis(int index, int axis);

void _csSetCursorMoved();
void _csSetButtonDown(int button, bool down);
void _csSetKeyDown(int key, bool down);
void _csSetJoysticks(const array<SJoystickInfo>& joysticks);
void _csSetJoystickEvent(const SEvent::SJoystickEvent& event);


#ifdef __cplusplus
} /* extern "C" */
#endif
