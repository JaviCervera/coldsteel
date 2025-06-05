#pragma once

#include "sharedlib.h"

struct Builder
{
  typedef void (*PrintCallback)(const char *);

  Builder(PrintCallback cb)
      : m_print(cb),
        m_lib("builder"),
        m_build_func(BuildFunc(m_lib.funcptr("Build@12"))),
        m_build_web_func(BuildFunc(m_lib.funcptr("BuildWeb@12")))
  {
  }

  bool IsReady() const
  {
    return m_lib.isopen() && m_build_func && m_build_web_func;
  }

  void Build(const char *dir, bool precompile) const
  {
    m_build_func(dir, precompile, m_print);
  }

  void BuildWeb(const char *dir, bool precompile) const
  {
    m_build_web_func(dir, precompile, m_print);
  }

private:
  typedef void (*BuildFunc)(const char *, bool, PrintCallback);

  PrintCallback m_print;
  sharedlib_t m_lib;
  BuildFunc m_build_func;
  BuildFunc m_build_web_func;
};
