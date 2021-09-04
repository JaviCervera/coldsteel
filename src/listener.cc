#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#include "listener.h"

static vector3df _position;
static float _yaw;

extern "C" {


EXPORT void CALL SetListener(float x, float y, float z, float yaw) {
    alListener3f(AL_POSITION, x, y, z);
    alListener3f(AL_ORIENTATION, 0, yaw, 0);
    // TODO: Set alVelocity based on previous position
    _position = vector3df(x, y, z);
    _yaw = yaw;
}

EXPORT float CALL ListenerX() {
    return _position.X;
}


EXPORT float CALL ListenerY() {
    return _position.Y;
}


EXPORT float CALL ListenerZ() {
    return _position.Z;
}


EXPORT float CALL ListenerYaw() {
    return _yaw;
}


} // extern "C"
