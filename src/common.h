#pragma once

#ifdef _WIN32
#if defined(DLLEXPORT)
#define EXPORT /*__declspec(dllexport)*/
#elif defined(DLLIMPORT)
#define EXPORT /*__declspec(dllimport)*/
#else
#define EXPORT
#endif
#define CALL /*__stdcall*/
#else
#define EXPORT
#define CALL
#endif

#ifndef SWIG
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#endif

#define TRUE 1
#define FALSE 0

#ifndef SWIG

typedef int bool_t;

inline const wchar_t *_WSTR(const char *string)
{
  static wchar_t wstring[1024];
  mbstowcs(wstring, string, 1024);
  return wstring;
}

inline const char *_CSTR(const wchar_t *wstring)
{
  static char string[1024];
  wcstombs(string, wstring, 1024);
  return string;
}

#endif
