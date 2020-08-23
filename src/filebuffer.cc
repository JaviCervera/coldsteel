#include <stdlib.h>
#include "core.h"
#include "filebuffer.h"


FileBuffer::FileBuffer(const char* filename) : buffer(NULL), size(0) {
    IReadFile* file = _csDevice()->getFileSystem()->createAndOpenFile(filename);
    if (file) {
        size = file->getSize();
        buffer = malloc(size);
        file->read(buffer, size);
        file->drop();
    }
}


FileBuffer::~FileBuffer() {
    free(buffer);
}

const void* FileBuffer::Buffer() const {
    return buffer;
}


const int FileBuffer::Size() const {
    return size;
}
