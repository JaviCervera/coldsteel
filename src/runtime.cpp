#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "dir.h"
#include "screen.h"
#include "script.h"
#include "string.h"
#include "xml.h"

struct CompilerConfig
{
  stringc sourceFilename;
  stringc path;
  int screenWidth;
  int screenHeight;
  int screenDepth;
  int screenFps;
  int screenSamples;
  int screenFlags;

  CompilerConfig(const stringc &path)
      : sourceFilename("main.lua"),
        path(path),
        screenWidth(0),
        screenHeight(0),
        screenDepth(0),
        screenFps(0),
        screenSamples(0),
        screenFlags(0) {}
};

static stringc GetBinDir();
static CompilerConfig ParseConfig(int argc, char *argv[]);
static void PrintError();
static void MainLoop();
static void EmscriptenMainLoop();

int main(int argc, char *argv[])
{
  _Init();
  const CompilerConfig config = ParseConfig(argc, argv);
#ifdef __APPLE__
  ChangeDir(GetBinDir().c_str());
#endif
  if (config.path != "")
    ChangeDir(config.path.c_str());
  if (config.screenWidth != 0 && config.screenHeight != 0)
  {
    int screenDepth = (config.screenDepth != 0) ? config.screenDepth : DesktopDepth();
    OpenScreenEx(config.screenWidth, config.screenHeight, screenDepth, config.screenFlags, config.screenSamples, NULL);
  }
  if (!Script::Get().Load(config.sourceFilename))
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

static stringc GetBinDir()
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

static CompilerConfig ParseConfig(int argc, char *argv[])
{
  CompilerConfig config((argc > 1) ? (stringc(argv[1]) + "/") : "");
  XMLNode *xml = ParseXML((config.path + "config.xml").c_str());
  if (xml)
  {
    XMLNode *screenWidth = XMLChildNamed(xml, "screen_width", 0);
    XMLNode *screenHeight = XMLChildNamed(xml, "screen_height", 0);
    XMLNode *screenDepth = XMLChildNamed(xml, "screen_depth", 0);
    XMLNode *screenFps = XMLChildNamed(xml, "screen_fps", 0);
    XMLNode *screenSamples = XMLChildNamed(xml, "screen_samples", 0);
    XMLNode *screenFullscreen = XMLChildNamed(xml, "screen_fullscreen", 0);
    XMLNode *screenResizable = XMLChildNamed(xml, "screen_resizable", 0);
    XMLNode *screenVsync = XMLChildNamed(xml, "screen_vsync", 0);
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
#ifdef EMSCRIPTEN
    if (config.screenWidth == 0)
      config.screenWidth = 640;
    if (config.screenHeight == 0)
      config.screenHeight = 480;
#endif
  }
  return config;
}

static void PrintError()
{
  _Device()->getLogger()->log(Script::Get().Error().c_str(), ELL_ERROR);
#ifdef _WIN32
  MessageBoxA(NULL, Script::Get().Error().c_str(), "Error", MB_OK | MB_ICONERROR);
#endif
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

/*
static CompilerConfig ParseCommandLine(int argc, char* argv[]) {
    CompilerConfig config;
    config.mode = MODE_RUN;
    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i], "-check") == 0) {
            config.mode = MODE_CHECK;
        } else if (strcmp(argv[i], "-build") == 0) {
            config.mode = MODE_BUILD;
        } else if (strcmp(argv[i], "-help") == 0) {
            printf("Usage: runtime [options] [sourcefile]\n");
            printf("Options:\n");
            printf(" -run: Run program (default).\n");
            printf(" -check: Check syntax (do not generate output).\n");
            printf(" -build: Build 'code.bcd' bytecode file.\n");
            printf(" -help: Prints this text.\n");
            printf("When called without arguments, the file 'code.bcd' will be executed.\n");
            exit(-1);
        } else if (strcmp(argv[i], "-run") != 0) {
            printf("Unrecognized argument: %s\n", argv[i]);
            exit(-1);
        }
    }
    config.sourceFilename = (argc > 1) ? argv[argc-1] : "";
    const int lastPathPos = config.sourceFilename.replace("\\", "/").findLast('/');
    config.path = (lastPathPos != -1)
        ? config.sourceFilename.subString(0, lastPathPos)
        : "";
    return config;
}
*/
