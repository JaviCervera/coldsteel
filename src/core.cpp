#include "common.h"
#include "include/engine.h"
#include "include/platform.h"

extern "C"
{

  EXPORT float CALL DeltaTime()
  {
    return GetEngine().GetPlatform().DeltaTime();
  }

  EXPORT int CALL Millisecs()
  {
    return GetEngine().GetPlatform().Millisecs();
  }

  EXPORT void CALL Exit(int code)
  {
    exit(code);
  }

  EXPORT int CALL System(const char *command)
  {
    return system(command);
  }

  IrrlichtDevice *_Device()
  {
    return (IrrlichtDevice *)GetEngine().GetPlatform().InternalHandle();
  }

} // extern "C"
