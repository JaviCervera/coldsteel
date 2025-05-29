#include "include/audio.h"
#include "include/engine.h"
#include "listener.h"

extern "C"
{

  EXPORT void CALL SetListener(float x, float y, float z, float yaw)
  {
    GetEngine().GetAudio().SetListener(x, y, z, yaw);
  }

} // extern "C"
