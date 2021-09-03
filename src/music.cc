#include <AL/al.h>
#define STB_VORBIS_HEADER_ONLY
#include "../lib/stb/stb_vorbis.c"
#include "music.h"

extern "C" {


struct AudioStream {
    ALuint source;
    stb_vorbis* stream;
    stb_vorbis_info info;
    ALuint buffers[2];
    size_t samplesLeft;
    bool_t loop;

    AudioStream(ALuint source, stb_vorbis* stream, bool_t loop) : stream(stream), samplesLeft(0), loop(loop) {
        memset(&info, 0, sizeof(stb_vorbis_info));
        buffers[0] = 0;
        buffers[1] = 0;
    }
};


static bool_t _UpdateStream(AudioStream* stream, size_t bufferIndex);

static AudioStream* _music = NULL;


EXPORT bool_t CALL PlayMusic(const char* filename, bool_t loop) {
    StopMusic();

    stb_vorbis* oggStream = stb_vorbis_open_filename(filename, NULL, NULL);
    if (!oggStream) return false;

    ALuint source;
    alGenSources(1, &source);

    _music = new AudioStream(source, oggStream, loop);
    _music->info = stb_vorbis_get_info(oggStream);
    _music->samplesLeft = stb_vorbis_stream_length_in_samples(oggStream) * _music->info.channels;

    alGenBuffers(2, _music->buffers);
    if (!_UpdateStream(_music, 0)) { StopMusic(); return false; }
    if (!_UpdateStream(_music, 1)) { StopMusic(); return false; }
    alSourceQueueBuffers(_music->source, 2, _music->buffers);

    return true;
}


EXPORT void CALL StopMusic() {
    if (_music) {
        alDeleteSources(1, &_music->source);
        alDeleteBuffers(2, _music->buffers);
        stb_vorbis_close(_music->stream);
        delete _music;
    }
    _music = NULL;
}


EXPORT void CALL PauseMusic() {
    if (_music) {
        alSourcePause(_music->source);
    }
}


EXPORT void CALL ResumeMusic() {
    if (_music) {
        alSourcePlay(_music->source);
    }
}


EXPORT void CALL SetMusicVolume(float volume) {
    if (_music) {
        alSourcef(_music->source, AL_GAIN, volume);
    }
}


EXPORT bool_t CALL MusicPlaying() {
    if (_music) {
        int state;
        alGetSourcei(_music->source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    } else {
        return false;
    }
}

void _UpdateMusic() {
    if (_music) {
        ALint processed = 0;
        alGetSourcei(_music->source, AL_BUFFERS_PROCESSED, &processed);
        while (processed--) {
            ALuint buffer;
            alSourceUnqueueBuffers(_music->source, 1, &buffer);
            bool streamOk = _UpdateStream(_music, buffer);
            if (streamOk) {
                alSourceQueueBuffers(_music->source, 1, &buffer);
            } else if (_music->loop) {
                stb_vorbis_seek_start(_music->stream);
                _music->samplesLeft = stb_vorbis_stream_length_in_samples(_music->stream) * _music->info.channels;
                if ( _UpdateStream(_music, buffer) ) alSourceQueueBuffers(_music->source, 1, &buffer);
            }
        }
    }
}


#define BUFFER_SIZE 4096*8


static bool_t _UpdateStream(AudioStream* stream, size_t bufferIndex) {
    ALshort pcm[BUFFER_SIZE];
    int size = stb_vorbis_get_samples_short_interleaved(stream->stream, stream->info.channels, pcm, BUFFER_SIZE);
    if (size == 0) return false;
    alBufferData(
        stream->buffers[bufferIndex],
        stream->info.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
        pcm,
        size * stream->info.channels * sizeof(ALshort), stream->info.sample_rate);
    stream->samplesLeft -= size;
    return true;
}


} // extern "C"
