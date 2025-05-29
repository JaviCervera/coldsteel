#pragma once

struct Audio;
struct FileSystem;
struct Platform;

struct Engine
{
  virtual Audio &GetAudio() = 0;
  virtual FileSystem &GetFileSystem() = 0;
  virtual Platform &GetPlatform() = 0;
};
