#include "core.h"
#include "input.h"
#include "screen.h"


static IrrlichtDevice* _device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;


extern "C" {


EXPORT void CALL csInit() {
    _csSetDevice(NULL);
}


EXPORT bool_t CALL csRun() {
    bool result = _csDevice()->run() && _csDevice()->getVideoDriver() != NULL;
    const int msecs = csMillisecs();
    const int deltaMsecs = msecs - _lastMillisecs;
    const int wait = _csScreenFrameMsecs() - deltaMsecs;
    const int fixedWait = (wait > 0) ? wait : 0;
    if (result && fixedWait > 0) {
        _csDevice()->sleep(fixedWait);
    }
    const int msecsAfterWait = csMillisecs();
    _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
    _lastMillisecs = msecsAfterWait;
    return result;
}


EXPORT float CALL csDeltaTime() {
    return _delta;
}


EXPORT int CALL csMillisecs() {
    return _csDevice()->getTimer()->getRealTime() - _initMillisecs;
}


void _csSetDevice(IrrlichtDevice* device) {
    if (_device) _device->drop();
    _device = device;
    if (!_device) {
        SIrrlichtCreationParameters params;
        params.DeviceType = EIDT_SDL;
        params.DriverType = EDT_NULL;
        params.LoggingLevel = ELL_NONE;
        _device = createDeviceEx(params);
    }
    array<SJoystickInfo> joysticks;
    _device->activateJoysticks(joysticks);
    _csSetJoysticks(joysticks);
    _initMillisecs = _device->getTimer()->getRealTime();
    _lastMillisecs = 0;
    _delta = 0.0f;
}


IrrlichtDevice* _csDevice() {
    return _device;
}


} // extern "C"
