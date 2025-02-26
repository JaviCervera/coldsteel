#include "core.h"
#include "log.h"

extern "C"
{

  EXPORT void CALL LogInfo(const char *msg)
  {
    ELOG_LEVEL level = _Device()->getLogger()->getLogLevel();
    _Device()->getLogger()->setLogLevel(ELL_INFORMATION);
    _Device()->getLogger()->log(msg, ELL_INFORMATION);
    _Device()->getLogger()->setLogLevel(level);
  }

} /* extern "C" */
