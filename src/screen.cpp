#include "include/audio.h"
#include "include/engine.h"
#include "include/platform.h"
#include "screen.h"

extern "C"
{
  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags)
  {
    GetEngine().GetPlatform().OpenScreen(width, height, depth, flags, 0, NULL);
  }

  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win)
  {
    GetEngine().GetPlatform().OpenScreen(width, height, depth, flags, samples, win);
  }

  EXPORT void CALL CloseScreen()
  {
    GetEngine().GetPlatform().CloseScreen();
  }

  EXPORT void CALL ClearScreen(int color)
  {
    GetEngine().GetPlatform().ClearScreen(color);
  }

  EXPORT void CALL RefreshScreen()
  {
    GetEngine().GetAudio().Update();
    GetEngine().GetPlatform().RefreshScreen();
  }

  EXPORT void CALL SetScreenTitle(const char *caption)
  {
    GetEngine().GetPlatform().SetScreenTitle(caption);
  }

  EXPORT void CALL SetScreenSize(int width, int height)
  {
    GetEngine().GetPlatform().SetScreenSize(width, height);
  }

  EXPORT bool_t CALL ScreenActive()
  {
    return GetEngine().GetPlatform().ScreenActive();
  }

  EXPORT bool_t CALL ScreenShouldClose()
  {
    return GetEngine().GetPlatform().ScreenShouldClose();
  }

  EXPORT int CALL ScreenWidth()
  {
    return GetEngine().GetPlatform().ScreenWidth();
  }

  EXPORT int CALL ScreenHeight()
  {
    return GetEngine().GetPlatform().ScreenHeight();
  }

  EXPORT void CALL SetScreenFPS(int fps)
  {
    GetEngine().GetPlatform().SetScreenFPS(fps);
  }

  EXPORT int CALL ScreenFPS()
  {
    return GetEngine().GetPlatform().ScreenFPS();
  }

  EXPORT int CALL ScreenNumModes()
  {
    return GetEngine().GetPlatform().ScreenNumModes();
  }

  EXPORT int CALL ScreenModeWidth(int index)
  {
    return GetEngine().GetPlatform().ScreenModeWidth(index - 1);
  }

  EXPORT int CALL ScreenModeHeight(int index)
  {
    return GetEngine().GetPlatform().ScreenModeHeight(index - 1);
  }

  EXPORT int CALL ScreenModeDepth(int index)
  {
    return GetEngine().GetPlatform().ScreenModeDepth(index - 1);
  }

  EXPORT int CALL DesktopWidth()
  {
    return GetEngine().GetPlatform().DesktopWidth();
  }

  EXPORT int CALL DesktopHeight()
  {
    return GetEngine().GetPlatform().DesktopHeight();
  }

  EXPORT int CALL DesktopDepth()
  {
    return GetEngine().GetPlatform().DesktopDepth();
  }

  EXPORT bool_t CALL FeatureSupported(int feature)
  {
    return GetEngine().GetPlatform().FeatureSupported(feature);
  }

} // extern "C"
