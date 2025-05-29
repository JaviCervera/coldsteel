#include "internal/platform.h"
#include "screen.h"

extern "C"
{
  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags)
  {
    Platform::Get().OpenScreen(width, height, depth, flags, 0, NULL);
  }

  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win)
  {
    Platform::Get().OpenScreen(width, height, depth, flags, samples, win);
  }

  EXPORT void CALL CloseScreen()
  {
    Platform::Get().CloseScreen();
  }

  EXPORT void CALL ClearScreen(int color)
  {
    Platform::Get().ClearScreen(color);
  }

  EXPORT void CALL RefreshScreen()
  {
    Platform::Get().RefreshScreen();
  }

  EXPORT void CALL SetScreenTitle(const char *caption)
  {
    Platform::Get().SetScreenTitle(caption);
  }

  EXPORT void CALL SetScreenSize(int width, int height)
  {
    Platform::Get().SetScreenSize(width, height);
  }

  EXPORT bool_t CALL ScreenActive()
  {
    return Platform::Get().ScreenActive();
  }

  EXPORT bool_t CALL ScreenShouldClose()
  {
    return Platform::Get().ScreenShouldClose();
  }

  EXPORT int CALL ScreenWidth()
  {
    return Platform::Get().ScreenWidth();
  }

  EXPORT int CALL ScreenHeight()
  {
    return Platform::Get().ScreenHeight();
  }

  EXPORT void CALL SetScreenFPS(int fps)
  {
    Platform::Get().SetScreenFPS(fps);
  }

  EXPORT int CALL ScreenFPS()
  {
    return Platform::Get().ScreenFPS();
  }

  EXPORT int CALL ScreenNumModes()
  {
    return Platform::Get().ScreenNumModes();
  }

  EXPORT int CALL ScreenModeWidth(int index)
  {
    return Platform::Get().ScreenModeWidth(index - 1);
  }

  EXPORT int CALL ScreenModeHeight(int index)
  {
    return Platform::Get().ScreenModeHeight(index - 1);
  }

  EXPORT int CALL ScreenModeDepth(int index)
  {
    return Platform::Get().ScreenModeDepth(index - 1);
  }

  EXPORT int CALL DesktopWidth()
  {
    return Platform::Get().DesktopWidth();
  }

  EXPORT int CALL DesktopHeight()
  {
    return Platform::Get().DesktopHeight();
  }

  EXPORT int CALL DesktopDepth()
  {
    return Platform::Get().DesktopDepth();
  }

  EXPORT bool_t CALL FeatureSupported(int feature)
  {
    return Platform::Get().FeatureSupported(feature);
  }

} // extern "C"
