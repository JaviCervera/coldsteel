#pragma once

#include "sharedlib.h"
#include "../../_build/sdk/sdk.h"

#include <cstdio>

struct Editor
{
  Editor(const char *bindir)
    : m_lib((irr::core::stringc(bindir) + "/editor").c_str()),
      m_load_func(0), m_run_func(0)
  {
    if (!m_lib.isopen())
    {
      m_error = "editor.dll could not be loaded from ";
      m_error += bindir;
      return;
    }
    m_load_func = (LoadFunc)m_lib.funcptr("editor_load@8");
    m_run_func = (RunFunc)m_lib.funcptr("editor_run@0");
    if (!m_load_func)
      m_error = "editor.dll does not export 'editor_load'";
    else if (!m_run_func)
      m_error = "editor.dll does not export 'editor_run'";
  }

  bool IsReady() const
  {
    return m_lib.isopen() && m_load_func && m_run_func;
  }

  bool Init(ColdSteelSDK *sdk, const char *bindir)
  {
    if (!m_load_func) return false;
    int version = m_load_func(sdk, bindir);
    if (version != COLDSTEEL_SDK_VERSION)
    {
      char buf[256];
      snprintf(buf, sizeof(buf), "Editor was built for SDK version %d but ColdSteel expects %d.",
               version, COLDSTEEL_SDK_VERSION);
      m_error = buf;
      return false;
    }
    return true;
  }

  const char *LastError() const
  {
    return m_error.c_str();
  }

  void Run() const
  {
    m_run_func();
  }

private:
#ifdef _WIN32
  typedef int (__stdcall *LoadFunc)(ColdSteelSDK *, const char *);
  typedef void (__stdcall *RunFunc)();
#else
  typedef int (*LoadFunc)(ColdSteelSDK *, const char *);
  typedef void (*RunFunc)();
#endif
  sharedlib_t m_lib;
  LoadFunc m_load_func;
  RunFunc m_run_func;
  irr::core::stringc m_error;
};
