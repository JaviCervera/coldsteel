#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void Memblock;

  EXPORT Memblock *CALL CreateMemblock(int size);
  EXPORT Memblock *CALL LoadMemblock(const char *filename);
  EXPORT void CALL SaveMemblock(Memblock *memblock, const char *filename);
  EXPORT void CALL FreeMemblock(Memblock *memblock);
  EXPORT int CALL MemblockSize(const Memblock *memblock);
  EXPORT void CALL PokeByte(Memblock *memblock, int offset, int val);
  EXPORT void CALL PokeShort(Memblock *memblock, int offset, int val);
  EXPORT void CALL PokeInt(Memblock *memblock, int offset, int val);
  EXPORT void CALL PokeFloat(Memblock *memblock, int offset, float val);
  EXPORT void CALL PokeString(Memblock *memblock, int offset, const char *val);
  EXPORT int CALL PeekByte(const Memblock *memblock, int offset);
  EXPORT int CALL PeekShort(const Memblock *memblock, int offset);
  EXPORT int CALL PeekInt(const Memblock *memblock, int offset);
  EXPORT float CALL PeekFloat(const Memblock *memblock, int offset);
  EXPORT const char *CALL PeekString(const Memblock *memblock, int offset);

#ifdef __cplusplus
} /* extern "C" */
#endif
