#include <ctype.h>
#include <stdlib.h>
#include "core.h"
#include "string.h"

extern "C"
{

  EXPORT int CALL Val(const char *str)
  {
    return atoi(str);
  }

  EXPORT float CALL ValF(const char *str)
  {
    return atof(str);
  }

  EXPORT const char *CALL Str(int number)
  {
    static char str[32];
    sprintf(str, "%i", number);
    return str;
  }

  EXPORT const char *CALL StrF(float number)
  {
    static char str[32];
    sprintf(str, "%f", number);
    return str;
  }

  EXPORT const char *CALL Left(const char *str, int num)
  {
    static stringc retstr;
    retstr = stringc(str).subString(0, num);
    return retstr.c_str();
  }

  EXPORT const char *CALL Right(const char *str, int num)
  {
    static stringc retstr;
    retstr = str;
    retstr = retstr.subString(retstr.size() - num, num);
    return retstr.c_str();
  }

  EXPORT const char *CALL Mid(const char *str, int pos, int num)
  {
    static stringc retstr;
    retstr = stringc(str).subString(pos - 1, num);
    return retstr.c_str();
  }

  EXPORT const char *CALL Replace(const char *str, const char *find, const char *replace)
  {
    static stringc retstr;
    retstr = stringc(str).replace(find, replace);
    return retstr.c_str();
  }

  EXPORT int CALL Find(const char *str, const char *find, int offset)
  {
    return stringc(str).find(find, offset);
  }

  EXPORT const char *CALL Upper(const char *str)
  {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i)
      retstr[i] = toupper(retstr[i]);
    return retstr.c_str();
  }

  EXPORT const char *CALL Lower(const char *str)
  {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i)
      retstr[i] = tolower(retstr[i]);
    return retstr.c_str();
  }

  EXPORT const char *CALL Trim(const char *str)
  {
    static stringc retstr;
    retstr = stringc(str).trim();
    return retstr.c_str();
  }

  EXPORT const char *CALL Char(int code)
  {
    static stringc retstr;
    retstr = " ";
    retstr[0] = (char)code;
    return retstr.c_str();
  }

  EXPORT int CALL Code(const char *str)
  {
    return (int)str[0];
  }

  EXPORT int CALL Len(const char *str)
  {
    return (int)strlen(str);
  }

  static array<int> _DelimiterIndices(const stringc &str, const char *delimiter)
  {
    array<int> indices;
    int pos = 0;
    const int size = strlen(delimiter);
    while (pos != -1)
    {
      pos = str.find(delimiter, pos);
      if (pos != -1)
      {
        indices.push_back(pos);
        pos += size;
      }
    }
    return indices;
  }

  EXPORT int CALL SplitCount(const char *str, const char *delimiter)
  {
    return _DelimiterIndices(str, delimiter).size() + 1;
  }

  EXPORT const char *CALL SplitIndex(const char *str, const char *delimiter, int index)
  {
    static stringc retstr;
    const stringc s = str;
    const array<int> indices = _DelimiterIndices(s, delimiter);
    if (index == 1 && indices.size() == 0)
    {
      retstr = s;
    }
    else if (index == 1)
    {
      retstr = s.subString(0, indices[0]);
    }
    else if (index == indices.size() + 1)
    {
      const int size = strlen(delimiter);
      retstr = s.subString(indices[index - 2] + size, s.size() - indices[index - 2] - size);
    }
    else if (index > indices.size() + 1)
    {
      retstr = "";
    }
    else
    {
      const int size = strlen(delimiter);
      retstr = s.subString(indices[index - 2] + size, indices[index - 1] - indices[index - 2] - size);
    }
    return retstr.c_str();
  }

  EXPORT const char *CALL StripExt(const char *filename)
  {
    static stringc retstr;
    const char *endp = strrchr(filename, '.');
    if (!endp)
    {
      retstr = filename;
    }
    else
    {
      retstr = Mid(filename, 1, endp - filename);
    }
    return retstr.c_str();
  }

  EXPORT const char *CALL StripDir(const char *filename)
  {
    static stringc retstr;
    const char *fendp = strrchr(filename, '/');
    const char *bendp = strrchr(filename, '\\');
    const char *endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp)
    {
      retstr = filename;
    }
    else
    {
      const size_t offset = endp - filename + 1;
      retstr = Mid(filename, offset + 1, strlen(filename) - offset);
    }
    return retstr.c_str();
  }

  EXPORT const char *CALL ExtractExt(const char *filename)
  {
    static stringc retstr;
    const char *endp = strrchr(filename, '.');
    if (!endp)
    {
      retstr = "";
    }
    else
    {
      const size_t offset = endp - filename + 1;
      retstr = Mid(filename, offset + 1, strlen(filename) - offset);
    }
    return retstr.c_str();
  }

  EXPORT const char *CALL ExtractDir(const char *filename)
  {
    static stringc retstr;
    const char *fendp = strrchr(filename, '/');
    const char *bendp = strrchr(filename, '\\');
    const char *endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp)
    {
      retstr = "";
    }
    else
    {
      const size_t size = endp - filename;
      retstr = Mid(filename, 1, size);
    }
    return retstr.c_str();
  }

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
