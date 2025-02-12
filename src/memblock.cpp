#include <string.h>
#include "core.h"
#include "memblock.h"

Memblock *CreateMemblock(int size)
{
  char *memblock = (char *)calloc(1, size + 4);
  ((int *)memblock)[0] = (int)size;
  return (Memblock *)((int *)memblock + 1);
}

Memblock *LoadMemblock(const char *filename)
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

void SaveMemblock(Memblock *memblock, const char *filename)
{
  IWriteFile *file = _Device()->getFileSystem()->createAndWriteFile(filename);
  if (file)
  {
    file->write(memblock, MemblockSize(memblock));
    file->drop();
  }
}

void FreeMemblock(Memblock *memblock)
{
  free((int *)memblock - 1);
}

int MemblockSize(const Memblock *memblock)
{
  return ((const int *)memblock - 1)[0];
}

void SetMemblockByte(Memblock *memblock, int offset, int val)
{
  unsigned char b = (unsigned char)val;
  memcpy((char *)memblock + offset, &b, sizeof(unsigned char));
}

void SetMemblockShort(Memblock *memblock, int offset, int val)
{
  unsigned short s = (unsigned short)val;
  memcpy((char *)memblock + offset, &s, sizeof(unsigned short));
}

void SetMemblockInt(Memblock *memblock, int offset, int val)
{
  memcpy((char *)memblock + offset, &val, sizeof(val));
}

void SetMemblockFloat(Memblock *memblock, int offset, float val)
{
  memcpy((char *)memblock + offset, &val, sizeof(val));
}

void SetMemblockString(Memblock *memblock, int offset, const char *val)
{
  const int len = (int)strlen(val);
  SetMemblockInt(memblock, offset, len);
  for (int i = 0; i < len; ++i)
  {
    SetMemblockByte(memblock, offset + 4 + i, val[i]);
  }
}

int MemblockByte(const Memblock *memblock, int offset)
{
  unsigned char val;
  memcpy(&val, (const char *)memblock + offset, sizeof(val));
  return val;
}

int MemblockShort(const Memblock *memblock, int offset)
{
  unsigned short val;
  memcpy(&val, (const char *)memblock + offset, sizeof(val));
  return val;
}

int MemblockInt(const Memblock *memblock, int offset)
{
  int val;
  memcpy(&val, (const char *)memblock + offset, sizeof(val));
  return val;
}

float MemblockFloat(const Memblock *memblock, int offset)
{
  float val;
  memcpy(&val, (const char *)memblock + offset, sizeof(val));
  return val;
}

const char *MemblockString(const Memblock *memblock, int offset)
{
  static stringc result;
  result = "";
  const int len = MemblockInt(memblock, offset);
  for (int i = 0; i < len; ++i)
  {
    result += (char)MemblockByte(memblock, offset + 4 + i);
  }
  return result.c_str();
}
