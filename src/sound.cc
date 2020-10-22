#include <SDL/SDL_mixer.h>
#include "channel.h"
#include "filebuffer.h"
#include "sound.h"

extern "C" {


EXPORT Sound* CALL LoadSound(const char* filename) {
    FileBuffer buffer(filename);
    if (buffer.Size() > 0) {
        SDL_RWops* rw = SDL_RWFromConstMem(buffer.Buffer(), buffer.Size());
        return (Sound*)Mix_LoadWAV_RW(rw, true);
    } else {
        return NULL;
    }
}


EXPORT void CALL FreeSound(Sound* sound) {
    Mix_FreeChunk((Mix_Chunk*)sound);
}


EXPORT int CALL PlaySound(Sound* sound, bool_t loop) {
    int channel = Mix_PlayChannel(-1, (Mix_Chunk*)sound, loop ? -1 : 0);
    if (channel != -1) _InitChannel(channel);
    return channel;
}


} // extern "C"
