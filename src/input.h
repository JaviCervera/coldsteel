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

void _asSetCursorMoved();
void _asSetButtonDown(int button, bool down);
void _asSetKeyDown(int key, bool down);


#ifdef __cplusplus
} /* extern "C" */
#endif
