/**
 * @file
 * Mathematical utility functions. Angles are specified in degrees unless stated otherwise.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Returns the largest integer not greater than the value.
   *
   * @param val The value to round down.
   * @return The rounded value.
   */
  EXPORT float CALL Floor(float val);

  /**
   * Returns the smallest integer not less than the value.
   *
   * @param val The value to round up.
   * @return The rounded value.
   */
  EXPORT float CALL Ceil(float val);

  /**
   * Returns the absolute value.
   *
   * @param val The value to evaluate.
   * @return The absolute value.
   */
  EXPORT float CALL Abs(float val);

  /**
   * Returns the square root of the value.
   *
   * @param val The value to evaluate.
   * @return The square root of the value.
   */
  EXPORT float CALL Sqr(float val);

  /**
   * Returns the sine of the value.
   *
   * @param val The angle in degrees.
   * @return The sine of the angle.
   */
  EXPORT float CALL Sin(float val);

  /**
   * Returns the cosine of the value.
   *
   * @param val The angle in degrees.
   * @return The cosine of the angle.
   */
  EXPORT float CALL Cos(float val);

  /**
   * Returns the tangent of the value.
   *
   * @param val The angle in degrees.
   * @return The tangent of the angle.
   */
  EXPORT float CALL Tan(float val);

  /**
   * Returns the arcsine of the value in degrees.
   *
   * @param val The value whose arcsine will be computed.
   * @return The arcsine in degrees.
   */
  EXPORT float CALL ASin(float val);

  /**
   * Returns the arccosine of the value in degrees.
   *
   * @param val The value whose arccosine will be computed.
   * @return The arccosine in degrees.
   */
  EXPORT float CALL ACos(float val);

  /**
   * Returns the arctangent of the value in degrees.
   *
   * @param val The value whose arctangent will be computed.
   * @return The arctangent in degrees.
   */
  EXPORT float CALL ATan(float val);

  /**
   * Returns the arctangent of x divided by y in degrees.
   *
   * @param x The first coordinate used in the angle calculation.
   * @param y The second coordinate used in the angle calculation.
   * @return The arctangent in degrees, using the signs of both values to determine the quadrant.
   */
  EXPORT float CALL ATan2(float x, float y);

  /**
   * Returns e raised to the power of the value.
   *
   * @param val The exponent.
   * @return The exponential result.
   */
  EXPORT float CALL Exp(float val);

  /**
   * Returns the base raised to the given power.
   *
   * @param base The base value.
   * @param exp The exponent.
   * @return The result of base raised to exp.
   */
  EXPORT float CALL Pow(float base, float exp);

  /**
   * Returns the natural logarithm of the value.
   *
   * @param val The value whose logarithm will be computed.
   * @return The natural logarithm.
   */
  EXPORT float CALL Log(float val);

  /**
   * Returns the base-10 logarithm of the value.
   *
   * @param val The value whose logarithm will be computed.
   * @return The base-10 logarithm.
   */
  EXPORT float CALL Log10(float val);

  /**
   * Returns the smaller of two values.
   *
   * @param a The first value.
   * @param b The second value.
   * @return The smaller value.
   */
  EXPORT float CALL Min(float a, float b);

  /**
   * Returns the larger of two values.
   *
   * @param a The first value.
   * @param b The second value.
   * @return The larger value.
   */
  EXPORT float CALL Max(float a, float b);

  /**
   * Clamps the value to the given range.
   *
   * @param val The value to clamp.
   * @param min The minimum allowed value.
   * @param max The maximum allowed value.
   * @return The clamped value.
   */
  EXPORT float CALL Clamp(float val, float min, float max);

  /**
   * Converts a boolean value to an integer.
   *
   * @param val The boolean value to convert.
   * @return 1 if the value is true; otherwise 0.
   */
  EXPORT int CALL BoolToInt(bool_t val);

  /**
   * Converts a floating-point value to an integer by truncation.
   *
   * @param val The value to convert.
   * @return The truncated integer value.
   */
  EXPORT int CALL Int(float val);

  /**
   * Returns a random integer within the given range.
   *
   * @param min The minimum value in the range.
   * @param max The maximum value in the range.
   * @return A random integer in the range [min, max].
   */
  EXPORT int CALL Rand(int min, int max);

  /**
   * Seeds the random number generator.
   *
   * @param seed The seed value to use.
   */
  EXPORT void CALL RandSeed(int seed);

  /**
   * Converts radians to degrees.
   *
   * @param rad The angle in radians.
   * @return The angle in degrees.
   */
  EXPORT float CALL Deg(float rad);

  /**
   * Converts degrees to radians.
   *
   * @param deg The angle in degrees.
   * @return The angle in radians.
   */
  EXPORT float CALL Rad(float deg);

  /**
   * Wraps a value into the range [0, mod).
   *
   * @param val The value to wrap.
   * @param mod The modulus that defines the wrap range.
   * @return The wrapped value.
   */
  EXPORT float CALL Wrap(float val, float mod);

  /**
   * Returns the X component of the last collision or pick point.
   *
   * @return The X component of the last collision or pick point.
   */
  EXPORT float CALL PointX();

  /**
   * Returns the Y component of the last collision or pick point.
   *
   * @return The Y component of the last collision or pick point.
   */
  EXPORT float CALL PointY();

  /**
   * Returns the Z component of the last collision or pick point.
   *
   * @return The Z component of the last collision or pick point.
   */
  EXPORT float CALL PointZ();

  /**
   * Returns the X component of the last collision or pick surface normal.
   *
   * @return The X component of the last collision or pick surface normal.
   */
  EXPORT float CALL NormalX();

  /**
   * Returns the Y component of the last collision or pick surface normal.
   *
   * @return The Y component of the last collision or pick surface normal.
   */
  EXPORT float CALL NormalY();

  /**
   * Returns the Z component of the last collision or pick surface normal.
   *
   * @return The Z component of the last collision or pick surface normal.
   */
  EXPORT float CALL NormalZ();

#ifndef SWIG
  void _SetPoint(float x, float y, float z);
  void _SetNormal(float x, float y, float z);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
