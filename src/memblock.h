#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void Memblock;


EXPORT Memblock* CALL asCreateMemblock(int size);
EXPORT Memblock* CALL asLoadMemblock(const char* filename);
EXPORT void CALL asSaveMemblock(Memblock* memblock, const char* filename);
EXPORT void CALL asFreeMemblock(Memblock* memblock);
EXPORT int CALL asMemblockSize(const Memblock* memblock);
EXPORT void CALL asSetMemblockByte(Memblock* memblock, int offset, int val);
EXPORT void CALL asSetMemblockShort(Memblock* memblock, int offset, int val);
EXPORT void CALL asSetMemblockInt(Memblock* memblock, int offset, int val);
EXPORT void CALL asSetMemblockFloat(Memblock* memblock, int offset, float val);
EXPORT void CALL asSetMemblockString(Memblock* memblock, int offset, const char* val);
EXPORT int CALL asMemblockByte(const Memblock* memblock, int offset);
EXPORT int CALL asMemblockShort(const Memblock* memblock, int offset);
EXPORT int CALL asMemblockInt(const Memblock* memblock, int offset);
EXPORT float CALL asMemblockFloat(const Memblock* memblock, int offset);
EXPORT const char* CALL asMemblockString(const Memblock* memblock, int offset);


#ifdef __cplusplus
} /* extern "C" */
#endif
