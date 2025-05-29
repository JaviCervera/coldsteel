#include "include/audio.h"
#include "include/engine.h"
#include "sound.h"

extern "C"
{

  EXPORT Sound *CALL LoadSound(const char *filename)
  {
    return GetEngine().GetAudio().LoadSound(filename);
  }

  EXPORT void CALL FreeSound(Sound *sound)
  {
    GetEngine().GetAudio().FreeSound(sound);
  }

  EXPORT Channel CALL PlaySound(Sound *sound, bool_t loop)
  {
    return GetEngine().GetAudio().PlaySound(sound, loop);
  }

  EXPORT Channel CALL PlaySound3D(Sound *sound, float x, float y, float z, float radius, bool_t loop)
  {
    return GetEngine().GetAudio().PlaySound3D(sound, x, y, z, radius, loop);
  }

} // extern "C"
