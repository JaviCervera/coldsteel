#include "channel.h"
#include "listener.h"

static vector3df _position;
static float _yaw;


extern "C" {


EXPORT void CALL asSetListener(float x, float y, float z, float yaw) {
    _position.set(x, y, z);
    _yaw = yaw;
    _asUpdateChannels();
}

EXPORT float CALL asListenerX() {
    return _position.X;
}


EXPORT float CALL asListenerY() {
    return _position.Y;
}


EXPORT float CALL asListenerZ() {
    return _position.Z;
}


EXPORT float CALL asListenerYaw() {
    return _yaw;
}


} // extern "C"
