#include <AL/al.h>
#define STB_VORBIS_HEADER_ONLY
#include "../lib/stb/stb_vorbis.c"
#include "channel.h"
#include "sound.h"

extern "C" {


struct Sound {
    ALuint id;
    Sound(ALuint id) : id(id) {}
};


static inline ALenum _GetFormatFromInfo(short channels, short bitsPerSample) {
	if (channels == 1)
		return AL_FORMAT_MONO16;
	return AL_FORMAT_STEREO16;
}


EXPORT Sound* CALL LoadSound(const char* filename) {
    stb_vorbis* file = stb_vorbis_open_filename((char*)filename, NULL, NULL);
    if (!file) return NULL;

    const stb_vorbis_info fileinfo = stb_vorbis_get_info(file);
    const int length_samples = stb_vorbis_stream_length_in_samples(file) * fileinfo.channels;
    short* buffer = new short[length_samples];
    stb_vorbis_get_samples_short_interleaved(file, fileinfo.channels, buffer, length_samples);

    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);
    alBufferData(alBuffer, _GetFormatFromInfo(fileinfo.channels, 0), buffer, length_samples * sizeof(short), fileinfo.sample_rate);
    stb_vorbis_close(file);
    delete[] buffer;

    return new Sound(alBuffer);
}


EXPORT void CALL FreeSound(Sound* sound) {
    if (sound) {
        alDeleteBuffers(1, &sound->id);
        delete sound;
    }
}


EXPORT Channel* CALL PlaySound(Sound* sound, bool_t loop) {
    return _PlayChannel(sound, loop);
}


} // extern "C"
