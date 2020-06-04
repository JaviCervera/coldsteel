#pragma once

#include "common.h"

class FileBuffer {
public:
    FileBuffer(const char* filename);
    ~FileBuffer();

    const void* Buffer() const;
    const int Size() const;
private:
    void* buffer;
    int size;
};
