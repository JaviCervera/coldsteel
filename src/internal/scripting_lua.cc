extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "../../_build/sdk/sdk.h"
#include "../engine.h"
#include "scripting.h"
#include "sharedlib.h"
#include "utils.h"
#undef DrawText
#undef LoadString

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(int, cs_load_wasm_module, (const char *url, const char *path), {
  return Asyncify.handleSleep(function(wakeUp) {
    fetch(UTF8ToString(url))
      .then(function(r) { return r.arrayBuffer(); })
      .then(function(bytes) {
        FS.writeFile(UTF8ToString(path), new Uint8Array(bytes));
        wakeUp(0);
      })
      .catch(function() { wakeUp(1); });
  });
});
#endif

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
    if (lua_gettop(L) > 0)
    {
      const stringc libname = lua_tostring(L, 1);
#ifdef __EMSCRIPTEN__
      const stringc wasmPath = stringc("/") + libname;
      if (cs_load_wasm_module((libname + ".wasm").c_str(), (wasmPath + ".wasm").c_str()))
      {
        lua_pushstring(L, (stringc("Library '") + libname + ".wasm' could not be fetched.").c_str());
        return 1;
      }
      sharedlib_t *lib = new sharedlib_t(wasmPath.c_str());
#else
      sharedlib_t *lib = new sharedlib_t((stringc(CurrentDir()) + "/" + libname.c_str()).c_str());
      if (!lib->isopen())
      {
        delete lib;
        lib = new sharedlib_t((BinDir() + "/" + libname.c_str()).c_str());
      }
#endif
      if (!lib->isopen())
      {
        delete lib;
        lua_pushstring(L, (stringc("Library '") + libname + "' could not be loaded.").c_str());
        return 1;
      }
      int (*loader)(ColdSteelSDK *) = (int (*)(ColdSteelSDK *))lib->funcptr((libname + "_load").c_str());
      if (!loader)
      {
        lua_pushstring(L, (stringc("Library '") + libname + "' does not contain '" + libname + "_load' function.").c_str());
        return 1;
      }
      const int moduleVersion = loader(&((Scripting_Lua &)Get()).m_sdk);
      if (moduleVersion != COLDSTEEL_SDK_VERSION)
      {
        char buf[256];
        snprintf(buf, sizeof(buf), "Library '%s' was built for SDK version %d but ColdSteel expects %d.",
                 libname.c_str(), moduleVersion, COLDSTEEL_SDK_VERSION);
        lua_pushstring(L, buf);
        return 1;
      }
    }
    else
    {
      lua_pushstring(L, "'load' requires library argument.");
      return 1;
    }
    lua_pushstring(L, "");
    return 1;
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
    struct SWIG_Ptr
    {
      void *type;
      int own;
      void *ptr;
    };

    lua_State *L = (lua_State *)context;
    if (lua_islightuserdata(L, index))
      return lua_touserdata(L, index);
    if (lua_isuserdata(L, index))
    {
      struct SWIG_Ptr *ud = (struct SWIG_Ptr *)lua_touserdata(L, index);
      return ud ? ud->ptr : NULL;
    }
    return NULL;
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
