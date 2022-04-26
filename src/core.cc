#ifdef __APPLE__
#include <OpenAL/alc.h>
#else
#include <AL/alc.h>
#endif
#include "core.h"
#include "dir.h"
#include "input.h"
#include "music.h"
#include "screen.h"


static IrrlichtDevice* _device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;
static ALCdevice* _alDevice = NULL;
static ALCcontext* _alContext = NULL;


extern "C" {


EXPORT float CALL DeltaTime() {
    return _delta;
}


EXPORT int CALL Millisecs() {
    return _Device()->getTimer()->getRealTime() - _initMillisecs;
}


EXPORT void CALL Exit(int code) {
    _CloseScreen();
    _Finish();
    exit(code);
}


void _Init() {
    _SetDevice(NULL);

    // Create OpenAL context
    if (!_alDevice) _alDevice = alcOpenDevice(NULL);
    if (!_alContext) _alContext = alcCreateContext(_alDevice, NULL);
    if (_alContext) alcMakeContextCurrent(_alContext);
}

void _Finish() {
    if (_alContext) alcDestroyContext(_alContext);
    if (_alDevice) alcCloseDevice(_alDevice);
    _alDevice = NULL;
    _alContext = NULL;
}


bool_t _Run() {
    _UpdateMusic();
    bool result = _Device()->run() && _Device()->getVideoDriver() != NULL;
    const int msecs = Millisecs();
    const int deltaMsecs = msecs - _lastMillisecs;
    const int wait = _ScreenFrameMsecs() - deltaMsecs;
    const int fixedWait = (wait > 0) ? wait : 0;
#ifndef __EMSCRIPTEN__
    if (result && fixedWait > 0) {
        _Device()->sleep(fixedWait);
    }
#endif
    const int msecsAfterWait = Millisecs();
    _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
    _lastMillisecs = msecsAfterWait;
    return result;
}


void _SetDevice(IrrlichtDevice* device) {
    if (_device) _device->drop();
    _device = device;
    if (!_device) {
        SIrrlichtCreationParameters params;
        params.DriverType = EDT_NULL;
        params.LoggingLevel = ELL_ERROR;
        _device = createDeviceEx(params);
    }
    array<SJoystickInfo> joysticks;
    _device->activateJoysticks(joysticks);
    _SetJoysticks(joysticks);
    _initMillisecs = _device->getTimer()->getRealTime();
    _lastMillisecs = 0;
    _delta = 0.0f;
#ifndef EMSCRIPTEN
    AddZip("package.dat");
#endif
}


IrrlichtDevice* _Device() {
    return _device;
}


} // extern "C"
