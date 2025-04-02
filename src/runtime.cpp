#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#undef __STRICT_ANSI__
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define realpath(N, R) _fullpath((R), (N), MAX_PATH)
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

#include "core.h"
#include "dialogs.h"
#include "dir.h"
#include "screen.h"
#include "script.h"
#include "string.h"
#include "xml.h"

static stringc ParseWorkingDir(int argc, char *argv[]);
static stringc RealDir(const stringc &dir);
static stringc BinDir();
static void PrintError();
static void MainLoop();
static void EmscriptenMainLoop();

struct CompilerConfig
{
  int screenWidth;
  int screenHeight;
  int screenDepth;
  int screenFps;
  int screenSamples;
  int screenFlags;

  static CompilerConfig Parse(const stringc &filename)
  {
    CompilerConfig config = CompilerConfig();
    XMLNode *xml = ParseXML(filename.c_str());
    if (xml)
    {
      XMLNode *screenWidth = XMLChildNamed(xml, "screen_width", 1);
      XMLNode *screenHeight = XMLChildNamed(xml, "screen_height", 1);
      XMLNode *screenDepth = XMLChildNamed(xml, "screen_depth", 1);
      XMLNode *screenFps = XMLChildNamed(xml, "screen_fps", 1);
      XMLNode *screenSamples = XMLChildNamed(xml, "screen_samples", 1);
      XMLNode *screenFullscreen = XMLChildNamed(xml, "screen_fullscreen", 1);
      XMLNode *screenResizable = XMLChildNamed(xml, "screen_resizable", 1);
      XMLNode *screenVsync = XMLChildNamed(xml, "screen_vsync", 1);
      if (screenWidth)
        config.screenWidth = Val(XMLText(screenWidth));
      if (screenHeight)
        config.screenHeight = Val(XMLText(screenHeight));
      if (screenDepth)
        config.screenDepth = Val(XMLText(screenDepth));
      if (screenFps)
        config.screenFps = Val(XMLText(screenFps));
      if (screenSamples)
        config.screenSamples = Val(XMLText(screenSamples));
      if (stringc(XMLText(screenFullscreen)) == "true")
        config.screenFlags |= SCREEN_FULLSCREEN;
      if (stringc(XMLText(screenResizable)) == "true")
        config.screenFlags |= SCREEN_RESIZABLE;
      if (stringc(XMLText(screenVsync)) == "true")
        config.screenFlags |= SCREEN_VSYNC;
      FreeXML(xml);
    }
#ifdef EMSCRIPTEN
    if (config.screenWidth == 0)
      config.screenWidth = 640;
    if (config.screenHeight == 0)
      config.screenHeight = 480;
#endif
    return config;
  }
};

int main(int argc, char *argv[])
{
  stringc path = ParseWorkingDir(argc, argv);
#ifdef __APPLE__
  if (path == "")
    path = BinDir();
#endif
  _Init(path.c_str());
  const CompilerConfig config = CompilerConfig::Parse("config.xml");
  if (config.screenWidth != 0 && config.screenHeight != 0)
  {
    int screenDepth = (config.screenDepth != 0) ? config.screenDepth : DesktopDepth();
    _OpenScreenEx(config.screenWidth, config.screenHeight, screenDepth, config.screenFlags, config.screenSamples, NULL);
  }
  if (!Script::Get().Load("main.lua"))
  {
    PrintError();
    return -1;
  }
  if (Script::Get().FunctionExists("Loop"))
  {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(EmscriptenMainLoop, config.screenFps, true);
#else
    SetScreenFPS(config.screenFps);
    MainLoop();
#endif
  }
  else
  {
    _Run();
  }
  CloseScreen();
  _Finish();
  return 0;
}

static stringc ParseWorkingDir(int argc, char *argv[])
{
  return (argc > 1) ? RealDir(stringc(argv[1]) + "/") : "";
}

static stringc RealDir(const stringc &dir)
{
  char out_dir[FILENAME_MAX];
  realpath(dir.c_str(), out_dir);
  return out_dir;
}

static stringc BinDir()
{
  char path[FILENAME_MAX];
#if defined(_WIN32)
  path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__)
  unsigned int size = FILENAME_MAX;
  _NSGetExecutablePath(path, &size);
#else
  path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
  return ExtractDir(path);
}

static void PrintError()
{
  _Device()->getLogger()->log(Script::Get().Error().c_str(), ELL_ERROR);
  Notify("Error", Script::Get().Error().c_str(), true);
}

static void MainLoop()
{
  bool ok = true;
  while (_Run() && ok)
  {
    ok = Script::Get().CallVoidFunction("Loop");
  }
  if (!ok)
    PrintError();
}

#ifdef EMSCRIPTEN
static void EmscriptenMainLoop()
{
  if (_Run())
  {
    if (!Script::Get().CallVoidFunction("Loop"))
      PrintError();
  }
  else
  {
    emscripten_cancel_main_loop();
  }
}
#endif
