#include "../lib/tinyfiledialogs/tinyfiledialogs.h"
#include "dialogs.h"
#include "string.h"

extern "C"
{

  EXPORT bool_t CALL Confirm(const char *title, const char *text, bool_t serious)
  {
    return tinyfd_messageBox(title, text, "yesno", serious ? "error" : "question", 1) == 1;
  }

  EXPORT void CALL Notify(const char *title, const char *text, bool_t serious)
  {
    tinyfd_messageBox(title, text, "ok", serious ? "error" : "info", 0);
  }

  EXPORT int CALL Proceed(const char *title, const char *text, bool_t serious)
  {
    return tinyfd_messageBox(title, text, "yesnocancel", serious ? "error" : "question", 0);
  }

  EXPORT int CALL RequestColor(const char *title, int color)
  {
    unsigned char c[3];
    c[0] = (color >> 16) & 0xFF;
    c[1] = (color >> 8) & 0xFF;
    c[2] = color & 0xFF;

    if (tinyfd_colorChooser(title, NULL, c, c))
      color = (c[0] << 16) | (c[1] << 8) | c[2];

    return color;
  }

  EXPORT const char *CALL RequestDir(const char *title, const char *dir)
  {
    const char *selDir = tinyfd_selectFolderDialog(title, dir);
    if (selDir != NULL)
      return selDir;
    else
      return "";
  }

  EXPORT const char *CALL RequestFile(const char *title, const char *filters, bool_t save, const char *file)
  {
    const char *fname = NULL;
    const char **pfilters = NULL;
    int splitCount = 0;
    if (filters && strcmp(filters, "") != 0)
    {
      splitCount = SplitCount(filters, ",");
      pfilters = (const char **)malloc(sizeof(char *) * splitCount);
      for (size_t i = 1; i <= splitCount; ++i)
        pfilters[i - 1] = SplitIndex(filters, ",", i);
    }

    if (!save)
      fname = tinyfd_openFileDialog(title, file, splitCount, pfilters, filters, 0);
    else
      fname = tinyfd_saveFileDialog(title, file, splitCount, pfilters, filters);

    if (pfilters)
      free(pfilters);

    if (fname != NULL)
      return fname;
    else
      return "";
  }

  EXPORT const char *CALL RequestInput(const char *title, const char *text, const char *def, bool_t password)
  {
    const char *input = tinyfd_inputBox(title, text, password ? (const char *)NULL : def);
    if (input != NULL)
      return input;
    else
      return "";
  }

} /* extern "C" */
