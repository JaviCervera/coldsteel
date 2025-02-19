#include "audio_driver.h"
#include "core.h"
#include "dir.h"
#include "input.h"
#include "screen.h"

static IrrlichtDevice *_device = NULL;
static u32 _initMillisecs;
static int _lastMillisecs;
static float _delta = 0.0f;

float DeltaTime()
{
  return _delta;
}

int Millisecs()
{
  return _Device()->getTimer()->getRealTime() - _initMillisecs;
}

void Exit(int code)
{
  _CloseScreen();
  _Finish();
  exit(code);
}

int System(const char *command)
{
  return system(command);
}

void _Init()
{
  _SetDevice(NULL);
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
#ifndef __EMSCRIPTEN__
  if (result && fixedWait > 0)
  {
    _Device()->sleep(fixedWait);
  }
#endif
  const int msecsAfterWait = Millisecs();
  _delta = (msecsAfterWait - _lastMillisecs) / 1000.0f;
  _lastMillisecs = msecsAfterWait;
  return result;
}

void _SetDevice(IrrlichtDevice *device)
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
#ifndef EMSCRIPTEN
  AddZip("data.bin");
#endif
}

IrrlichtDevice *_Device()
{
  return _device;
}
