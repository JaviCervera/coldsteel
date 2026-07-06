#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdio.h>
#ifdef _WIN32
extern "C" int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
#define MB_OK 0
#define MB_ICONERROR 0x00000010
#endif
#include "internal/builder.h"
#include "internal/editor.h"
#include "internal/scripting.h"
#include "internal/utils.h"
#include "core.h"
#include "screen.h"
#include "string.h"

static void Error(const char *msg)
{
  if (_Device())
    _Device()->getLogger()->log(msg, ELL_ERROR);
  else
    puts(msg);
#ifdef _WIN32
  MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONERROR);
#endif
  exit(-1);
}

// ====================================
// Runtime options
// ====================================

enum Mode
{
  MODE_RUN,
  MODE_BUILD,
  MODE_BUILD_WEB
};

struct Options
{
  Mode mode;
  stringc dir;
  bool precompile;
  bool no_args;

  Options(Mode mode, const stringc &dir, bool precompile, bool no_args)
      : mode(mode), dir(dir), precompile(precompile), no_args(no_args) {}

  static Options Parse(int argc, char *argv[])
  {
    if (argc != 1 && argc != 3)
      Error("Usage: coldsteel mode project_dir (mode=run,build,build_web)");
    const Mode mode = (argc > 1) ? ParseMode(argv[1]) : MODE_RUN;
    stringc dir = (argc > 2) ? argv[2] : "";
    if (dir != "")
    {
      dir = RealDir(IsDir(dir) ? dir : core::stringc(ExtractDir(dir.c_str())));
    }
    if (dir.findLast('\\') == dir.size() - 1 || dir.findLast('/') == dir.size() - 1)
      dir = dir.subString(0, dir.size() - 1);
#ifdef __APPLE__
    if (dir == "")
      dir = BinDir().c_str();
#endif
    return Options(mode, dir, true, argc == 1);
  }

private:
  static Mode ParseMode(const stringc &mode_name)
  {
    if (mode_name == "run")
      return MODE_RUN;
    if (mode_name == "build")
      return MODE_BUILD;
    if (mode_name == "build_web")
      return MODE_BUILD_WEB;
    Error((stringc("Invalid mode ") + mode_name + ". Accepted values: run, build, build_web").c_str());
    return MODE_RUN;
  }
};

// ====================================
// Main program
// ====================================

static void Run(bool no_args)
{
  if (!Scripting::Get().Load("main.lua"))
  {
    if (no_args)
    {
      Editor editor(BinDir().c_str());
      if (editor.IsReady()
          && editor.Init((ColdSteelSDK *)Scripting::Get().GetSDKPtr(), BinDir().c_str()))
      {
        editor.Run();
        RefreshScreen();
        CloseScreen();
        return;
      }
      puts((stringc("ColdSteel: ") + editor.LastError()).c_str());
    }
    Error(Scripting::Get().Error());
  }
  RefreshScreen();
  CloseScreen();
}

static void PrintInfo(const char *msg)
{
  puts(msg);
}

static void Build(const stringc &dir, bool precompile)
{
  Builder builder(PrintInfo, BinDir().c_str());
  if (!builder.IsReady())
    Error("Could not initialize builder");
  builder.Build(dir.c_str(), precompile);
}

static void BuildWeb(const stringc &dir, bool precompile)
{
  Builder builder(PrintInfo, BinDir().c_str());
  if (!builder.IsReady())
    Error("Could not initialize builder");
  builder.BuildWeb(dir.c_str(), precompile);
}

int main(
#ifdef __EMSCRIPTEN__
  void
#else
  int argc, char *argv[]
#endif
)
{
#ifdef __EMSCRIPTEN__
  int argc = 1;
  char *argv[] = {(char*)""};
#endif
  const Options opts = Options::Parse(argc, argv);
  _Init(opts.dir.c_str());
  switch (opts.mode)
  {
  case MODE_RUN:
    Run(opts.no_args);
    break;
  case MODE_BUILD:
    Build(opts.dir, opts.precompile);
    break;
  case MODE_BUILD_WEB:
    BuildWeb(opts.dir, opts.precompile);
    break;
  }
  _Finish();
  return 0;
}
