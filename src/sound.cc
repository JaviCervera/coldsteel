#include "channel.h"
#include "filebuffer.h"
#include "sound.h"

extern "C" {


EXPORT Sound* CALL LoadSound(const char* filename) {
    return NULL;
}


EXPORT void CALL FreeSound(Sound* sound) {
}


EXPORT int CALL PlaySound(Sound* sound, bool_t loop) {
}


} // extern "C"
