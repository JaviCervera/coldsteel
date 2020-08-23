#include "channel.h"
#include "listener.h"

static vector3df _position;
static float _yaw;


extern "C" {


EXPORT void CALL csSetListener(float x, float y, float z, float yaw) {
    _position.set(x, y, z);
    _yaw = yaw;
    _csUpdateChannels();
}

EXPORT float CALL csListenerX() {
    return _position.X;
}


EXPORT float CALL csListenerY() {
    return _position.Y;
}


EXPORT float CALL csListenerZ() {
    return _position.Z;
}


EXPORT float CALL csListenerYaw() {
    return _yaw;
}


} // extern "C"
