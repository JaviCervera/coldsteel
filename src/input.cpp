#include "core.h"
#include "input.h"
#include "math.h"

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

void SetCursorVisible(bool_t visible)
{
  _Device()->getCursorControl()->setVisible(visible);
}

void SetCursorPosition(int x, int y)
{
  _Device()->getCursorControl()->setPosition(x, y);
}

int CursorX()
{
  return _Device()->getCursorControl()->getPosition().X;
}

int CursorY()
{
  return _Device()->getCursorControl()->getPosition().Y;
}

bool_t ButtonDown(int button)
{
  return _mouseButton[button];
}

bool_t KeyDown(int key)
{
  return _key[key];
}

int NumJoysticks()
{
  return _joysticks.size();
}

const char *JoystickName(int index)
{
  return _joysticks[index].info.Name.c_str();
}

int NumJoystickAxes(int index)
{
  return _joysticks[index].info.Axes;
}

int NumJoystickButtons(int index)
{
  return _joysticks[index].info.Buttons;
}

bool_t JoystickButtonDown(int index, int button)
{
  return _joysticks[index].event.IsButtonPressed(button);
}

float JoystickAxis(int index, int axis)
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
