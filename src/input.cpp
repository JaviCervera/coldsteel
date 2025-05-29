#include "input.h"
#include "internal/platform.h"

extern "C"
{

  EXPORT void CALL SetCursorVisible(bool_t visible)
  {
    Platform::Get().SetCursorVisible(visible);
  }

  EXPORT void CALL SetCursorPosition(int x, int y)
  {
    Platform::Get().SetCursorPosition(x, y);
  }

  EXPORT int CALL CursorX()
  {
    return Platform::Get().CursorX();
  }

  EXPORT int CALL CursorY()
  {
    return Platform::Get().CursorY();
  }

  EXPORT int CALL CursorZ()
  {
    return Platform::Get().CursorZ();
  }

  EXPORT bool_t CALL ButtonDown(int button)
  {
    return Platform::Get().ButtonDown(button);
  }

  EXPORT bool_t CALL ButtonHit(int button)
  {
    return Platform::Get().ButtonHit(button);
  }

  EXPORT bool_t CALL KeyDown(int key)
  {
    return Platform::Get().KeyDown(key);
  }

  EXPORT bool_t CALL KeyHit(int key)
  {
    return Platform::Get().KeyHit(key);
  }

  EXPORT int CALL NumJoysticks()
  {
    return Platform::Get().NumJoysticks();
  }

  EXPORT const char *CALL JoystickName(int index)
  {
    return Platform::Get().JoystickName(index - 1);
  }

  EXPORT int CALL NumJoystickAxes(int index)
  {
    return Platform::Get().NumJoystickAxes(index - 1);
  }

  EXPORT int CALL NumJoystickButtons(int index)
  {
    return Platform::Get().NumJoystickButtons(index - 1);
  }

  EXPORT bool_t CALL JoystickButtonDown(int index, int button)
  {
    return Platform::Get().JoystickButtonDown(index - 1, button);
  }

  EXPORT float CALL JoystickAxis(int index, int axis)
  {
    return Platform::Get().JoystickAxis(index - 1, axis - 1);
  }

} // extern "C"
