#include <irrlicht.h>
#undef RGB
#include "../color.h"
#include "../dir.h" // TODO: Remove this
#include "../include/platform.h"
#include "../math.h"
#include "../screen.h"
#include "irr_color.h"
#include "str.h"

struct Joystick
{
  irr::SJoystickInfo info;
  irr::SEvent::SJoystickEvent event;

  Joystick(const irr::SJoystickInfo &info) : info(info) {}
};

struct Platform_Irrlicht;

struct EventReceiver : public irr::IEventReceiver
{
  EventReceiver(Platform_Irrlicht &platform) : m_platform(&platform) {}

  bool OnEvent(const irr::SEvent &event);

private:
  Platform_Irrlicht *m_platform;
};

struct Platform_Irrlicht : public Platform
{
  Platform_Irrlicht(const char *working_dir) : m_last_mouse_x(0), m_last_mouse_y(0), m_wheel(0)
  {
    puts("Platform_Irrlicht");
    Init(working_dir);
  }

  ~Platform_Irrlicht()
  {
    Finish();
  }

  float DeltaTime() const
  {
    return m_delta;
  }

  int Millisecs() const
  {
    return m_device ? (m_device->getTimer()->getRealTime() - m_init_msecs) : 0;
  }

  void Exit(int code)
  {
    Finish();
    exit(code);
  }

  int System(const char *command) const
  {
    return system(command);
  }

  void *InternalHandle()
  {
    return m_device;
  }

  void OpenScreen(int width, int height, int depth, int flags, int samples, void *win)
  {
    puts("OpenScreen");
    Finish();

    irr::SIrrlichtCreationParameters params;
    params.AntiAlias = samples;
    params.Bits = depth;
#ifdef EMSCRIPTEN
    params.DriverType = irr::video::EDT_WEBGL1;
    params.LoggingLevel = irr::ELL_DEBUG;
#else
    params.DriverType = irr::video::EDT_OPENGL;
    params.LoggingLevel = irr::ELL_ERROR;
#endif
    params.EventReceiver = new EventReceiver(*this); // TODO: This leaks
    params.Fullscreen = (flags & SCREEN_FULLSCREEN) == SCREEN_FULLSCREEN;
    params.Stencilbuffer = true;
    params.Vsync = (flags & SCREEN_VSYNC) == SCREEN_VSYNC;
    params.WindowId = win;
    params.WindowSize.Width = width;
    params.WindowSize.Height = height;

    // Init device
    m_device = createDeviceEx(params);
    m_device->setResizable((flags & SCREEN_RESIZABLE) == SCREEN_RESIZABLE);
    m_device->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
    m_device->getSceneManager()->setAmbientLight(IrrColor(RGB(255, 255, 255)));
    Init(NULL);

    m_device->getVideoDriver()->beginScene(false, false);
  }

  void CloseScreen()
  {
    Finish();
    Init(NULL);
  }

  void ClearScreen(int color)
  {
    m_device->getVideoDriver()->setRenderTarget(NULL, true, false, IrrColor(color));
  }

  void RefreshScreen()
  {
    if (!m_device)
      return;
    ClearInputs();
    m_device->getVideoDriver()->endScene();
    m_should_close = !m_device->run() || !m_device->getVideoDriver();
    const int msecs = Millisecs();
    const int delta_msecs = msecs - m_last_msecs;
    const int wait = m_screen_frame_msecs - delta_msecs;
    const int fixed_wait = (wait > 0) ? wait : 0;
#ifndef EMSCRIPTEN
    if (!m_should_close && fixed_wait > 0)
    {
      m_device->sleep(fixed_wait);
    }
#else
    emscripten_sleep(fixed_wait);
#endif
    const int msecs_after_wait = Millisecs();
    m_delta = (msecs_after_wait - m_last_msecs) / 1000.0f;
    m_last_msecs = msecs_after_wait;
    m_device->getVideoDriver()->beginScene(false, false);
  }

  void SetScreenTitle(const char *caption)
  {
    m_device->setWindowCaption(Wstr(caption));
  }

  void SetScreenSize(int width, int height)
  {
    m_device->getVideoDriver()->OnResize(irr::core::dimension2du(width, height));
  }

