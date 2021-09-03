#include "listener.h"

static vector3df _position;
static float _yaw;


extern "C" {


EXPORT void CALL SetListener(float x, float y, float z, float yaw) {
    _position.set(x, y, z);
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
