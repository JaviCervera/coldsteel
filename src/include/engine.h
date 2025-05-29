#pragma once

struct Audio;
struct Platform;

struct Engine
{
  virtual ~Engine() {}
  virtual Audio &GetAudio() = 0;
  virtual Platform &GetPlatform() = 0;
};
