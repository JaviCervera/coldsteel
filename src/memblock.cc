#include <string.h>
#include "core.h"
#include "memblock.h"


EXPORT Memblock* CALL csCreateMemblock(int size) {
    char* memblock = (char*)calloc(1, size + 4);
    ((int*)memblock)[0] = (int)size;
    return (Memblock*)((int*)memblock+1);
}


EXPORT Memblock* CALL csLoadMemblock(const char* filename) {
    Memblock* memblock = NULL;
    IReadFile* file = _csDevice()->getFileSystem()->createAndOpenFile(filename);
    if (file) {
        memblock = csCreateMemblock(file->getSize());
        file->read(memblock, file->getSize());
        file->drop();
    }
    return memblock;
}


EXPORT void CALL csSaveMemblock(Memblock* memblock, const char* filename) {
    IWriteFile* file = _csDevice()->getFileSystem()->createAndWriteFile(filename);
    if (file) {
        file->write(memblock, csMemblockSize(memblock));
        file->drop();
    }
}


EXPORT void CALL csFreeMemblock(Memblock* memblock) {
    free((int*)memblock-1);
}


EXPORT int CALL csMemblockSize(const Memblock* memblock) {
    return ((const int*)memblock-1)[0];
}


EXPORT void CALL csSetMemblockByte(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, (((char*)&val)+3), sizeof(unsigned char));
}


EXPORT void CALL csSetMemblockShort(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, (((char*)&val)+2), sizeof(unsigned short));
}


EXPORT void CALL csSetMemblockInt(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, &val, sizeof(val));
}


EXPORT void CALL csSetMemblockFloat(Memblock* memblock, int offset, float val) {
    memcpy((char*)memblock+offset, &val, sizeof(val));
}


EXPORT void CALL csSetMemblockString(Memblock* memblock, int offset, const char* val) {
    const int len = (int)strlen(val);
    csSetMemblockInt(memblock, offset, len);
    for (int i = 0; i < len; ++i) {
        csSetMemblockByte(memblock, offset + 4 + i, val[i]);
    }
}


EXPORT int CALL csMemblockByte(const Memblock* memblock, int offset) {
    unsigned char val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT int CALL csMemblockShort(const Memblock* memblock, int offset) {
    unsigned short val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT int CALL csMemblockInt(const Memblock* memblock, int offset) {
    int val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT float CALL csMemblockFloat(const Memblock* memblock, int offset) {
    float val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT const char* CALL csMemblockString(const Memblock* memblock, int offset) {
    static stringc result;
    const int len = csMemblockInt(memblock, offset);
    result = "";
    for (int i = 0; i < len; ++i) {
        char c;
        int i_ = csMemblockInt(memblock, offset + 4 + i);
        memcpy(&c, ((char*)&i_) + 3, sizeof(c));
        result += c;
    }
    return result.c_str();
}
