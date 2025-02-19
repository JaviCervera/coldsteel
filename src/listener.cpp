#include "audio_driver.h"
#include "listener.h"

void SetListener(float x, float y, float z, float yaw)
{
  AudioDriver::Get().SetListener(x, y, z, yaw);
}
