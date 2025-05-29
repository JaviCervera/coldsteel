#include "input.h"
#include "include/engine.h"
#include "include/platform.h"

extern "C"
{

  EXPORT void CALL SetCursorVisible(bool_t visible)
  {
    GetEngine().GetPlatform().SetCursorVisible(visible);
  }

  EXPORT void CALL SetCursorPosition(int x, int y)
  {
    GetEngine().GetPlatform().SetCursorPosition(x, y);
  }

  EXPORT int CALL CursorX()
  {
    return GetEngine().GetPlatform().CursorX();
  }

  EXPORT int CALL CursorY()
  {
    return GetEngine().GetPlatform().CursorY();
  }

  EXPORT int CALL CursorZ()
  {
    return GetEngine().GetPlatform().CursorZ();
  }

  EXPORT bool_t CALL ButtonDown(int button)
  {
    return GetEngine().GetPlatform().ButtonDown(button);
  }

  EXPORT bool_t CALL ButtonHit(int button)
  {
    return GetEngine().GetPlatform().ButtonHit(button);
  }

  EXPORT bool_t CALL KeyDown(int key)
  {
    return GetEngine().GetPlatform().KeyDown(key);
  }

  EXPORT bool_t CALL KeyHit(int key)
  {
    return GetEngine().GetPlatform().KeyHit(key);
  }

  EXPORT int CALL NumJoysticks()
  {
    return GetEngine().GetPlatform().NumJoysticks();
  }

  EXPORT const char *CALL JoystickName(int index)
  {
    return GetEngine().GetPlatform().JoystickName(index - 1);
  }

  EXPORT int CALL NumJoystickAxes(int index)
  {
    return GetEngine().GetPlatform().NumJoystickAxes(index - 1);
  }

  EXPORT int CALL NumJoystickButtons(int index)
  {
    return GetEngine().GetPlatform().NumJoystickButtons(index - 1);
  }

  EXPORT bool_t CALL JoystickButtonDown(int index, int button)
  {
    return GetEngine().GetPlatform().JoystickButtonDown(index - 1, button);
  }

  EXPORT float CALL JoystickAxis(int index, int axis)
  {
    return GetEngine().GetPlatform().JoystickAxis(index - 1, axis - 1);
  }

} // extern "C"
