#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include "audio_driver.h"
#include "core.h"
#include "dir.h"
#include "input.h"
#include "screen.h"

static IrrlichtDevice *_device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;

extern "C"
{

  EXPORT float CALL DeltaTime()
  {
    return _delta;
  }

  EXPORT int CALL Millisecs()
  {
    return _Device()->getTimer()->getRealTime() - _initMillisecs;
  }

  EXPORT void CALL Exit(int code)
  {
    CloseScreen();
    _Finish();
    exit(code);
  }

  EXPORT int CALL System(const char *command)
  {
    return system(command);
  }

  void _Init(const char *workingDir)
  {
    _SetDevice(NULL, workingDir);
    AudioDriver::Get().Init();
  }

  void _Finish()
  {
    AudioDriver::Get().Finish();
  }

  bool_t _Run()
  {
    AudioDriver::Get().Update();
    bool result = _Device()->run() && _Device()->getVideoDriver() != NULL;
    const int msecs = Millisecs();
    const int deltaMsecs = msecs - _lastMillisecs;
    const int wait = _ScreenFrameMsecs() - deltaMsecs;
    const int fixedWait = (wait > 0) ? wait : 0;
#ifndef EMSCRIPTEN
    if (result && fixedWait > 0)
    {
      _Device()->sleep(fixedWait);
    }
#else
    emscripten_sleep(fixedWait);
#endif
    const int msecsAfterWait = Millisecs();
    _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
    _lastMillisecs = msecsAfterWait;
    return result;
  }

  void _SetDevice(IrrlichtDevice *device, const char *workingDir)
  {
    if (_device)
      _device->drop();
    _device = device;
    if (!_device)
    {
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
    if (workingDir && strcmp(workingDir, ""))
    {
      ChangeDir(workingDir);
    }
#ifndef EMSCRIPTEN
    AddZip("data.bin");
#endif
  }

  IrrlichtDevice *_Device()
  {
    return _device;
  }

} // extern "C"
