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


static int _csFindJoystickIndex(u8 id);


EXPORT void CALL csSetCursorVisible(bool_t visible) {
    _csDevice()->getCursorControl()->setVisible(visible);
}


EXPORT void CALL csSetCursorPosition(int x, int y) {
    _csDevice()->getCursorControl()->setPosition(x, y);
}


EXPORT int CALL csCursorX() {
    return _csDevice()->getCursorControl()->getPosition().X;
}


EXPORT int CALL csCursorY() {
    return _csDevice()->getCursorControl()->getPosition().Y;
}


EXPORT bool_t CALL csButtonDown(int button) {
    return _mouseButton[button];
}


EXPORT bool_t CALL csKeyDown(int key) {
    return _key[key];
}


EXPORT int CALL csNumJoysticks() {
    return _joysticks.size();
}


EXPORT const char* CALL csJoystickName(int index) {
    return _joysticks[index].info.Name.c_str();
}


EXPORT int CALL csNumJoystickAxes(int index) {
    return _joysticks[index].info.Axes;
}


EXPORT int CALL csNumJoystickButtons(int index) {
    return _joysticks[index].info.Buttons;
}


EXPORT bool_t CALL csJoystickButtonDown(int index, int button) {
    return _joysticks[index].event.IsButtonPressed(button);
}


EXPORT float CALL csJoystickAxis(int index, int axis) {
    const float value = _joysticks[index].event.Axis[axis] * 1.0f / 32768;
    const float fixedValue =
        (csAbs(value) <= 0.1f) ? 0.0f :
        (value >= 0.995f) ? 1.0f :
        value;
    return fixedValue;
}


void _csSetCursorMoved() {
    _mouseXSpeed = csCursorX() - _lastMouseX;
    _mouseYSpeed = csCursorY() - _lastMouseY;
    _lastMouseX = csCursorX();
    _lastMouseY = csCursorY();
}


void _csSetButtonDown(int button, bool down) {
    _mouseButton[button] = down;
}


void _csSetKeyDown(int key, bool down) {
    _key[key] = down;
}


void _csSetJoysticks(const array<SJoystickInfo>& joysticks) {
    _joysticks.clear();
    for (u32 i = 0; i < joysticks.size(); ++i) {
        _joysticks.push_back(Joystick());
        _joysticks[i].info = joysticks[i];
    }
}


void _csSetJoystickEvent(const SEvent::SJoystickEvent& event) {
    int index = _csFindJoystickIndex(event.Joystick);
    if (index != -1) _joysticks[index].event = event;
}


static int _csFindJoystickIndex(u8 id) {
    for (u32 i = 0; i < _joysticks.size(); ++i)
        if (_joysticks[i].info.Joystick == id)
            return i;
    return -1;
}


} // extern "C"
