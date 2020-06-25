#include "../lib/sdl/include/SDL/SDL_mixer.h"
#include "channel.h"
#include "listener.h"
#include "math.h"

struct ChannelInfo {
    float radius;
    vector3df position;

    ChannelInfo() : radius(-1), position(0, 0, 0) {}
};

static ChannelInfo _channels[MIX_CHANNELS];


extern "C" {


static void _asUpdateChannel(int channel);


EXPORT void CALL asStopChannel(int channel) {
    Mix_HaltChannel(channel);
}


EXPORT void CALL asPauseChannel(int channel) {
    Mix_Pause(channel);
}


EXPORT void CALL asResumeChannel(int channel) {
    Mix_Resume(channel);
}


EXPORT void CALL asSetChannelPosition(int channel, float x, float y, float z, float radius) {
    _channels[channel].position = vector3df(x, y, z);
    _channels[channel].radius = radius;
    _asUpdateChannel(channel);
}


EXPORT float CALL asChannelX(int channel) {
    return _channels[channel].position.X;
}


EXPORT float CALL asChannelY(int channel) {
    return _channels[channel].position.Y;
}


EXPORT float CALL asChannelZ(int channel) {
    return _channels[channel].position.Z;
}


EXPORT float CALL asChannelRadius(int channel) {
    return _channels[channel].radius;
}


EXPORT void CALL asSetChannelVolume(int channel, float volume) {
    Mix_Volume(channel, (int)(volume*128));
}


EXPORT void CALL asSetChannelPan(int channel, float pan) {
    Uint8 left = 255, right = 255;
    if (pan < 0) right = 255 - (Uint8)(pan * -255);
    else if (pan > 0) left = 255 - (Uint8)(pan * 255);
    Mix_SetPanning(channel, left, right);
}


EXPORT bool_t CALL asChannelPlaying(int channel) {
    return Mix_Playing(channel);
}


void _asUpdateChannels() {
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        if (asChannelPlaying(i) && _channels[i].radius > -1) {
            _asUpdateChannel(i);
        }
    }
}



void _asInitChannel(int channel) {
    _channels[channel] = ChannelInfo();
}


static void _asUpdateChannel(int channel) {
    float distx = asListenerX() - asChannelX(channel);
    float disty = asListenerY() - asChannelY(channel);
    float distz = asListenerZ() - asChannelZ(channel);
    float angle = asWrap((-asATan2(distz, distx) - 90) - asListenerYaw(), 360);
    float dist = (float)sqrt(distx*distx + disty*disty + distz*distz);
    dist = dist * 255 / asChannelRadius(channel);
    if (dist > 255) dist = 255; // Wrap in 0 .. 255
    Mix_SetPosition(channel, (Sint16)angle, (Uint8)dist);
}


} // extern "C"
