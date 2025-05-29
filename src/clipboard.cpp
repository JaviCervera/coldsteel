#include "clipboard.h"
#include "core.h"

extern "C"
{

  EXPORT void CALL CopyToClipboard(const char *str)
  {
    _Device()->getOSOperator()->copyToClipboard(str);
  }

  EXPORT const char *CALL PasteFromClipboard()
  {
    static stringc retstr;
    retstr = _Device()->getOSOperator()->getTextFromClipboard();
    return retstr.c_str();
  }

} // extern "C"
