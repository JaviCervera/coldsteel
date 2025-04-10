#pragma once

#include "common.h"
#include "sdk.h"

struct lua_State;

class Script
{
public:
  static Script &Get();
  bool Load(const stringc &filename);
  const stringc &Error() const { return mError; }

protected:
  Script();
  ~Script();

private:
  lua_State *mState;
  stringc mError;
  ColdSteelSDK mSDK;

  static int LuaImport(lua_State *L);
  static int LuaLoad(lua_State *L);

  static ColdSteelSDK GetSDK();
  static void RegisterFunction(const char *name, int (*f)(void *));
  static int GetBoolArg(void *context, int index);
  static int GetIntArg(void *context, int index);
  static float GetFloatArg(void *context, int index);
  static const char *GetStringArg(void *context, int index);
  static const void *GetPointerArg(void *context, int index);
  static void PushBool(void *context, int val);
  static void PushInt(void *context, int val);
  static void PushFloat(void *context, float val);
  static void PushString(void *context, const char *val);
  static void PushPointer(void *context, void *val);
};
