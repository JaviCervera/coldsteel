/**
 * @file
 * Bitwise operations. Functions operate on integer bit patterns.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Returns the bitwise AND of two integers.
   *
   * @param a The first integer value.
   * @param b The second integer value.
   * @return The bitwise AND of a and b.
   */
  EXPORT int CALL BitAnd(int a, int b);

  /**
   * Returns the bitwise OR of two integers.
   *
   * @param a The first integer value.
   * @param b The second integer value.
   * @return The bitwise OR of a and b.
   */
  EXPORT int CALL BitOr(int a, int b);

  /**
   * Returns the bitwise XOR of two integers.
   *
   * @param a The first integer value.
   * @param b The second integer value.
   * @return The bitwise XOR of a and b.
   */
  EXPORT int CALL BitXor(int a, int b);

  /**
   * Returns the bitwise NOT of an integer.
   *
   * @param val The integer value to invert.
   * @return The bitwise NOT of val.
   */
  EXPORT int CALL BitNot(int val);

  /**
   * Returns an integer shifted left by a number of bits.
   *
   * @param val The integer value to shift.
   * @param count The number of bits to shift left.
   * @return val shifted left by count bits.
   */
  EXPORT int CALL Shl(int val, int count);

  /**
   * Returns an integer shifted right by a number of bits.
   *
   * @param val The integer value to shift.
   * @param count The number of bits to shift right.
   * @return val shifted right by count bits.
   */
  EXPORT int CALL Shr(int val, int count);

#ifdef __cplusplus
} /* extern "C" */
#endif
