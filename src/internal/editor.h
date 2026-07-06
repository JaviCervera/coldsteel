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
    if (m_lib.isopen())
    {
      m_load_func = (LoadFunc)m_lib.funcptr("editor_load");
      m_run_func = (RunFunc)m_lib.funcptr("editor_run");
    }
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
      printf("Editor was built for SDK version %d but ColdSteel expects %d.\n",
             version, COLDSTEEL_SDK_VERSION);
      return false;
    }
    return true;
  }

  void Run() const
  {
    m_run_func();
  }

private:
  typedef int (*LoadFunc)(ColdSteelSDK *, const char *);
  typedef void (*RunFunc)();
  sharedlib_t m_lib;
  LoadFunc m_load_func;
  RunFunc m_run_func;
};
