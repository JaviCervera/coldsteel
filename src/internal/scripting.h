#pragma once

#include <stdio.h>

struct Scripting
{
  static Scripting &Get();
  virtual bool Load(const char *filename) = 0;
  virtual const char *Error() const = 0;
  virtual FILE *CreateCompiledFile(const char *file) = 0;
};
