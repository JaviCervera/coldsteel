#include "audio_driver.h"
#include "sound.h"

Sound *LoadSound(const char *filename)
{
  return AudioDriver::Get().LoadSound(filename);
}

void FreeSound(Sound *sound)
{
  AudioDriver::Get().FreeSound(sound);
}

Channel *PlaySound(Sound *sound, bool_t loop)
{
  return AudioDriver::Get().PlaySound(sound, loop);
}
