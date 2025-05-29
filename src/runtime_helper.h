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

  bool LoadScript(const char *filename, irr::core::stringc &output) const
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

  irr::core::stringc ExtractDir(const irr::core::stringc &filename) const
  {
    const char *fendp = strrchr(filename.c_str(), '/');
    const char *bendp = strrchr(filename.c_str(), '\\');
    const char *endp = (fendp >= bendp) ? fendp : bendp;
    if (endp)
      return filename.subString(0, endp - filename.c_str());
    return "";
  }

private:
  irr::IrrlichtDevice *m_device;
};
