#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void Memblock;


EXPORT Memblock* CALL csCreateMemblock(int size);
EXPORT Memblock* CALL csLoadMemblock(const char* filename);
EXPORT void CALL csSaveMemblock(Memblock* memblock, const char* filename);
EXPORT void CALL csFreeMemblock(Memblock* memblock);
EXPORT int CALL csMemblockSize(const Memblock* memblock);
EXPORT void CALL csSetMemblockByte(Memblock* memblock, int offset, int val);
EXPORT void CALL csSetMemblockShort(Memblock* memblock, int offset, int val);
EXPORT void CALL csSetMemblockInt(Memblock* memblock, int offset, int val);
EXPORT void CALL csSetMemblockFloat(Memblock* memblock, int offset, float val);
EXPORT void CALL csSetMemblockString(Memblock* memblock, int offset, const char* val);
EXPORT int CALL csMemblockByte(const Memblock* memblock, int offset);
EXPORT int CALL csMemblockShort(const Memblock* memblock, int offset);
EXPORT int CALL csMemblockInt(const Memblock* memblock, int offset);
EXPORT float CALL csMemblockFloat(const Memblock* memblock, int offset);
EXPORT const char* CALL csMemblockString(const Memblock* memblock, int offset);


#ifdef __cplusplus
} /* extern "C" */
#endif
