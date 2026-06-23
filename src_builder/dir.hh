// note: on visual studio, if you include this file dirent.h must be present

#pragma once

#include <irrString.h>
#include <irrArray.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#define _getcwd getcwd
#define _chdir chdir
#define _mkdir mkdir
#define _rmdir rmdir
#else
#include <direct.h>
#include "dirent.h"
#endif

#ifdef _WIN32
#ifndef realpath
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#endif
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace dir
{
  inline irr::core::array<irr::core::stringc> contents(const irr::core::stringc &path)
  {
    irr::core::array<irr::core::stringc> arr;

    DIR *d = (DIR *)opendir(path.c_str());
    if (d == NULL)
      return arr;

    struct dirent *entry;
    while ((entry = (struct dirent *)readdir(d)))
    {
      arr.push_back(irr::core::stringc(entry->d_name));
    }

    closedir(d);

    return arr;
  }

  inline irr::core::stringc current()
  {
    char buf[FILENAME_MAX];
    _getcwd(buf, FILENAME_MAX);
    return irr::core::stringc(buf);
  }

  inline bool change(const irr::core::stringc &path)
  {
    return _chdir(path.c_str()) == 0;
  }

  inline void create(const irr::core::stringc &path)
  {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != -1 && S_ISDIR(statbuf.st_mode))
      return;
    irr::s32 pos = path.findLast('/');
    if (pos != -1 && pos > 0)
      create(path.subString(0, pos));
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    _mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
#endif
  }

  inline void remove(const irr::core::stringc &path)
  {
    _rmdir(path.c_str());
  }

  inline irr::core::stringc real_path(const irr::core::stringc &path)
  {
    char out_path[FILENAME_MAX];
    realpath(path.c_str(), out_path);
    return irr::core::stringc(out_path);
  }

  static irr::core::stringc bin_filename()
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
    return irr::core::stringc(path);
  }
} // namespace dir
