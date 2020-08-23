#include "core.h"
#include "dir.h"

extern "C" {


EXPORT bool_t CALL csAddZip(const char* filename) {
    return _csDevice()->getFileSystem()->addFileArchive(filename, true, false, EFAT_ZIP);
}


EXPORT const char* CALL csDirContents(const char* dir) {
    static stringc retstr;
    path current = _csDevice()->getFileSystem()->getWorkingDirectory();
    _csDevice()->getFileSystem()->changeWorkingDirectoryTo(dir);
    IFileList* list = _csDevice()->getFileSystem()->createFileList();
    retstr = "";
    for (u32 i = 0; i < list->getFileCount(); ++i) {
        if (retstr != "") retstr += "\n";
        retstr += list->getFileName(i);
    }
    list->drop();
    _csDevice()->getFileSystem()->changeWorkingDirectoryTo(current);
    return retstr.c_str();
}


EXPORT void CALL csChangeDir(const char* dir) {
    _csDevice()->getFileSystem()->changeWorkingDirectoryTo(dir);
}


EXPORT const char* CALL csCurrentDir() {
    return _csDevice()->getFileSystem()->getWorkingDirectory().c_str();
}


}