  bool ScreenActive() const
  {
    return m_device->isWindowActive();
  }

  bool ScreenShouldClose() const
  {
    return m_should_close;
  }

  int ScreenWidth() const
  {
    return m_device->getVideoDriver()->getScreenSize().Width;
  }

  int ScreenHeight() const
  {
    return m_device->getVideoDriver()->getScreenSize().Height;
  }

  void SetScreenFPS(int fps)
  {
    m_screen_frame_msecs = (fps > 0) ? (1000 / fps) : 0;
  }

  int ScreenFPS() const
  {
    return m_device->getVideoDriver()->getFPS();
  }

  int ScreenNumModes() const
  {
    return m_device->getVideoModeList()->getVideoModeCount();
  }

  int ScreenModeWidth(int index) const
  {
    return m_device->getVideoModeList()->getVideoModeResolution(index).Width;
  }

  int ScreenModeHeight(int index) const
  {
    return m_device->getVideoModeList()->getVideoModeResolution(index).Height;
  }

  int ScreenModeDepth(int index) const
  {
    return m_device->getVideoModeList()->getVideoModeDepth(index);
  }

  int DesktopWidth() const
  {
    return m_device->getVideoModeList()->getDesktopResolution().Width;
  }

  int DesktopHeight() const
  {
    return m_device->getVideoModeList()->getDesktopResolution().Height;
  }

  int DesktopDepth() const
  {
    return m_device->getVideoModeList()->getDesktopDepth();
  }

  bool FeatureSupported(int feature) const
  {
    return m_device->getVideoDriver()->queryFeature(E_VIDEO_DRIVER_FEATURE(feature));
  }

  void SetCursorVisible(bool visible)
  {
    return m_device->getCursorControl()->setVisible(visible);
  }

  void SetCursorPosition(int x, int y)
  {
    m_device->getCursorControl()->setPosition(x, y);
  }

  int CursorX() const
  {
    return m_device->getCursorControl()->getPosition().X;
  }

  int CursorY() const
  {
    return m_device->getCursorControl()->getPosition().Y;
  }

  int CursorZ() const
  {
    return m_wheel;
  }

  bool ButtonDown(int button) const
  {
    return m_mouse_buttons[button];
  }

  bool ButtonHit(int button) const
  {
    return m_mouse_buttons_hit[button];
  }

  bool KeyDown(int key) const
  {
    return m_keys[key];
  }

  bool KeyHit(int key) const
  {
    return m_keys_hit[key];
  }

  int NumJoysticks() const
  {
    return m_joysticks.size();
  }

  const char *JoystickName(int index) const
  {
    return m_joysticks[index].info.Name.c_str();
  }

  int NumJoystickAxes(int index) const
  {
    return m_joysticks[index].info.Axes;
  }

  int NumJoystickButtons(int index) const
  {
    return m_joysticks[index].info.Buttons;
  }

  bool JoystickButtonDown(int index, int button) const
  {
    return m_joysticks[index].event.IsButtonPressed(button);
  }

  float JoystickAxis(int index, int axis) const
  {
    const float value = m_joysticks[index].event.Axis[axis] / 32768.0f;
    const float fixed_value = (Abs(value) <= 0.1f) ? 0.0f : (value >= 0.995f) ? 1.0f
                                                                              : value;
    return fixed_value;
  }

private:
  irr::IrrlichtDevice *m_device;
  irr::u32 m_init_msecs;
  int m_last_msecs;
  float m_delta;
  int m_screen_frame_msecs;
  bool m_should_close;

  int m_mouse_x_speed, m_mouse_y_speed;
  int m_last_mouse_x, m_last_mouse_y;
  int m_wheel;
  bool m_mouse_buttons[3];
  bool m_mouse_buttons_hit[3];
  bool m_mouse_buttons_was_hit[3];
  bool m_keys[irr::KEY_KEY_CODES_COUNT];
  bool m_keys_hit[irr::KEY_KEY_CODES_COUNT];
  bool m_keys_was_hit[irr::KEY_KEY_CODES_COUNT];
  irr::core::array<Joystick> m_joysticks;

