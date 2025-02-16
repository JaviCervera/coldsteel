#include "audio_driver.h"
#include "listener.h"

void SetListener(float x, float y, float z, float yaw)
{
  AudioDriver::Get().SetListener(x, y, z, yaw);
}

float ListenerX()
{
  return AudioDriver::Get().ListenerX();
}

float ListenerY()
{
  return AudioDriver::Get().ListenerY();
}

float ListenerZ()
{
  return AudioDriver::Get().ListenerZ();
}

float ListenerYaw()
{
  return AudioDriver::Get().ListenerYaw();
}
