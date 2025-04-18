#include "driver/audio.h"
#include "music.h"

EXPORT bool_t CALL PlayMusic(const char *filename, bool_t loop)
{
  return Audio::Get().PlayMusic(filename, loop);
}

EXPORT void CALL StopMusic()
{
  Audio::Get().StopMusic();
}

EXPORT void CALL PauseMusic()
{
  Audio::Get().PauseMusic();
}

EXPORT void CALL ResumeMusic()
{
  Audio::Get().ResumeMusic();
}

EXPORT void CALL SetMusicVolume(float volume)
{
  Audio::Get().SetMusicVolume(volume);
}

EXPORT bool_t CALL MusicPlaying()
{
  return Audio::Get().MusicPlaying();
}
