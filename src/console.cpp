#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "console.h"

extern "C"
{

  EXPORT void CALL OpenConsole()
  {
#ifdef _WIN32
    AllocConsole();
    freopen("CONIN$", "w", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
  }

  EXPORT void CALL CloseConsole()
  {
#ifdef _WIN32
    FreeConsole();
#endif
  }

} // extern "C"
