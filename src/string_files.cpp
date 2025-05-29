#include "core.h"
#include "string_files.h"

extern "C"
{

  EXPORT const char *CALL LoadString(const char *filename)
  {
    static stringc retstr;
    IReadFile *file = _Device()->getFileSystem()->createAndOpenFile(filename);
    if (file)
    {
      long size = file->getSize();
      char *str = (char *)malloc(size + 1);
      str[size] = 0;
      file->read(str, size);
      retstr = str;
      free(str);
      file->drop();
    }
    else
    {
      retstr = "";
    }
    return retstr.c_str();
  }

  EXPORT void CALL SaveString(const char *str, const char *filename, bool_t append)
  {
    IWriteFile *file = _Device()->getFileSystem()->createAndWriteFile(filename, append);
    file->write(str, strlen(str));
    file->drop();
  }

} // extern "C"
