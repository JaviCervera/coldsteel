#include "color.h"
#include "core.h"
#include "input.h"
#include "listener.h"
#include "screen.h"

class EventReceiver : public IEventReceiver
{
  virtual bool OnEvent(const SEvent &event)
  {
    switch (event.EventType)
    {
    case EET_JOYSTICK_INPUT_EVENT:
      _SetJoystickEvent(event.JoystickEvent);
      break;
    case EET_KEY_INPUT_EVENT:
      _SetKeyDown(event.KeyInput.Key, event.KeyInput.PressedDown);
      break;
    case EET_MOUSE_INPUT_EVENT:
      switch (event.MouseInput.Event)
      {
      case EMIE_MOUSE_MOVED:
        _SetCursorMoved();
        break;
      case EMIE_LMOUSE_PRESSED_DOWN:
        _SetButtonDown(BUTTON_LEFT, true);
        break;
      case EMIE_RMOUSE_PRESSED_DOWN:
        _SetButtonDown(BUTTON_RIGHT, true);
        break;
      case EMIE_MMOUSE_PRESSED_DOWN:
        _SetButtonDown(BUTTON_MIDDLE, true);
        break;
      case EMIE_LMOUSE_LEFT_UP:
        _SetButtonDown(BUTTON_LEFT, false);
        break;
      case EMIE_RMOUSE_LEFT_UP:
        _SetButtonDown(BUTTON_RIGHT, false);
        break;
      case EMIE_MMOUSE_LEFT_UP:
        _SetButtonDown(BUTTON_MIDDLE, false);
        break;
      case EMIE_MOUSE_WHEEL:
        _IncCursorWheel(int(event.MouseInput.Wheel));
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
    return false;
  }
};

static EventReceiver _eventReceiver;
static int _selectedDriver = DRIVER_OPENGL;
static int _screenFrameMsecs = 0;
static bool_t _run = true;

static E_DRIVER_TYPE irrlichtDriver(int driver)
{
  switch (driver)
  {
  case DRIVER_SOFTWAREFAST:
    return EDT_SOFTWARE;
  case DRIVER_SOFTWARE:
    return EDT_BURNINGSVIDEO;
  case DRIVER_OPENGL:
#ifdef EMSCRIPTEN
    return EDT_WEBGL1;
#else
    return EDT_OPENGL;
#endif
  case DRIVER_DIRECT3D:
    return EDT_DIRECT3D8;
  default:
    return EDT_BURNINGSVIDEO;
  }
}

extern "C"
{

  EXPORT void CALL SetDriver(int driver)
  {
    if (driver >= DRIVER_SOFTWAREFAST && driver <= DRIVER_DIRECT3D)
      _selectedDriver = driver;
  }

  EXPORT const char *CALL DriverName()
  {
    static stringc driverName;
    switch (_Device()->getVideoDriver()->getDriverType())
    {
    case EDT_DIRECT3D9:
      driverName = "Direct3D9";
      break;
    case EDT_DIRECT3D8:
      driverName = "Direct3D8";
      break;
    case EDT_OPENGL:
      driverName = "OpenGL";
      break;
#ifdef IRRLICHT_SVN
    case EDT_OGLES1:
      driverName = "GLES1";
      break;
    case EDT_OGLES2:
      driverName = "GLES2";
      break;
    case EDT_WEBGL1:
      driverName = "WebGL1";
      break;
#endif
    case EDT_BURNINGSVIDEO:
      driverName = "Software";
      break;
    case EDT_SOFTWARE:
      driverName = "SoftwareFast";
      break;
    default:
      driverName = "Unknown";
      break;
    }
    return driverName.c_str();
  }

  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags)
  {
    OpenScreenEx(width, height, depth, flags, 0, NULL);
  }

  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win)
  {
    SIrrlichtCreationParameters params;
    params.DriverType = irrlichtDriver(_selectedDriver);
    params.AntiAlias = samples;
    params.Bits = depth;
#ifdef EMSCRIPTEN
    params.LoggingLevel = ELL_DEBUG;
#else
    params.LoggingLevel = ELL_ERROR;
#endif
    params.EventReceiver = &_eventReceiver;
    params.Fullscreen = (flags & SCREEN_FULLSCREEN) == SCREEN_FULLSCREEN;
    params.Stencilbuffer = true;
    params.Vsync = (flags & SCREEN_VSYNC) == SCREEN_VSYNC;
    params.WindowId = win;
    params.WindowSize.Width = width;
    params.WindowSize.Height = height;

    // Init device
    IrrlichtDevice *device = createDeviceEx(params);
    if (device)
    {
      _SetDevice(device, NULL);
      _Device()->setResizable((flags & SCREEN_RESIZABLE) == SCREEN_RESIZABLE);
      _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
      _Device()->getSceneManager()->setAmbientLight(_Color(RGB(255, 255, 255)));
      _Device()->getVideoDriver()->beginScene(false, false);
      _run = true;
    }
    else if (_selectedDriver > 0)
    {
      _selectedDriver--;
      OpenScreenEx(width, height, depth, flags, samples, win);
    }
  }

