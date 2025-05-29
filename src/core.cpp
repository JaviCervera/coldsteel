#include "common.h"
#include "internal/audio.h"
#include "internal/platform.h"

extern "C"
{

  EXPORT float CALL DeltaTime()
  {
    return Platform::Get().DeltaTime();
  }

  EXPORT int CALL Millisecs()
  {
    return Platform::Get().Millisecs();
  }

  EXPORT void CALL Exit(int code)
  {
    Platform::Get().Finish();
    Audio::Get().Finish();
    exit(code);
  }

  EXPORT int CALL System(const char *command)
  {
    return system(command);
  }

  IrrlichtDevice *_Device()
  {
    return (IrrlichtDevice *)Platform::Get().InternalHandle();
  }

} // extern "C"
