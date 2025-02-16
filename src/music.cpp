#include "audio_driver.h"
#include "music.h"

bool_t PlayMusic(const char *filename, bool_t loop)
{
  return AudioDriver::Get().PlayMusic(filename, loop);
}

void StopMusic()
{
  AudioDriver::Get().StopMusic();
}

void PauseMusic()
{
  AudioDriver::Get().PauseMusic();
}

void ResumeMusic()
{
  AudioDriver::Get().ResumeMusic();
}

void SetMusicVolume(float volume)
{
  AudioDriver::Get().SetMusicVolume(volume);
}

bool_t MusicPlaying()
{
  return AudioDriver::Get().MusicPlaying();
}
