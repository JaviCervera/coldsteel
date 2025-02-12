#include "bitwise.h"

int BitAnd(int a, int b)
{
  return a & b;
}

int BitOr(int a, int b)
{
  return a | b;
}

int BitXor(int a, int b)
{
  return a ^ b;
}

int BitNot(int val)
{
  return ~val;
}

int Shl(int val, int count)
{
  return val << count;
}

int Shr(int val, int count)
{
  return val >> count;
}
