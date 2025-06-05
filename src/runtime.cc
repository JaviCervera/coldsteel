#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#define __DID_UNDEF__STRICT_ANSI__
#endif
#include <stdlib.h>
#define realpath(N, R) _fullpath((R), (N), MAX_PATH)
#ifdef __DID_UNDEF__STRICT_ANSI__
#define __STRICT_ANSI__
#endif
#endif
#include <sys/stat.h>
#include "internal/builder.h"
#include "internal/scripting.h"
#include "core.h"
#include "screen.h"
#include "string.h"

// ====================================
// Utility functions
// ====================================

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
#elif defined(__APPLE__) && defined(__MACH__)
  unsigned int size = FILENAME_MAX;
  _NSGetExecutablePath(path, &size);
#else
  path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
  return ExtractDir(path);
}

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

static bool IsDir(const stringc &path)
{
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) == -1)
    return false;
  return S_ISDIR(statbuf.st_mode);
}

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
      dir = RealDir(IsDir(dir) ? dir : ExtractDir(dir.c_str()));
    if (dir.findLast('\\') == dir.size() - 1 || dir.findLast('/') == dir.size() - 1)
      dir = dir.subString(0, dir.size() - 1);
#ifdef __APPLE__
    if (dir == "")
      dir = BinDir();
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
  Builder builder(PrintInfo);
  if (!builder.IsReady())
    Error("Could not initialize builder");
  builder.Build(dir.c_str(), precompile);
}

static void BuildWeb(const stringc &dir, bool precompile)
{
  Builder builder(PrintInfo);
  if (!builder.IsReady())
    Error("Could not initialize builder");
  builder.BuildWeb(dir.c_str(), precompile);
}

int main(int argc, char *argv[])
{
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
