#include "audio_driver.h"
#include "sound.h"

extern "C"
{

  EXPORT Sound *CALL LoadSound(const char *filename)
  {
    return AudioDriver::Get().LoadSound(filename);
  }

  EXPORT void CALL FreeSound(Sound *sound)
  {
    AudioDriver::Get().FreeSound(sound);
  }

  EXPORT Channel CALL PlaySound(Sound *sound, bool_t loop)
  {
    return AudioDriver::Get().PlaySound(sound, loop);
  }

  EXPORT Channel CALL PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool_t loop)
  {
    return AudioDriver::Get().PlaySound3D(sound, x, y, z, radius, loop);
  }

} // extern "C"
