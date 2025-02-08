#include "core.h"
#include "input.h"
#include "math.h"

extern "C"
{

    struct Joystick
    {
        SJoystickInfo info;
        SEvent::SJoystickEvent event;
    };

    static int _mouseXSpeed, _mouseYSpeed;
    static int _lastMouseX = 0, _lastMouseY = 0;
    static bool _mouseButton[3];
    static bool _key[irr::KEY_KEY_CODES_COUNT];
    static array<Joystick> _joysticks;

    static int _FindJoystickIndex(u8 id);

    EXPORT void CALL SetCursorVisible(bool_t visible)
    {
        _Device()->getCursorControl()->setVisible(visible);
    }

    EXPORT void CALL SetCursorPosition(int x, int y)
    {
        _Device()->getCursorControl()->setPosition(x, y);
    }

    EXPORT int CALL CursorX()
    {
        return _Device()->getCursorControl()->getPosition().X;
    }

    EXPORT int CALL CursorY()
    {
        return _Device()->getCursorControl()->getPosition().Y;
    }

    EXPORT bool_t CALL ButtonDown(int button)
    {
        return _mouseButton[button];
    }

    EXPORT bool_t CALL KeyDown(int key)
    {
        return _key[key];
    }

    EXPORT int CALL NumJoysticks()
    {
        return _joysticks.size();
    }

    EXPORT const char *CALL JoystickName(int index)
    {
        return _joysticks[index].info.Name.c_str();
    }

    EXPORT int CALL NumJoystickAxes(int index)
    {
        return _joysticks[index].info.Axes;
    }

    EXPORT int CALL NumJoystickButtons(int index)
    {
        return _joysticks[index].info.Buttons;
    }

    EXPORT bool_t CALL JoystickButtonDown(int index, int button)
    {
        return _joysticks[index].event.IsButtonPressed(button);
    }

    EXPORT float CALL JoystickAxis(int index, int axis)
    {
        const float value = _joysticks[index].event.Axis[axis] * 1.0f / 32768;
        const float fixedValue =
            (Abs(value) <= 0.1f) ? 0.0f : (value >= 0.995f) ? 1.0f
                                                            : value;
        return fixedValue;
    }

    void _SetCursorMoved()
    {
        _mouseXSpeed = CursorX() - _lastMouseX;
        _mouseYSpeed = CursorY() - _lastMouseY;
        _lastMouseX = CursorX();
        _lastMouseY = CursorY();
    }

    void _SetButtonDown(int button, bool down)
    {
        _mouseButton[button] = down;
    }

    void _SetKeyDown(int key, bool down)
    {
        _key[key] = down;
    }

    void _SetJoysticks(const array<SJoystickInfo> &joysticks)
    {
        _joysticks.clear();
        for (u32 i = 0; i < joysticks.size(); ++i)
        {
            _joysticks.push_back(Joystick());
            _joysticks[i].info = joysticks[i];
        }
    }

    void _SetJoystickEvent(const SEvent::SJoystickEvent &event)
    {
        int index = _FindJoystickIndex(event.Joystick);
        if (index != -1)
            _joysticks[index].event = event;
    }

    static int _FindJoystickIndex(u8 id)
    {
        for (u32 i = 0; i < _joysticks.size(); ++i)
            if (_joysticks[i].info.Joystick == id)
                return i;
        return -1;
    }

} // extern "C"
