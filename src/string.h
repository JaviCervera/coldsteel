/**
 * @file
 * String manipulation and conversion functions. Functions that return strings return pointers to a
 * shared internal buffer; copy the result if you need to retain it.
 */
#pragma once

#include "common.h"
#ifdef LoadString
#undef LoadString
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Parses an integer from a string.
   *
   * @param str The string to parse.
   * @return The parsed integer value.
   */
  EXPORT int CALL Val(const char *str);

  /**
   * Parses a float from a string.
   *
   * @param str The string to parse.
   * @return The parsed floating-point value.
   */
  EXPORT float CALL ValF(const char *str);

  /**
   * Converts an integer to its string representation.
   *
   * @param number The integer to convert.
   * @return The integer converted to a string.
   */
  EXPORT const char *CALL Str(int number);

  /**
   * Converts a float to its string representation.
   *
   * @param number The floating-point value to convert.
   * @return The floating-point value converted to a string.
   */
  EXPORT const char *CALL StrF(float number);

  /**
   * Returns the leftmost characters of a string.
   *
   * @param str The source string.
   * @param num The number of characters to return.
   * @return The leftmost characters of the string.
   */
  EXPORT const char *CALL Left(const char *str, int num);

  /**
   * Returns the rightmost characters of a string.
   *
   * @param str The source string.
   * @param num The number of characters to return.
   * @return The rightmost characters of the string.
   */
  EXPORT const char *CALL Right(const char *str, int num);

  /**
   * Returns characters from a string starting at a 1-based position.
   *
   * @param str The source string.
   * @param pos The 1-based starting position.
   * @param num The number of characters to return.
   * @return The requested substring.
   */
  EXPORT const char *CALL Mid(const char *str, int pos, int num);

  /**
   * Returns a string with all occurrences of one substring replaced by another.
   *
   * @param str The source string.
   * @param find The substring to find.
   * @param replace The replacement substring.
   * @return The string with all replacements applied.
   */
  EXPORT const char *CALL Replace(const char *str, const char *find, const char *replace);

  /**
   * Finds a substring in a string starting from the given offset.
   *
   * @param str The string to search.
   * @param find The substring to find.
   * @param offset The starting offset for the search.
   * @return The position of the substring, or -1 if it is not found.
   */
  EXPORT int CALL Find(const char *str, const char *find, int offset);

  /**
   * Converts a string to uppercase.
   *
   * @param str The string to convert.
   * @return The uppercase string.
   */
  EXPORT const char *CALL Upper(const char *str);

  /**
   * Converts a string to lowercase.
   *
   * @param str The string to convert.
   * @return The lowercase string.
   */
  EXPORT const char *CALL Lower(const char *str);

  /**
   * Removes leading and trailing whitespace from a string.
   *
   * @param str The string to trim.
   * @return The trimmed string.
   */
  EXPORT const char *CALL Trim(const char *str);

  /**
   * Returns the character with the given ASCII/Unicode code.
   *
   * @param code The ASCII/Unicode code of the character.
   * @return A string containing the requested character.
   */
  EXPORT const char *CALL Char(int code);

  /**
   * Returns the ASCII/Unicode code of the first character of a string.
   *
   * @param str The string to read.
   * @return The ASCII/Unicode code of the first character.
   */
  EXPORT int CALL Code(const char *str);

  /**
   * Returns the number of characters in a string.
   *
   * @param str The string to measure.
   * @return The number of characters in the string.
   */
  EXPORT int CALL Len(const char *str);

  /**
   * Returns the number of substrings produced by splitting a string.
   *
   * @param str The string to split.
   * @param delimiter The delimiter to split on.
   * @return The number of substrings.
   */
  EXPORT int CALL SplitCount(const char *str, const char *delimiter);

  /**
   * Returns a substring at the given index after splitting a string.
   *
   * @param str The string to split.
   * @param delimiter The delimiter to split on.
   * @param index The index of the substring to return.
   * @return The substring at the requested index.
   */
  EXPORT const char *CALL SplitIndex(const char *str, const char *delimiter, int index);

  /**
   * Returns a filename without its extension.
   *
   * @param filename The filename to process.
   * @return The filename without its extension.
   */
  EXPORT const char *CALL StripExt(const char *filename);

  /**
   * Returns just the filename without the directory path.
   *
   * @param filename The path to process.
   * @return The filename without its directory path.
   */
  EXPORT const char *CALL StripDir(const char *filename);

  /**
   * Returns just the file extension.
   *
   * @param filename The filename to process.
   * @return The file extension.
   */
  EXPORT const char *CALL ExtractExt(const char *filename);

  /**
   * Returns just the directory part of a path.
   *
   * @param filename The path to process.
   * @return The directory part of the path.
   */
  EXPORT const char *CALL ExtractDir(const char *filename);

  /**
   * Returns the entire contents of a text file as a string.
   *
   * @param filename The path to the text file to load.
   * @return The contents of the text file.
   */
  EXPORT const char *CALL LoadString(const char *filename);

  /**
   * Saves a string to a file.
   *
   * @param str The string to save.
   * @param filename The path to the file to write.
   * @param append True to append to the file, or false to overwrite it.
   */
  EXPORT void CALL SaveString(const char *str, const char *filename, bool_t append);

  /**
   * Copies a string to the system clipboard.
   *
   * @param str The string to copy.
   */
  EXPORT void CALL CopyToClipboard(const char *str);

  /**
   * Returns the text currently on the system clipboard.
   *
   * @return The current clipboard text.
   */
  EXPORT const char *CALL PasteFromClipboard();

#ifdef __cplusplus
} /* extern "C" */
#endif
