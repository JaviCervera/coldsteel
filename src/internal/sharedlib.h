#pragma once

#include <irrString.h>

#ifdef _WIN32
extern "C" {
void* __stdcall LoadLibraryA(const char*);
void* __stdcall GetProcAddress(void*, const char*);
int   __stdcall FreeLibrary(void*);
}
#else
#include <dlfcn.h>
#endif

typedef void *Handle;

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
  handle = LoadLibraryA((irr::core::stringc(libname) + ".dll").c_str());
#elif defined(__APPLE__)
  handle = dlopen((irr::core::stringc(libname) + ".dylib").c_str(), RTLD_LAZY);
#elif defined(__linux__)
  handle = dlopen((irr::core::stringc(libname) + ".so").c_str(), RTLD_LAZY);
#elif defined(__EMSCRIPTEN__)
  handle = dlopen((irr::core::stringc(libname) + ".wasm").c_str(), RTLD_LAZY);
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
    irr::core::stringc str = funcname;
    irr::s32 atpos = str.find("@");
    if (atpos != -1)
      str = str.subString(0, atpos);
    if (str[0] == '_')
      str = str.subString(1, str.size() - 1);
    return dlsym(handle, str.c_str());
#endif
  }
  else
  {
    return 0;
  }
}
