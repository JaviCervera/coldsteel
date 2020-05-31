#include "../lib/sdl/SDL_mixer.h"
#include "music.h"

static Mix_Music* _music = NULL;


extern "C" {


EXPORT bool_t CALL asPlayMusic(const char* filename, bool_t loop) {
    if (_music != NULL) asStopMusic();
    _music = Mix_LoadMUS(filename);
    if (_music != NULL) {
        Mix_PlayMusic(_music, loop ? -1 : 1);
        return TRUE;
    } else {
        return FALSE;
    }
}


EXPORT void CALL asStopMusic() {
    if (_music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(_music);
        _music = NULL;
    }
}


EXPORT void CALL asPauseMusic() {
    Mix_PauseMusic();
}


EXPORT void CALL asResumeMusic() {
    Mix_ResumeMusic();
}


EXPORT void CALL asSetMusicVolume(float volume) {
    Mix_VolumeMusic(int(volume*128));
}


EXPORT bool_t CALL asMusicPlaying() {
    return Mix_PlayingMusic();
}


} // extern "C"
