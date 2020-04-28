#include "core.h"
#include "dir.h"

extern "C" {


EXPORT bool_t CALL asAddZip(const char* filename) {
    return _asDevice()->getFileSystem()->addFileArchive(filename, true, false, EFAT_ZIP);
}


EXPORT const char* CALL asDirContents(const char* dir) {
    static stringc retstr;
    path current = _asDevice()->getFileSystem()->getWorkingDirectory();
    _asDevice()->getFileSystem()->changeWorkingDirectoryTo(dir);
    IFileList* list = _asDevice()->getFileSystem()->createFileList();
    retstr = "";
    for (u32 i = 0; i < list->getFileCount(); ++i) {
        if (retstr != "") retstr += "\n";
        retstr += list->getFileName(i);
    }
    list->drop();
    _asDevice()->getFileSystem()->changeWorkingDirectoryTo(current);
    return retstr.c_str();
}


EXPORT void CALL asChangeDir(const char* dir) {
    _asDevice()->getFileSystem()->changeWorkingDirectoryTo(dir);
}


EXPORT const char* CALL asCurrentDir() {
    return _asDevice()->getFileSystem()->getWorkingDirectory().c_str();
}


}
