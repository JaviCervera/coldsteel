#include "core.h"
#include "input.h"

extern "C" {

static int _mouseXSpeed, _mouseYSpeed;
static int _lastMouseX = 0, _lastMouseY = 0;
static bool _mouseButton[3];
static bool _key[irr::KEY_KEY_CODES_COUNT];


EXPORT void CALL asSetCursorVisible(bool_t visible) {
    _asDevice()->getCursorControl()->setVisible(visible);
}


EXPORT void CALL asSetCursorPosition(int x, int y) {
    _asDevice()->getCursorControl()->setPosition(x, y);
}


EXPORT int CALL asCursorX() {
    return _asDevice()->getCursorControl()->getPosition().X;
}


EXPORT int CALL asCursorY() {
    return _asDevice()->getCursorControl()->getPosition().Y;
}


EXPORT bool_t CALL asButtonDown(int button) {
    return _mouseButton[button];
}


EXPORT bool_t CALL asKeyDown(int key) {
    return _key[key];
}


void _asSetCursorMoved() {
    _mouseXSpeed = asCursorX() - _lastMouseX;
    _mouseYSpeed = asCursorY() - _lastMouseY;
    _lastMouseX = asCursorX();
    _lastMouseY = asCursorY();
}


void _asSetButtonDown(int button, bool down) {
    _mouseButton[button] = down;
}


void _asSetKeyDown(int key, bool down) {
    _key[key] = down;
}


} // extern "C"
