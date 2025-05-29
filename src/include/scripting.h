#pragma once

#include <stdio.h>

struct Scripting
{
  static Scripting &Get();
  virtual bool Run(const char *filename, const char *contents, size_t size) = 0;
  virtual const char *Error() const = 0;
  virtual FILE *CreateCompiledFile(const char *file) = 0;
};
