#include "include/audio.h"
#include "include/engine.h"
#include "music.h"

EXPORT bool_t CALL PlayMusic(const char *filename, bool_t loop)
{
  return GetEngine().GetAudio().PlayMusic(filename, loop);
}

EXPORT void CALL StopMusic()
{
  GetEngine().GetAudio().StopMusic();
}

EXPORT void CALL PauseMusic()
{
  GetEngine().GetAudio().PauseMusic();
}

EXPORT void CALL ResumeMusic()
{
  GetEngine().GetAudio().ResumeMusic();
}

EXPORT void CALL SetMusicVolume(float volume)
{
  GetEngine().GetAudio().SetMusicVolume(volume);
}

EXPORT bool_t CALL MusicPlaying()
{
  return GetEngine().GetAudio().MusicPlaying();
}
