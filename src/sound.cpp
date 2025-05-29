#include "internal/audio.h"
#include "sound.h"

extern "C"
{

  EXPORT Sound *CALL LoadSound(const char *filename)
  {
    return Audio::Get().LoadSound(filename);
  }

  EXPORT void CALL FreeSound(Sound *sound)
  {
    Audio::Get().FreeSound(sound);
  }

  EXPORT Channel CALL PlaySound(Sound *sound, bool_t loop)
  {
    return Audio::Get().PlaySound(sound, loop);
  }

  EXPORT Channel CALL PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool_t loop)
  {
    return Audio::Get().PlaySound3D(sound, x, y, z, radius, loop);
  }

} // extern "C"
