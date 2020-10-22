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


static void _UpdateChannel(int channel);


EXPORT void CALL StopChannel(int channel) {
    Mix_HaltChannel(channel);
}


EXPORT void CALL PauseChannel(int channel) {
    Mix_Pause(channel);
}


EXPORT void CALL ResumeChannel(int channel) {
    Mix_Resume(channel);
}


EXPORT void CALL SetChannelPosition(int channel, float x, float y, float z, float radius) {
    _channels[channel].position = vector3df(x, y, z);
    _channels[channel].radius = radius;
    _UpdateChannel(channel);
}


EXPORT float CALL ChannelX(int channel) {
    return _channels[channel].position.X;
}


EXPORT float CALL ChannelY(int channel) {
    return _channels[channel].position.Y;
}


EXPORT float CALL ChannelZ(int channel) {
    return _channels[channel].position.Z;
}


EXPORT float CALL ChannelRadius(int channel) {
    return _channels[channel].radius;
}


EXPORT void CALL SetChannelVolume(int channel, float volume) {
    Mix_Volume(channel, (int)(volume*128));
}


EXPORT void CALL SetChannelPan(int channel, float pan) {
    Uint8 left = 255, right = 255;
    if (pan < 0) right = 255 - (Uint8)(pan * -255);
    else if (pan > 0) left = 255 - (Uint8)(pan * 255);
    Mix_SetPanning(channel, left, right);
}


EXPORT bool_t CALL ChannelPlaying(int channel) {
    return Mix_Playing(channel);
}


void _UpdateChannels() {
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        if (ChannelPlaying(i) && _channels[i].radius > -1) {
            _UpdateChannel(i);
        }
    }
}



void _InitChannel(int channel) {
    _channels[channel] = ChannelInfo();
}


static void _UpdateChannel(int channel) {
    float distx = ListenerX() - ChannelX(channel);
    float disty = ListenerY() - ChannelY(channel);
    float distz = ListenerZ() - ChannelZ(channel);
    float angle = Wrap((-ATan2(distz, distx) - 90) - ListenerYaw(), 360);
    float dist = (float)sqrt(distx*distx + disty*disty + distz*distz);
    dist = dist * 255 / ChannelRadius(channel);
    if (dist > 255) dist = 255; // Wrap in 0 .. 255
    Mix_SetPosition(channel, (Sint16)angle, (Uint8)dist);
}


} // extern "C"
