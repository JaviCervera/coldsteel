#include "core.h"
#include "screen.h"


static IrrlichtDevice* _device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;


extern "C" {


EXPORT void CALL asInit() {
    _asSetDevice(NULL);
}


EXPORT bool_t CALL asRun() {
    bool result = _asDevice()->run() && _asDevice()->getVideoDriver() != NULL;
    const int msecs = asMillisecs();
    const int deltaMsecs = msecs - _lastMillisecs;
    const int wait = _asScreenFrameMsecs() - deltaMsecs;
    const int fixedWait = (wait > 0) ? wait : 0;
    if (result && fixedWait > 0) {
        _asDevice()->sleep(fixedWait);
    }
    const int msecsAfterWait = asMillisecs();
    _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
    _lastMillisecs = msecsAfterWait;
    return result;
}


EXPORT float CALL asDeltaTime() {
    return _delta;
}


EXPORT int CALL asMillisecs() {
    return _asDevice()->getTimer()->getRealTime() - _initMillisecs;
}


void _asSetDevice(IrrlichtDevice* device) {
    if (_device) _device->drop();
    _device = device;
    if (!_device) {
        SIrrlichtCreationParameters params;
        params.DriverType = EDT_NULL;
        params.LoggingLevel = ELL_NONE;
        _device = createDeviceEx(params);
    }
    _initMillisecs = _device->getTimer()->getRealTime();
    _lastMillisecs = 0;
    _delta = 0.0f;
}


IrrlichtDevice* _asDevice() {
    return _device;
}


} // extern "C"
