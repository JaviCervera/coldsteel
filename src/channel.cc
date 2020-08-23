#include <SDL/SDL_mixer.h>
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


static void _csUpdateChannel(int channel);


EXPORT void CALL csStopChannel(int channel) {
    Mix_HaltChannel(channel);
}


EXPORT void CALL csPauseChannel(int channel) {
    Mix_Pause(channel);
}


EXPORT void CALL csResumeChannel(int channel) {
    Mix_Resume(channel);
}


EXPORT void CALL csSetChannelPosition(int channel, float x, float y, float z, float radius) {
    _channels[channel].position = vector3df(x, y, z);
    _channels[channel].radius = radius;
    _csUpdateChannel(channel);
}


EXPORT float CALL csChannelX(int channel) {
    return _channels[channel].position.X;
}


EXPORT float CALL csChannelY(int channel) {
    return _channels[channel].position.Y;
}


EXPORT float CALL csChannelZ(int channel) {
    return _channels[channel].position.Z;
}


EXPORT float CALL csChannelRadius(int channel) {
    return _channels[channel].radius;
}


EXPORT void CALL csSetChannelVolume(int channel, float volume) {
    Mix_Volume(channel, (int)(volume*128));
}


EXPORT void CALL csSetChannelPan(int channel, float pan) {
    Uint8 left = 255, right = 255;
    if (pan < 0) right = 255 - (Uint8)(pan * -255);
    else if (pan > 0) left = 255 - (Uint8)(pan * 255);
    Mix_SetPanning(channel, left, right);
}


EXPORT bool_t CALL csChannelPlaying(int channel) {
    return Mix_Playing(channel);
}


void _csUpdateChannels() {
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        if (csChannelPlaying(i) && _channels[i].radius > -1) {
            _csUpdateChannel(i);
        }
    }
}



void _csInitChannel(int channel) {
    _channels[channel] = ChannelInfo();
}


static void _csUpdateChannel(int channel) {
    float distx = csListenerX() - csChannelX(channel);
    float disty = csListenerY() - csChannelY(channel);
    float distz = csListenerZ() - csChannelZ(channel);
    float angle = csWrap((-csATan2(distz, distx) - 90) - csListenerYaw(), 360);
    float dist = (float)sqrt(distx*distx + disty*disty + distz*distz);
    dist = dist * 255 / csChannelRadius(channel);
    if (dist > 255) dist = 255; // Wrap in 0 .. 255
    Mix_SetPosition(channel, (Sint16)angle, (Uint8)dist);
}


} // extern "C"
