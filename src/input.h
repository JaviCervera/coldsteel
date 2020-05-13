#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT void CALL asSetCursorVisible(bool_t visible);
EXPORT void CALL asSetCursorPosition(int x, int y);
EXPORT int CALL asCursorX();
EXPORT int CALL asCursorY();
EXPORT bool_t CALL asButtonDown(int button);
EXPORT bool_t CALL asKeyDown(int key);
EXPORT int CALL asNumJoysticks();
EXPORT const char* CALL asJoystickName(int index);
EXPORT int CALL asNumJoystickAxes(int index);
EXPORT int CALL asNumJoystickButtons(int index);
EXPORT bool_t CALL asJoystickButtonDown(int index, int button);
EXPORT float CALL asJoystickAxis(int index, int axis);

void _asSetCursorMoved();
void _asSetButtonDown(int button, bool down);
void _asSetKeyDown(int key, bool down);
void _asSetJoysticks(const array<SJoystickInfo>& joysticks);
void _asSetJoystickEvent(const SEvent::SJoystickEvent& event);


#ifdef __cplusplus
} /* extern "C" */
#endif
