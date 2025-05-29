#pragma once

struct Platform
{
  static Platform &Get();

  // Core
  virtual void Init(const char *working_dir) = 0;
  virtual void Finish() = 0;
  virtual float DeltaTime() const = 0;
  virtual int Millisecs() const = 0;
  virtual void Exit(int code) = 0;
  virtual int System(const char *command) const = 0;
  virtual void *InternalHandle() = 0;

  // Screen
  virtual void OpenScreen(int width, int height, int depth, int flags, int samples, void *win) = 0;
  virtual void CloseScreen() = 0;
  virtual void ClearScreen(int color) = 0;
  virtual void RefreshScreen() = 0;
  virtual void SetScreenTitle(const char *caption) = 0;
  virtual void SetScreenSize(int width, int height) = 0;
  virtual bool ScreenActive() const = 0;
  virtual bool ScreenShouldClose() const = 0;
  virtual int ScreenWidth() const = 0;
  virtual int ScreenHeight() const = 0;
  virtual void SetScreenFPS(int fps) = 0;
  virtual int ScreenFPS() const = 0;
  virtual int ScreenNumModes() const = 0;
  virtual int ScreenModeWidth(int index) const = 0;
  virtual int ScreenModeHeight(int index) const = 0;
  virtual int ScreenModeDepth(int index) const = 0;
  virtual int DesktopWidth() const = 0;
  virtual int DesktopHeight() const = 0;
  virtual int DesktopDepth() const = 0;
  virtual bool FeatureSupported(int feature) const = 0;

  // Input
  virtual void SetCursorVisible(bool visible) = 0;
  virtual void SetCursorPosition(int x, int y) = 0;
  virtual int CursorX() const = 0;
  virtual int CursorY() const = 0;
  virtual int CursorZ() const = 0;
  virtual bool ButtonDown(int button) const = 0;
  virtual bool ButtonHit(int button) const = 0;
  virtual bool KeyDown(int key) const = 0;
  virtual bool KeyHit(int key) const = 0;
  virtual int NumJoysticks() const = 0;
  virtual const char *JoystickName(int index) const = 0;
  virtual int NumJoystickAxes(int index) const = 0;
  virtual int NumJoystickButtons(int index) const = 0;
  virtual bool JoystickButtonDown(int index, int button) const = 0;
  virtual float JoystickAxis(int index, int axis) const = 0;
};
