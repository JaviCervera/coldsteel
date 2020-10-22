#include "core.h"
#include "input.h"
#include "screen.h"


static IrrlichtDevice* _device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;


extern "C" {


EXPORT void CALL Init() {
    _SetDevice(NULL);
}


EXPORT bool_t CALL Run() {
    bool result = _Device()->run() && _Device()->getVideoDriver() != NULL;
    const int msecs = Millisecs();
    const int deltaMsecs = msecs - _lastMillisecs;
    const int wait = _ScreenFrameMsecs() - deltaMsecs;
    const int fixedWait = (wait > 0) ? wait : 0;
    if (result && fixedWait > 0) {
        _Device()->sleep(fixedWait);
    }
    const int msecsAfterWait = Millisecs();
    _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
    _lastMillisecs = msecsAfterWait;
    return result;
}


EXPORT float CALL DeltaTime() {
    return _delta;
}


EXPORT int CALL Millisecs() {
    return _Device()->getTimer()->getRealTime() - _initMillisecs;
}


void _SetDevice(IrrlichtDevice* device) {
    if (_device) _device->drop();
    _device = device;
    if (!_device) {
        SIrrlichtCreationParameters params;
        params.DriverType = EDT_NULL;
        params.LoggingLevel = ELL_NONE;
        _device = createDeviceEx(params);
    }
    array<SJoystickInfo> joysticks;
    _device->activateJoysticks(joysticks);
    _SetJoysticks(joysticks);
    _initMillisecs = _device->getTimer()->getRealTime();
    _lastMillisecs = 0;
    _delta = 0.0f;
}


IrrlichtDevice* _Device() {
    return _device;
}


} // extern "C"
