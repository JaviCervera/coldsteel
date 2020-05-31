#include "../lib/sdl/SDL_mixer.h"
#include "channel.h"
#include "sound.h"

extern "C" {


EXPORT Sound* CALL asLoadSound(const char* filename) {
    return (Sound*)Mix_LoadWAV(filename);
}


EXPORT void CALL asFreeSound(Sound* sound) {
    Mix_FreeChunk((Mix_Chunk*)sound);
}


EXPORT int CALL asPlaySound(Sound* sound, bool_t loop) {
    int channel = Mix_PlayChannel(-1, (Mix_Chunk*)sound, loop ? -1 : 0);
    if (channel != -1) _asInitChannel(channel);
    return channel;
}


} // extern "C"
