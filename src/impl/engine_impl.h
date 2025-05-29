#include "../include/engine.h"
#include "audio_soloud.h"
#include "platform_irrlicht.h"

struct Engine_Impl : public Engine
{
  Engine_Impl(const char *working_dir) : m_audio(), m_platform(working_dir) {}

  Audio &GetAudio()
  {
    return m_audio;
  }

  Platform &GetPlatform()
  {
    return m_platform;
  }

private:
  Audio_SoLoud m_audio;
  Platform_Irrlicht m_platform;
};
