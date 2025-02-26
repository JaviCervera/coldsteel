#include "audio_driver.h"
#include "music.h"

EXPORT bool_t CALL PlayMusic(const char *filename, bool_t loop)
{
  return AudioDriver::Get().PlayMusic(filename, loop);
}

EXPORT void CALL StopMusic()
{
  AudioDriver::Get().StopMusic();
}

EXPORT void CALL PauseMusic()
{
  AudioDriver::Get().PauseMusic();
}

EXPORT void CALL ResumeMusic()
{
  AudioDriver::Get().ResumeMusic();
}

EXPORT void CALL SetMusicVolume(float volume)
{
  AudioDriver::Get().SetMusicVolume(volume);
}

EXPORT bool_t CALL MusicPlaying()
{
  return AudioDriver::Get().MusicPlaying();
}
