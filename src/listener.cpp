#include "internal/audio.h"
#include "listener.h"

extern "C"
{

  EXPORT void CALL SetListener(float x, float y, float z, float yaw)
  {
    Audio::Get().SetListener(x, y, z, yaw);
  }

} // extern "C"
