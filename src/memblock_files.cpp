#include "core.h"
#include "memblock_files.h"

extern "C"
{

  EXPORT Memblock *CALL LoadMemblock(const char *filename)
  {
    Memblock *memblock = NULL;
    IReadFile *file = _Device()->getFileSystem()->createAndOpenFile(filename);
    if (file)
    {
      memblock = CreateMemblock(file->getSize());
      file->read(memblock, file->getSize());
      file->drop();
    }
    return memblock;
  }

  EXPORT void CALL SaveMemblock(Memblock *memblock, const char *filename)
  {
    IWriteFile *file = _Device()->getFileSystem()->createAndWriteFile(filename);
    if (file)
    {
      file->write(memblock, MemblockSize(memblock));
      file->drop();
    }
  }

} // extern "C"
