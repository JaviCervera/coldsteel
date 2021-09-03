#include "music.h"

extern "C" {


EXPORT bool_t CALL PlayMusic(const char* filename, bool_t loop) {
    return false;
}


EXPORT void CALL StopMusic() {
}


EXPORT void CALL PauseMusic() {
}


EXPORT void CALL ResumeMusic() {
}


EXPORT void CALL SetMusicVolume(float volume) {
}


EXPORT bool_t CALL MusicPlaying() {
    return false;
}


} // extern "C"
