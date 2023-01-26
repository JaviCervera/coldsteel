#pragma once

#include <string>

#ifndef __stdcall
#define __stdcall
#endif

#ifdef _WIN32
extern "C" void* __stdcall LoadLibraryA(const char*);
extern "C" void* __stdcall GetProcAddress(const void*, const char*);
extern "C" int __stdcall FreeLibrary(void*);
#else
#include <dlfcn.h>
#endif

class sharedlib_t
{
public:
  sharedlib_t(const char* filename);
  ~sharedlib_t();
  bool isopen() const { return handle != 0; }
  void* funcptr(const char* name) const;
private:
  void* handle;
};

inline sharedlib_t::sharedlib_t(const char* libname)
{
#if defined(_WIN32)
  handle = LoadLibraryA((libname + std::string(".dll")).c_str());
#elif defined(__APPLE__)
  handle = dlopen((std::string("./") + libname + ".dylib").c_str(), RTLD_LAZY);
#elif defined(__linux__)
  handle = dlopen((std::string("./") + libname + ".so").c_str(), RTLD_LAZY);
#endif
}

inline sharedlib_t::~sharedlib_t()
{
  if ( handle )
  {
#if defined(_WIN32)
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
  }
}

inline void* sharedlib_t::funcptr(const char* funcname) const
{
  if ( handle )
  {
#if defined(_WIN32)
    return GetProcAddress(handle, funcname);
#else
    std::string str = funcname;
    size_t atpos = str.find('@');
    if ( atpos != std::string::npos ) str = str.substr(0, atpos);
    if ( str[0] == '_' ) str = str.substr(1, std::string::npos);
    return dlsym(handle, str.c_str());
#endif
  }
  else
  {
    return 0;
  }
}
