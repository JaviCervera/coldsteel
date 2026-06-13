#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#define __DID_UNDEF__STRICT_ANSI__
#endif
#include <cstdlib>
#define realpath(N, R) _fullpath((R), (N), MAX_PATH)
#ifdef __DID_UNDEF__STRICT_ANSI__
#define __STRICT_ANSI__
#endif
#else
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

static std::string ExtractDir(const std::string &path)
{
  size_t pos = path.find_last_of("/\\");
  if (pos != std::string::npos)
    return path.substr(0, pos);
  return ".";
}

std::string RealDir(const std::string &dir)
{
  char out_dir[FILENAME_MAX];
  realpath(dir.c_str(), out_dir);
  return out_dir;
}

std::string BinDir()
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

bool IsDir(const std::string &path)
{
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) == -1)
    return false;
  return S_ISDIR(statbuf.st_mode);
}
