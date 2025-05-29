#pragma once

#include <irrlicht.h>

struct RuntimeHelper
{
  RuntimeHelper()
  {
    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_NULL;
    params.LoggingLevel = irr::ELL_ERROR;
    m_device = createDeviceEx(params);
  }

  ~RuntimeHelper()
  {
    m_device->drop();
  }

  bool LoadString(const irr::core::stringc &filename, irr::core::stringc &output) const
  {
    bool result = false;
    irr::io::IFileSystem *fs = m_device->getFileSystem();
    if (fs->existFile(filename) && fs->addFileArchive(filename, true, false, irr::io::EFAT_ZIP))
    {
      irr::io::IReadFile *file = fs->createAndOpenFile(filename);
      if (file)
      {
        char *chars = new char[file->getSize() + 1];
        chars[file->getSize()] = '\0';
        file->read(chars, file->getSize());
        file->drop();
        output = chars;
        delete[] chars;
        result = true;
      }
    }
    return result;
  }

  void SaveString(const irr::core::stringc &str, const irr::core::stringc &filename)
  {
    IWriteFile *file = m_device->getFileSystem()->createAndWriteFile(filename, false);
    file->write(str.c_str(), str.size());
    file->drop();
  }

  irr::core::stringc CurrentDir()
  {
    return m_device->getFileSystem()->getWorkingDirectory();
  }

private:
  irr::IrrlichtDevice *m_device;
};
