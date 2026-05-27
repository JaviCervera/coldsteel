/**
 * @file
 * File system functions for navigating directories and archives.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Mounts a ZIP archive into the virtual file system.
   *
   * @param filename The path to the ZIP archive to mount.
   * @return TRUE if the archive was mounted successfully, otherwise FALSE.
   */
  EXPORT bool_t CALL AddZip(const char *filename);

  /**
   * Returns the contents of a directory.
   *
   * @param dir The directory whose contents will be listed.
   * @return A newline-separated list of files and directories in dir.
   */
  EXPORT const char *CALL DirContents(const char *dir);

  /**
   * Changes the current working directory.
   *
   * @param dir The new working directory path.
   */
  EXPORT void CALL ChangeDir(const char *dir);

  /**
   * Returns the current working directory path.
   *
   * @return The current working directory path.
   */
  EXPORT const char *CALL CurrentDir();

#ifdef __cplusplus
} /* extern "C" */
#endif
