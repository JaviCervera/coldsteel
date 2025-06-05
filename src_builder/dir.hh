// note: on visual studio, if you include this file dirent.h must be present

#pragma once

#include "string.hh"
#include "vector.hh"
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

namespace dir
{
  inline std::vector<std::string> contents(const std::string &path)
  {
    std::vector<std::string> arr;

    // open directory
    DIR *d = (DIR *)opendir(path.c_str());
    if (d == NULL)
      return arr;

    // copy directory contents
    struct dirent *entry;
    while ((entry = (struct dirent *)readdir(d)))
    {
      arr.push_back(entry->d_name);
    }

    // close directory
    closedir(d);

    return arr;
  }

  inline std::string current()
  {
    char buf[FILENAME_MAX];
    _getcwd(buf, FILENAME_MAX);
    return std::string(buf);
  }

  inline bool change(const std::string &path)
  {
    return _chdir(path.c_str()) == 0;
  }

  inline void create(const std::string &path)
  {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    _mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
#endif
  }

  inline void remove(const std::string &path)
  {
    _rmdir(path.c_str());
  }

  inline std::string real_path(const std::string &path)
  {
    char out_path[FILENAME_MAX];
    realpath(path.c_str(), out_path);
    return std::string(out_path);
  }

  static std::string bin_filename()
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
    return path;
  }
} // namespace dir
