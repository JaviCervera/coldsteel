#include "core.h"
#include "dir.h"

extern "C"
{

  EXPORT bool_t CALL AddZip(const char *filename)
  {
    if (_Device()->getFileSystem()->existFile(filename))
    {
      return _Device()->getFileSystem()->addFileArchive(filename, true, false, EFAT_ZIP);
    }
    else
    {
      return false;
    }
  }

  EXPORT const char *CALL DirContents(const char *dir)
  {
    static stringc retstr;
    path current = _Device()->getFileSystem()->getWorkingDirectory();
    _Device()->getFileSystem()->changeWorkingDirectoryTo(dir);
    IFileList *list = _Device()->getFileSystem()->createFileList();
    retstr = "";
    for (u32 i = 0; i < list->getFileCount(); ++i)
    {
      if (retstr != "")
        retstr += "\n";
      retstr += list->getFileName(i);
    }
    list->drop();
    _Device()->getFileSystem()->changeWorkingDirectoryTo(current);
    return retstr.c_str();
  }

  EXPORT void CALL ChangeDir(const char *dir)
  {
    _Device()->getFileSystem()->changeWorkingDirectoryTo(dir);
  }

  EXPORT const char *CALL CurrentDir()
  {
    return _Device()->getFileSystem()->getWorkingDirectory().c_str();
  }
}
