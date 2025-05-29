#include <stdlib.h>

inline const wchar_t *Wstr(const char *string)
{
  static wchar_t wstring[1024];
  mbstowcs(wstring, string, 1024);
  return wstring;
}

inline const char *Cstr(const wchar_t *wstring)
{
  static char string[1024];
  wcstombs(string, wstring, 1024);
  return string;
}
