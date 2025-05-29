/** @file */
#pragma once

#include "common.h"
#include "memblock.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT Memblock *CALL LoadMemblock(const char *filename);
  EXPORT void CALL SaveMemblock(Memblock *memblock, const char *filename);

#ifdef __cplusplus
} /* extern "C" */
#endif
