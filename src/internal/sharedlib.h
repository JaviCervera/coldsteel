#pragma once

#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef HMODULE Handle;
#else
#include <dlfcn.h>
typedef void *Handle;
#endif

class sharedlib_t
{
public:
  sharedlib_t(const char *filename);
  ~sharedlib_t();
  bool isopen() const { return handle != 0; }
  void *funcptr(const char *name) const;

private:
  Handle handle;
};

inline sharedlib_t::sharedlib_t(const char *libname)
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
  if (handle)
  {
#if defined(_WIN32)
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
  }
}

inline void *sharedlib_t::funcptr(const char *funcname) const
{
  if (handle)
  {
#if defined(_WIN32)
    return (void *)GetProcAddress(handle, funcname);
#else
    std::string str = funcname;
    size_t atpos = str.find('@');
    if (atpos != std::string::npos)
      str = str.substr(0, atpos);
    if (str[0] == '_')
      str = str.substr(1, std::string::npos);
    return dlsym(handle, str.c_str());
#endif
  }
  else
  {
    return 0;
  }
}
