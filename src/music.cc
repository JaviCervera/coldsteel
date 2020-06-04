#include "../lib/sdl/SDL_mixer.h"
#include "filebuffer.h"
#include "music.h"
#include "string.h"

static FileBuffer* _filebuffer = NULL;
static Mix_Music* _music = NULL;


extern "C" {


static Mix_MusicType _asMusicType(const char* filename);


EXPORT bool_t CALL asPlayMusic(const char* filename, bool_t loop) {
    if (_music != NULL) asStopMusic();
    _filebuffer = new FileBuffer(filename);
    if (_filebuffer->Size() > 0) {
        SDL_RWops* rw = SDL_RWFromConstMem(_filebuffer->Buffer(), _filebuffer->Size());
        _music = Mix_LoadMUSType_RW(rw, _asMusicType(filename), true);
        if (_music != NULL) {
            Mix_PlayMusic(_music, loop ? -1 : 1);
            return TRUE;
        }
    } else {
        delete _filebuffer;
        _filebuffer = NULL;
    }
    return FALSE;
}


EXPORT void CALL asStopMusic() {
    if (_music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(_music);
        delete _filebuffer;
        _music = NULL;
        _filebuffer = NULL;
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


static Mix_MusicType _asMusicType(const char* filename) {
    const int len = asLen(filename);
    const stringc lower = asLower(filename);
    const stringc last5 = (len >= 5) ? asRight(lower.c_str(), 5) : "";
    const stringc last4 = (len >= 4) ? asRight(lower.c_str(), 4) : "";
    if (last4 == ".wav") return MUS_WAV;
    if (last4 == ".mid") return MUS_MID;
    if (last5 == ".midi") return MUS_MID;
    if (last4 == ".kar") return MUS_MID;
    if (last4 == ".ogg") return MUS_OGG;
    if (last5 == ".flac") return MUS_FLAC;
    if (last4 == ".mp3") return MUS_MP3;
    if (last4 == ".mpg") return MUS_MP3;
    if (last5 == ".mpeg") return MUS_MP3;
    if (last4 == ".mad") return MUS_MP3;
    return MUS_WAV;
}


} // extern "C"
