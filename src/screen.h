/**
 * @file
 * Functions for creating and managing the application window and display settings. Use DRIVER_*
 * constants to select the graphics driver and SCREEN_* flags with OpenScreen.
 */
#pragma once

#include "common.h"

// Drivers
#define DRIVER_SOFTWAREFAST 0
#define DRIVER_SOFTWARE 1
#define DRIVER_OPENGL 2
#define DRIVER_DIRECT3D 3

// Flags
#define SCREEN_FULLSCREEN 1
#define SCREEN_RESIZABLE 2
#define SCREEN_VSYNC 4

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Sets the graphics driver to use when opening the screen. This must be called before OpenScreen.
   *
   * @param driver The graphics driver to use (DRIVER_*).
   */
  EXPORT void CALL SetDriver(int driver);

  /**
   * Returns the name of the current graphics driver.
   *
   * @return The name of the current graphics driver.
   */
  EXPORT const char *CALL DriverName();

  /**
   * Opens the display window with the given resolution, color depth, and flags.
   *
   * @param width The screen width in pixels.
   * @param height The screen height in pixels.
   * @param depth The color depth in bits.
   * @param flags Screen creation flags (SCREEN_*).
   */
  EXPORT void CALL OpenScreen(int width, int height, int depth, int flags);

  /**
   * Opens the screen with extended options.
   *
   * @param width The screen width in pixels.
   * @param height The screen height in pixels.
   * @param depth The color depth in bits.
   * @param flags Screen creation flags (SCREEN_*).
   * @param samples The MSAA sample count.
   * @param win An optional existing window handle, or NULL.
   */
  EXPORT void CALL OpenScreenEx(int width, int height, int depth, int flags, int samples, void *win);

  /**
   * Closes the display window.
   */
  EXPORT void CALL CloseScreen();

  /**
   * Clears the screen with the given color.
   *
   * @param color The color to clear the screen with.
   */
  EXPORT void CALL ClearScreen(int color);

  /**
   * Swaps the back buffer to display the rendered frame.
   */
  EXPORT void CALL RefreshScreen();

  /**
   * Sets the window title.
   *
   * @param caption The new window title.
   */
  EXPORT void CALL SetScreenTitle(const char *caption);

  /**
   * Resizes the window.
   *
   * @param width The new window width in pixels.
   * @param height The new window height in pixels.
   */
  EXPORT void CALL SetScreenSize(int width, int height);

  /**
   * Returns true if the window is currently focused.
   *
   * @return True if the window is currently focused.
   */
  EXPORT bool_t CALL ScreenActive();

  /**
   * Returns true if the user has requested to close the window.
   *
   * @return True if the user has requested to close the window.
   */
  EXPORT bool_t CALL ScreenShouldClose();

  /**
   * Returns the current screen width in pixels.
   *
   * @return The current screen width in pixels.
   */
  EXPORT int CALL ScreenWidth();

  /**
   * Returns the current screen height in pixels.
   *
   * @return The current screen height in pixels.
   */
  EXPORT int CALL ScreenHeight();

  /**
   * Limits the frame rate to the given frames per second.
   *
   * @param fps The frame rate limit in frames per second, or 0 for unlimited.
   */
  EXPORT void CALL SetScreenFPS(int fps);

  /**
   * Returns the current measured frames per second.
   *
   * @return The current measured frames per second.
   */
  EXPORT int CALL ScreenFPS();

  /**
   * Returns the number of available fullscreen display modes.
   *
   * @return The number of available fullscreen display modes.
   */
  EXPORT int CALL ScreenNumModes();

  /**
   * Returns the width of the display mode at the given index.
   *
   * @param index The display mode index.
   * @return The width of the display mode in pixels.
   */
  EXPORT int CALL ScreenModeWidth(int index);

  /**
   * Returns the height of the display mode at the given index.
   *
   * @param index The display mode index.
   * @return The height of the display mode in pixels.
   */
  EXPORT int CALL ScreenModeHeight(int index);

  /**
   * Returns the color depth of the display mode at the given index.
   *
   * @param index The display mode index.
   * @return The color depth of the display mode in bits.
   */
  EXPORT int CALL ScreenModeDepth(int index);

  /**
   * Returns the desktop width in pixels.
   *
   * @return The desktop width in pixels.
   */
  EXPORT int CALL DesktopWidth();

  /**
   * Returns the desktop height in pixels.
   *
   * @return The desktop height in pixels.
   */
  EXPORT int CALL DesktopHeight();

  /**
   * Returns the desktop color depth in bits.
   *
   * @return The desktop color depth in bits.
   */
  EXPORT int CALL DesktopDepth();

  /**
   * Minimizes the window.
   */
  EXPORT void CALL MinimizeWindow();

  /**
   * Maximizes the window.
   */
  EXPORT void CALL MaximizeWindow();

  /**
   * Restores the window to its normal size.
   */
  EXPORT void CALL RestoreWindow();

#ifndef SWIG
  int _ScreenFrameMsecs();
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
