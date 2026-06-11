#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_OSX
#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#endif
#endif
#include "internal/audio.h"
#include "core.h"
#include "dir.h"
#include "input.h"
#include "screen.h"

#if defined(__APPLE__) && TARGET_OS_OSX
__attribute__((constructor))
static void MacOSAppActivationSetup()
{
    Class nsAppClass = objc_getClass("NSApplication");
    if (!nsAppClass) return;
    SEL sharedAppSel = sel_registerName("sharedApplication");
    id (*sharedAppMsg)(id, SEL) = (id (*)(id, SEL))objc_msgSend;
    id app = sharedAppMsg((id)nsAppClass, sharedAppSel);
    if (!app) return;
    void (*setPolicyMsg)(id, SEL, long) = (void (*)(id, SEL, long))objc_msgSend;
    setPolicyMsg(app, sel_registerName("setActivationPolicy:"), 0L);
    void (*activateMsg)(id, SEL, int) = (void (*)(id, SEL, int))objc_msgSend;
    activateMsg(app, sel_registerName("activateIgnoringOtherApps:"), 1);
}
#endif

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
    Audio::Get().Init();
  }

  void _Finish()
  {
    Audio::Get().Finish();
  }

  bool_t _Run()
  {
    Audio::Get().Update();
    _ClearInputs();
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
    io::path savedDir = "";
    if (_device)
    {
      savedDir = _device->getFileSystem()->getWorkingDirectory();
      _device->closeDevice();
      _device->run();
      _device->drop();
    }
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
    else if (savedDir != "")
    {
      ChangeDir(savedDir.c_str());
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
