#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void Memblock;


EXPORT Memblock* CALL CreateMemblock(int size);
EXPORT Memblock* CALL LoadMemblock(const char* filename);
EXPORT void CALL SaveMemblock(Memblock* memblock, const char* filename);
EXPORT void CALL FreeMemblock(Memblock* memblock);
EXPORT int CALL MemblockSize(const Memblock* memblock);
EXPORT void CALL SetMemblockByte(Memblock* memblock, int offset, int val);
EXPORT void CALL SetMemblockShort(Memblock* memblock, int offset, int val);
EXPORT void CALL SetMemblockInt(Memblock* memblock, int offset, int val);
EXPORT void CALL SetMemblockFloat(Memblock* memblock, int offset, float val);
EXPORT void CALL SetMemblockString(Memblock* memblock, int offset, const char* val);
EXPORT int CALL MemblockByte(const Memblock* memblock, int offset);
EXPORT int CALL MemblockShort(const Memblock* memblock, int offset);
EXPORT int CALL MemblockInt(const Memblock* memblock, int offset);
EXPORT float CALL MemblockFloat(const Memblock* memblock, int offset);
EXPORT const char* CALL MemblockString(const Memblock* memblock, int offset);


#ifdef __cplusplus
} /* extern "C" */
#endif
