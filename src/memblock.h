/**
 * @file
 * A Memblock is a raw block of memory used for low-level data manipulation.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /** Raw memblock handle. */
  typedef void Memblock;

  /**
   * Creates a new memblock.
   *
   * @param size The size of the memblock in bytes.
   * @return A new memblock.
   */
  EXPORT Memblock *CALL CreateMemblock(int size);

  /**
   * Loads a file into a new memblock.
   *
   * @param filename The file to load.
   * @return A new memblock containing the file contents.
   */
  EXPORT Memblock *CALL LoadMemblock(const char *filename);

  /**
   * Saves the contents of a memblock to a file.
   *
   * @param memblock The memblock to save.
   * @param filename The destination filename.
   */
  EXPORT void CALL SaveMemblock(Memblock *memblock, const char *filename);

  /**
   * Frees a memblock.
   *
   * @param memblock The memblock to free.
   */
  EXPORT void CALL FreeMemblock(Memblock *memblock);

  /**
   * Returns the size of a memblock in bytes.
   *
   * @param memblock The memblock to query.
   * @return The memblock size in bytes.
   */
  EXPORT int CALL MemblockSize(const Memblock *memblock);

  /**
   * Writes a byte value at the given byte offset.
   *
   * @param memblock The memblock to modify.
   * @param offset The byte offset to write to.
   * @param val The byte value to write.
   */
  EXPORT void CALL PokeByte(Memblock *memblock, int offset, int val);

  /**
   * Writes a 16-bit integer at the given byte offset.
   *
   * @param memblock The memblock to modify.
   * @param offset The byte offset to write to.
   * @param val The 16-bit integer value to write.
   */
  EXPORT void CALL PokeShort(Memblock *memblock, int offset, int val);

  /**
   * Writes a 32-bit integer at the given byte offset.
   *
   * @param memblock The memblock to modify.
   * @param offset The byte offset to write to.
   * @param val The 32-bit integer value to write.
   */
  EXPORT void CALL PokeInt(Memblock *memblock, int offset, int val);

  /**
   * Writes a 32-bit floating-point value at the given byte offset.
   *
   * @param memblock The memblock to modify.
   * @param offset The byte offset to write to.
   * @param val The floating-point value to write.
   */
  EXPORT void CALL PokeFloat(Memblock *memblock, int offset, float val);

  /**
   * Writes a null-terminated string at the given byte offset.
   *
   * @param memblock The memblock to modify.
   * @param offset The byte offset to write to.
   * @param val The string to write.
   */
  EXPORT void CALL PokeString(Memblock *memblock, int offset, const char *val);

  /**
   * Reads a byte value at the given byte offset.
   *
   * @param memblock The memblock to read from.
   * @param offset The byte offset to read from.
   * @return The byte value at the offset.
   */
  EXPORT int CALL PeekByte(const Memblock *memblock, int offset);

  /**
   * Reads a 16-bit integer at the given byte offset.
   *
   * @param memblock The memblock to read from.
   * @param offset The byte offset to read from.
   * @return The 16-bit integer value at the offset.
   */
  EXPORT int CALL PeekShort(const Memblock *memblock, int offset);

  /**
   * Reads a 32-bit integer at the given byte offset.
   *
   * @param memblock The memblock to read from.
   * @param offset The byte offset to read from.
   * @return The 32-bit integer value at the offset.
   */
  EXPORT int CALL PeekInt(const Memblock *memblock, int offset);

  /**
   * Reads a 32-bit floating-point value at the given byte offset.
   *
   * @param memblock The memblock to read from.
   * @param offset The byte offset to read from.
   * @return The floating-point value at the offset.
   */
  EXPORT float CALL PeekFloat(const Memblock *memblock, int offset);

  /**
   * Reads a null-terminated string at the given byte offset.
   *
   * @param memblock The memblock to read from.
   * @param offset The byte offset to read from.
   * @return The string at the offset.
   */
  EXPORT const char *CALL PeekString(const Memblock *memblock, int offset);

#ifdef __cplusplus
} /* extern "C" */
#endif
