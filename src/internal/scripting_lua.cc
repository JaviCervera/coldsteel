extern "C"
{
#include "../../lib/lua/lua.h"
#include "../../lib/lua/lualib.h"
#include "../../lib/lua/lauxlib.h"
}
#include "../../_build/sdk/sdk.h"
#include "../engine.h"
#include "scripting.h"
#include "sharedlib.h"
#undef DrawText
#undef LoadString

extern "C" int luaopen_coldsteel(lua_State *l);

class Scripting_Lua : public Scripting
{
public:
  Scripting_Lua()
  {
    // Create Lua state and register libs
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    luaopen_coldsteel(m_state);
    lua_register(m_state, "import", LuaImport);
    lua_register(m_state, "load", LuaLoad);

    // Move definitions into global namespace
    luaL_dostring(m_state, "for k,v in pairs(coldsteel) do _G[k]=v end");

    m_sdk = GetSDK();
  }

  ~Scripting_Lua()
  {
    lua_close(m_state);
  }

  bool Load(const char *filename)
  {
    Memblock *memblock = LoadMemblock(filename);
    if (!memblock)
    {
      m_error = stringc("Cannot find file: ") + CurrentDir() + "/" + filename;
      return false;
    }
    if (luaL_loadbuffer(m_state, (const char *)memblock, MemblockSize(memblock), filename) || lua_pcall(m_state, 0, LUA_MULTRET, 0))
    {
      m_error = Replace(Replace(lua_tostring(m_state, -1), "\"", "`"), "'", "`");
      FreeMemblock(memblock);
      return false;
    }
    else
    {
      FreeMemblock(memblock);
      return true;
    }
  }

  const char *Error() const
  {
    return m_error.c_str();
  }

private:
  lua_State *m_state;
  stringc m_error;
  ColdSteelSDK m_sdk;

  static int LuaImport(lua_State *L)
  {
    if (lua_gettop(L) > 0)
    {
      const stringc filename = lua_tostring(L, 1);
      const stringc fixedFilename = (Find(filename.c_str(), ".", 0) == -1)
                                        ? (filename + ".lua")
                                        : filename;
      const stringc buffer = LoadString(fixedFilename.c_str());
      if (buffer == "")
      {
        lua_pushstring(L, (stringc("File '") + fixedFilename + "' does not exist or is empty.").c_str());
        lua_error(L);
        return 0;
      }
      if (luaL_loadbuffer(L, buffer.c_str(), buffer.size(), fixedFilename.c_str()) == 0)
      {
        lua_pcall(L, 0, LUA_MULTRET, 0);
      }
      else
      {
        lua_error(L);
      }
    }
    else
    {
      lua_pushstring(L, "'import' requires filename argument.");
      lua_error(L);
    }
    return 0;
  }

  static int LuaLoad(lua_State *L)
  {
#ifndef _EMSCRIPTEN
    if (lua_gettop(L) > 0)
    {
      const stringc libname = lua_tostring(L, 1);
      sharedlib_t *lib = new sharedlib_t(libname.c_str());
      if (!lib->isopen())
      {
        lua_pushstring(L, (stringc("Library '") + libname + "' could not be loaded.").c_str());
        lua_error(L);
        return 0;
      }
      int (*loader)(ColdSteelSDK *) = (int (*)(ColdSteelSDK *))lib->funcptr((libname + "_load").c_str());
      if (!loader)
      {
        lua_pushstring(L, (stringc("Library '") + libname + "' does not contain '" + libname + "_load' function.").c_str());
        lua_error(L);
        return 0;
      }
      if (!loader(&((Scripting_Lua &)Get()).m_sdk))
      {
        lua_pushstring(L, (stringc("Function '") + libname + "_load' returned 0.").c_str());
        lua_error(L);
        return 0;
      }
    }
    else
    {
      lua_pushstring(L, "'load' requires library argument.");
      lua_error(L);
    }
#else
    lua_pushstring(L, "'load' is not supported on the web.");
    lua_error(L);
#endif
    return 0;
  }

  static ColdSteelSDK GetSDK();

  static void RegisterFunction(const char *name, int (*f)(void *))
  {
    lua_register(((Scripting_Lua &)Get()).m_state, name, (lua_CFunction)f);
  }

  static int GetBoolArg(void *context, int index)
  {
    return lua_toboolean((lua_State *)context, index);
  }

  static int GetIntArg(void *context, int index)
  {
    return lua_tointeger((lua_State *)context, index);
  }

  static float GetFloatArg(void *context, int index)
  {
    return lua_tonumber((lua_State *)context, index);
  }

  static const char *GetStringArg(void *context, int index)
  {
    return lua_tostring((lua_State *)context, index);
  }

  static const void *GetPointerArg(void *context, int index)
  {
    return lua_topointer((lua_State *)context, index);
  }

  static void PushBool(void *context, int val)
  {
    lua_pushboolean((lua_State *)context, val);
  }

  static void PushInt(void *context, int val)
  {
    lua_pushinteger((lua_State *)context, val);
  }

  static void PushFloat(void *context, float val)
  {
    lua_pushnumber((lua_State *)context, val);
  }

  static void PushString(void *context, const char *val)
  {
    lua_pushstring((lua_State *)context, val);
  }

  static void PushPointer(void *context, void *val)
  {
    lua_pushlightuserdata((lua_State *)context, val);
  }
};

Scripting &Scripting::Get()
{
  static Scripting_Lua *instance = new Scripting_Lua();
  return *instance;
}

#include "get_sdk_impl.h"
