#pragma once

class Scripting
{
public:
  static Scripting &Get();
  virtual bool Load(const char *filename) = 0;
  virtual const char *Error() const = 0;
};