  void Init(const char *working_dir)
  {
    if (!m_device)
    {
      irr::SIrrlichtCreationParameters params;
      params.DriverType = irr::video::EDT_NULL;
      params.LoggingLevel = irr::ELL_ERROR;
      m_device = createDeviceEx(params);
    }
    InitJoysticks();
    m_init_msecs = m_device->getTimer()->getRealTime();
    m_last_msecs = 0;
    m_delta = 0.0f;
    if (working_dir && strcmp(working_dir, ""))
      ChangeDir(working_dir);
#ifndef EMSCRIPTEN
    AddZip("data.bin");
#endif
  }

  void Finish()
  {
    if (m_device)
      m_device->drop();
    m_device = NULL;
  }

  void SetCursorMoved()
  {
    m_mouse_x_speed = CursorX() - m_last_mouse_x;
    m_mouse_y_speed = CursorY() - m_last_mouse_y;
    m_last_mouse_x = CursorX();
    m_last_mouse_y = CursorY();
  }

  void SetButtonDown(int button, bool down)
  {
    if (down && !m_mouse_buttons_was_hit[button])
      m_mouse_buttons_hit[button] = down;
    m_mouse_buttons_was_hit[button] = down;
    m_mouse_buttons[button] = down;
  }

  void IncCursorWheel(int wheel)
  {
    m_wheel += wheel;
  }

  void ClearInputs()
  {
    memset(m_keys_hit, false, irr::KEY_KEY_CODES_COUNT * sizeof(bool));
    memset(m_mouse_buttons_hit, false, 3 * sizeof(bool));
  }

  void InitJoysticks()
  {
    irr::core::array<irr::SJoystickInfo> joysticks;
    m_device->activateJoysticks(joysticks);
    m_joysticks.clear();
    for (irr::u32 i = 0; i < joysticks.size(); ++i)
      m_joysticks.push_back(Joystick(joysticks[i]));
  }

  int FindJoystickIndex(irr::u8 id) const
  {
    for (irr::u32 i = 0; i < m_joysticks.size(); ++i)
      if (m_joysticks[i].info.Joystick == id)
        return i;
    return -1;
  }

  friend class EventReceiver;
};

inline bool EventReceiver::OnEvent(const irr::SEvent &event)
{
  switch (event.EventType)
  {
  case irr::EET_JOYSTICK_INPUT_EVENT:
  {
    int index = m_platform->FindJoystickIndex(event.JoystickEvent.Joystick);
    if (index != -1)
      m_platform->m_joysticks[index].event = event.JoystickEvent;
    break;
  }
  case irr::EET_KEY_INPUT_EVENT:
  {
    if (event.KeyInput.PressedDown && !m_platform->m_keys_was_hit[event.KeyInput.Key])
      m_platform->m_keys_hit[event.KeyInput.Key] = true;
    m_platform->m_keys_was_hit[event.KeyInput.Key] = event.KeyInput.PressedDown;
    m_platform->m_keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
    break;
  }
  case irr::EET_MOUSE_INPUT_EVENT:
    switch (event.MouseInput.Event)
    {
    case irr::EMIE_MOUSE_MOVED:
      m_platform->SetCursorMoved();
      break;
    case irr::EMIE_LMOUSE_PRESSED_DOWN:
      m_platform->SetButtonDown(1, true);
      break;
    case irr::EMIE_RMOUSE_PRESSED_DOWN:
      m_platform->SetButtonDown(2, true);
      break;
    case irr::EMIE_MMOUSE_PRESSED_DOWN:
      m_platform->SetButtonDown(3, true);
      break;
    case irr::EMIE_LMOUSE_LEFT_UP:
      m_platform->SetButtonDown(1, false);
      break;
    case irr::EMIE_RMOUSE_LEFT_UP:
      m_platform->SetButtonDown(2, false);
      break;
    case irr::EMIE_MMOUSE_LEFT_UP:
      m_platform->SetButtonDown(3, false);
      break;
    case irr::EMIE_MOUSE_WHEEL:
      m_platform->IncCursorWheel(int(event.MouseInput.Wheel));
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
