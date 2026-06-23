#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdio.h>
#include "internal/builder.h"
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

  Options(Mode mode, const stringc &dir, bool precompile)
      : mode(mode), dir(dir), precompile(precompile) {}

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
    return Options(mode, dir, true);
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

static void Run()
{
  if (!Scripting::Get().Load("main.lua"))
    Error(Scripting::Get().Error());
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
    Run();
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
