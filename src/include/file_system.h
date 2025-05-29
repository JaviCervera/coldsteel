#pragma once

#include <stddef.h>

struct FileSystem
{
  virtual bool AddZip(const char *filename) = 0;
  virtual const char *DirContents(const char *dir) const = 0;
  virtual void ChangeDir(const char *dir) = 0;
  virtual const char *CurrentDir() const = 0;
  virtual size_t FileSize(const char *filename) const = 0;
  virtual size_t ReadFile(const char *filename, void *buffer, size_t max_size) const = 0;
  virtual bool WriteFile(const char *filename, void *buffer, size_t size, bool append = false) = 0;
};
