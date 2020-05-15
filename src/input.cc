#include "core.h"
#include "input.h"
#include "math.h"

extern "C" {

struct Joystick {
    SJoystickInfo info;
    SEvent::SJoystickEvent event;
};

static int _mouseXSpeed, _mouseYSpeed;
static int _lastMouseX = 0, _lastMouseY = 0;
static bool _mouseButton[3];
static bool _key[irr::KEY_KEY_CODES_COUNT];
static array<Joystick> _joysticks;


static int _asFindJoystickIndex(u8 id);


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


EXPORT int CALL asNumJoysticks() {
    return _joysticks.size();
}


EXPORT const char* CALL asJoystickName(int index) {
    return _joysticks[index].info.Name.c_str();
}


EXPORT int CALL asNumJoystickAxes(int index) {
    return _joysticks[index].info.Axes;
}


EXPORT int CALL asNumJoystickButtons(int index) {
    return _joysticks[index].info.Buttons;
}


EXPORT bool_t CALL asJoystickButtonDown(int index, int button) {
    return _joysticks[index].event.IsButtonPressed(button);
}


EXPORT float CALL asJoystickAxis(int index, int axis) {
    const float value = _joysticks[index].event.Axis[axis] * 1.0f / 32768;
    const float fixedValue =
        (asAbs(value) <= 0.1f) ? 0.0f :
        (value >= 0.995f) ? 1.0f :
        value;
    return fixedValue;
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


void _asSetJoysticks(const array<SJoystickInfo>& joysticks) {
    _joysticks.clear();
    for (u32 i = 0; i < joysticks.size(); ++i) {
        _joysticks.push_back(Joystick());
        _joysticks[i].info = joysticks[i];
    }
}


void _asSetJoystickEvent(const SEvent::SJoystickEvent& event) {
    int index = _asFindJoystickIndex(event.Joystick);
    if (index != -1) _joysticks[index].event = event;
}


static int _asFindJoystickIndex(u8 id) {
    for (u32 i = 0; i < _joysticks.size(); ++i)
        if (_joysticks[i].info.Joystick == id)
            return i;
    return -1;
}


} // extern "C"
