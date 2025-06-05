#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "string.hh"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

enum filetype_t
{
  funknown,
  ffile,
  fdir
};

inline filetype_t filetype(const char *filename)
{
  struct stat statbuf;
  if (stat(filename, &statbuf) == -1)
    return funknown;
  else if (S_ISDIR(statbuf.st_mode))
    return fdir;
  else
    return ffile;
}

inline size_t filesize(const char *filename)
{
  struct stat statbuf;
  if (stat(filename, &statbuf) == -1)
    return 0; // does not exist
  else
    return statbuf.st_size;
}

static void filecopy(const char *src, const char *dst)
{
#ifdef _WIN32
  CopyFileA(src, dst, false);
#else
  system((std::string("cp \"") + src + "\" \"" + dst + "\"").c_str());
#endif
}

inline void fileremove(const char *filename)
{
  remove(filename);
}
