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
#include "driver/scripting.h"
#include "screen.h"
#include "string.h"
#include "xml.h"

static stringc ParseWorkingDir(int argc, char *argv[]);
static stringc RealDir(const stringc &dir);
static stringc BinDir();
static void PrintError();

int main(int argc, char *argv[])
{
  stringc path = ParseWorkingDir(argc, argv);
#ifdef __APPLE__
  if (path == "")
    path = BinDir();
#endif
  _Init(path.c_str());
  if (!Scripting::Get().Load("main.lua"))
  {
    PrintError();
    return -1;
  }
  RefreshScreen();
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
  _Device()->getLogger()->log(Scripting::Get().Error(), ELL_ERROR);
  Notify("Error", Scripting::Get().Error(), true);
}
