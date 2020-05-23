#include <string.h>
#include "core.h"
#include "memblock.h"


EXPORT Memblock* CALL asCreateMemblock(int size) {
    char* memblock = (char*)calloc(1, size + 4);
    ((int*)memblock)[0] = (int)size;
    return (Memblock*)((int*)memblock+1);
}


EXPORT Memblock* CALL asLoadMemblock(const char* filename) {
    Memblock* memblock = NULL;
    IReadFile* file = _asDevice()->getFileSystem()->createAndOpenFile(filename);
    if (file) {
        memblock = asCreateMemblock(file->getSize());
        file->read(memblock, file->getSize());
        file->drop();
    }
    return memblock;
}


EXPORT void CALL asSaveMemblock(Memblock* memblock, const char* filename) {
    IWriteFile* file = _asDevice()->getFileSystem()->createAndWriteFile(filename);
    if (file) {
        file->write(memblock, asMemblockSize(memblock));
        file->drop();
    }
}


EXPORT void CALL asFreeMemblock(Memblock* memblock) {
    free((int*)memblock-1);
}


EXPORT int CALL asMemblockSize(const Memblock* memblock) {
    return ((const int*)memblock-1)[0];
}


EXPORT void CALL asSetMemblockByte(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, (((char*)&val)+3), sizeof(unsigned char));
}


EXPORT void CALL asSetMemblockShort(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, (((char*)&val)+2), sizeof(unsigned short));
}


EXPORT void CALL asSetMemblockInt(Memblock* memblock, int offset, int val) {
    memcpy((char*)memblock+offset, &val, sizeof(val));
}


EXPORT void CALL asSetMemblockFloat(Memblock* memblock, int offset, float val) {
    memcpy((char*)memblock+offset, &val, sizeof(val));
}


EXPORT void CALL asSetMemblockString(Memblock* memblock, int offset, const char* val) {
    const int len = (int)strlen(val);
    asSetMemblockInt(memblock, offset, len);
    for (int i = 0; i < len; ++i) {
        asSetMemblockByte(memblock, offset + 4 + i, val[i]);
    }
}


EXPORT int CALL asMemblockByte(const Memblock* memblock, int offset) {
    unsigned char val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT int CALL asMemblockShort(const Memblock* memblock, int offset) {
    unsigned short val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT int CALL asMemblockInt(const Memblock* memblock, int offset) {
    int val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT float CALL asMemblockFloat(const Memblock* memblock, int offset) {
    float val;
    memcpy(&val, (const char*)memblock+offset, sizeof(val));
    return val;
}


EXPORT const char* CALL asMemblockString(const Memblock* memblock, int offset) {
    static stringc result;
    const int len = asMemblockInt(memblock, offset);
    result = "";
    for (int i = 0; i < len; ++i) {
        char c;
        int i_ = asMemblockInt(memblock, offset + 4 + i);
        memcpy(&c, ((char*)&i_) + 3, sizeof(c));
        result += c;
    }
    return result.c_str();
}
