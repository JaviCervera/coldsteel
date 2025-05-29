#pragma once

#include <irrlicht.h>
#include "../include/file_system.h"
#include "platform_irrlicht.h"

struct FileSystem_Irrlicht : public FileSystem
{
  FileSystem_Irrlicht(Platform_Irrlicht &platform) : m_platform(&platform) {}

  bool AddZip(const char *filename)
  {
    if (GetFileSystem() && GetFileSystem()->existFile(filename))
    {
      return GetFileSystem()->addFileArchive(filename, true, false, irr::io::EFAT_ZIP);
    }
    return false;
  }

  const char *DirContents(const char *dir) const
  {
    static irr::core::stringc retstr;
    retstr = "";
    if (!GetFileSystem())
      return retstr.c_str();
    irr::io::path current = GetFileSystem()->getWorkingDirectory();
    GetFileSystem()->changeWorkingDirectoryTo(dir);
    irr::io::IFileList *list = GetFileSystem()->createFileList();
    for (irr::u32 i = 0; i < list->getFileCount(); ++i)
    {
      if (retstr != "")
        retstr += "\n";
      retstr += list->getFileName(i);
    }
    list->drop();
    GetFileSystem()->changeWorkingDirectoryTo(current);
    return retstr.c_str();
  }

  void ChangeDir(const char *dir)
  {
    if (GetFileSystem())
      GetFileSystem()->changeWorkingDirectoryTo(dir);
  }

  const char *CurrentDir() const
  {
    if (!GetFileSystem())
      return "";
    return GetFileSystem()->getWorkingDirectory().c_str();
  }

  size_t FileSize(const char *filename) const
  {
    if (!GetFileSystem() || !GetFileSystem()->existFile(filename))
      return size_t(-1);
    irr::io::IReadFile *file = GetFileSystem()->createAndOpenFile(filename);
    const size_t size = size_t(file->getSize());
    file->drop();
    return size;
  }

  size_t ReadFile(const char *filename, void *buffer, size_t max_size) const
  {
    if (!GetFileSystem() || !GetFileSystem()->existFile(filename))
      return 0;
    irr::io::IReadFile *file = GetFileSystem()->createAndOpenFile(filename);
    const size_t read_size = file->read(buffer, max_size);
    file->drop();
    return read_size;
  }

  bool WriteFile(const char *filename, void *buffer, size_t size, bool append = false)
  {
    if (!GetFileSystem())
      return false;
    irr::io::IWriteFile *file = GetFileSystem()->createAndWriteFile(filename, append);
    if (!file)
      return false;
    file->write(buffer, size);
    file->drop();
    return true;
  }

private:
  Platform_Irrlicht *m_platform;

  irr::io::IFileSystem *GetFileSystem() const
  {
    return m_platform->Device() ? m_platform->Device()->getFileSystem() : NULL;
  }
};