  EXPORT void CALL CloseScreen()
  {
    _SetDevice(NULL, NULL);
  }

  EXPORT void CALL ClearScreen(int color)
  {
    _Device()->getVideoDriver()->setRenderTarget(NULL, true, false, _Color(color));
  }

  EXPORT void CALL RefreshScreen()
  {
    _Device()->getVideoDriver()->endScene();
    _run = _Run();
    _Device()->getVideoDriver()->beginScene(false, false);
  }

  EXPORT void CALL SetScreenTitle(const char *caption)
  {
    _Device()->setWindowCaption(_WSTR(caption));
  }

  EXPORT void CALL SetScreenSize(int width, int height)
  {
    _Device()->getVideoDriver()->OnResize(dimension2du(width, height));
  }

  EXPORT bool_t CALL ScreenActive()
  {
    return _Device()->isWindowActive();
  }

  EXPORT bool_t CALL ScreenShouldClose()
  {
    return !_run;
  }

  EXPORT int CALL ScreenWidth()
  {
    return _Device()->getVideoDriver()->getScreenSize().Width;
  }

  EXPORT int CALL ScreenHeight()
  {
    return _Device()->getVideoDriver()->getScreenSize().Height;
  }

  EXPORT void CALL SetScreenFPS(int fps)
  {
    _screenFrameMsecs = (fps > 0)
                            ? (1000 / fps)
                            : 0;
  }

  EXPORT int CALL ScreenFPS()
  {
    return _Device()->getVideoDriver()->getFPS();
  }

  EXPORT int CALL ScreenNumModes()
  {
    return _Device()->getVideoModeList()->getVideoModeCount();
  }

  EXPORT int CALL ScreenModeWidth(int index)
  {
    return _Device()->getVideoModeList()->getVideoModeResolution(index - 1).Width;
  }

  EXPORT int CALL ScreenModeHeight(int index)
  {
    return _Device()->getVideoModeList()->getVideoModeResolution(index - 1).Height;
  }

  EXPORT int CALL ScreenModeDepth(int index)
  {
    return _Device()->getVideoModeList()->getVideoModeDepth(index - 1);
  }

  EXPORT int CALL DesktopWidth()
  {
    return _Device()->getVideoModeList()->getDesktopResolution().Width;
  }

  EXPORT int CALL DesktopHeight()
  {
    return _Device()->getVideoModeList()->getDesktopResolution().Height;
  }

  EXPORT int CALL DesktopDepth()
  {
    return _Device()->getVideoModeList()->getDesktopDepth();
  }

  EXPORT void CALL MinimizeWindow()
  {
    _Device()->minimizeWindow();
  }

  EXPORT void CALL MaximizeWindow()
  {
    _Device()->maximizeWindow();
  }

  EXPORT void CALL RestoreWindow()
  {
    _Device()->restoreWindow();
  }

  int _ScreenFrameMsecs()
  {
    return _screenFrameMsecs;
  }

} // extern "C"
