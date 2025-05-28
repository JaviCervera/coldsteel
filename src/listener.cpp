#include "driver/audio.h"
#include "listener.h"

extern "C"
{

  EXPORT void CALL SetListenerPosition(float x, float y, float z)
  {
    Audio::Get().SetListenerPosition(x, y, z);
  }

  EXPORT void CALL SetListenerVelocity(float x, float y, float z)
  {
    Audio::Get().SetListenerVelocity(x, y, z);
  }

  EXPORT void CALL SetListenerYaw(float yaw)
  {
    Audio::Get().SetListenerYaw(yaw);
  }

} // extern "C"
