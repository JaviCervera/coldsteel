#pragma once

#include "common.h"

typedef void Memblock;

Memblock *CreateMemblock(int size);
Memblock *LoadMemblock(const char *filename);
void SaveMemblock(Memblock *memblock, const char *filename);
void FreeMemblock(Memblock *memblock);
int MemblockSize(const Memblock *memblock);
void SetMemblockByte(Memblock *memblock, int offset, int val);
void SetMemblockShort(Memblock *memblock, int offset, int val);
void SetMemblockInt(Memblock *memblock, int offset, int val);
void SetMemblockFloat(Memblock *memblock, int offset, float val);
void SetMemblockString(Memblock *memblock, int offset, const char *val);
int MemblockByte(const Memblock *memblock, int offset);
int MemblockShort(const Memblock *memblock, int offset);
int MemblockInt(const Memblock *memblock, int offset);
float MemblockFloat(const Memblock *memblock, int offset);
const char *MemblockString(const Memblock *memblock, int offset);
