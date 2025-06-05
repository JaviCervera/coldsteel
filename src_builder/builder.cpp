#include "builder.h"

#ifdef _WIN32
#if defined(DLLEXPORT)
#define EXPORT __declspec(dllexport)
#elif defined(DLLIMPORT)
#define EXPORT __declspec(dllimport)
#else
#define EXPORT
#endif
#define CALL __stdcall
#else
#define EXPORT
#define CALL
#endif

extern "C"
{

  EXPORT void CALL Build(const char *dir, bool precompile, Builder::PrintCallback print)
  {
    Builder(print).Build(dir, precompile);
  }

  EXPORT void CALL BuildWeb(const char *dir, bool precompile, Builder::PrintCallback print)
  {
    Builder(print).BuildWeb(dir, precompile);
  }

} // extern "C"
