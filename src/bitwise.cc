#include "bitwise.h"

extern "C" {


EXPORT int CALL BitAnd(int a, int b) {
    return a & b;
}


EXPORT int CALL BitOr(int a, int b) {
    return a | b;
}


EXPORT int CALL BitXor(int a, int b) {
    return a ^ b;
}


EXPORT int CALL BitNot(int val) {
    return ~val;
}


EXPORT int CALL Shl(int val, int count) {
    return val << count;
}


EXPORT int CALL Shr(int val, int count) {
    return val >> count;
}


} // extern "C"
