#include "../lib/lua/lua.hpp"
#include "../lib/lua/lualib.h"
#include "../lib/lua/lauxlib.h"
#include "script.h"
#include "sharedlib.h"
#include "string.h"

extern "C" int luaopen_coldsteel(lua_State* l);


Script& Script::Get() {
    static Script* instance = new Script();
    return *instance;
}


Script::Script() {
    // Create Lua state and register libs
    mState = luaL_newstate();
    luaL_requiref(mState, "_G", luaopen_base, true); lua_pop(mState, 1);
    luaopen_coldsteel(mState);
    lua_register(mState, "import", LuaImport);
    lua_register(mState, "load", LuaLoad);

    // Move definitions into global namespace
    luaL_dostring(mState, "for k,v in pairs(coldsteel) do _G[k]=v end");

    mSDK = GetSDK();
}


Script::~Script() {
    lua_close(mState);
}


bool Script::Load(const stringc& filename) {
    const stringc buffer = LoadString(filename.c_str());
    if (luaL_loadbuffer(mState, buffer.c_str(), buffer.size(), filename.c_str())
            || lua_pcall(mState, 0, LUA_MULTRET, 0)) {
        mError = lua_tostring(mState, -1);
        return false;
    } else {
        return true;
    }
}


bool Script::FunctionExists(const stringc& name) const {
    bool exists = false;
    lua_getglobal(mState, name.c_str());
    if (lua_isfunction(mState, -1)) exists = true;
    lua_pop(mState, 1);
    return exists;
}


bool Script::CallVoidFunction(const stringc& name) {
    mError = "";
    lua_getglobal(mState, name.c_str());
    if (lua_isfunction(mState, -1)) {
        if (lua_pcall(mState, 0, 1, 0)) mError = lua_tostring(mState, -1);
        else if (!lua_isnil(mState, -1)) mError = stringc("'") + name + "' function cannot return a value";
    } else {
        mError = stringc("'") + name + "' function does not exist";
    }
    lua_pop(mState, 1);
    return mError == "";
}


int Script::LuaImport(lua_State* L) {
    if (lua_gettop(L) > 0) {
        const stringc filename = lua_tostring(L, 1);
        const stringc buffer = LoadString(filename.c_str());
        if (buffer == "") {
            lua_pushstring(L, (stringc("File '") + filename + "' does not exist or is empty.").c_str());
            lua_error(L);
            return 0;
        }
        if (luaL_loadbuffer(L, buffer.c_str(), buffer.size(), filename.c_str()) == 0) {
            lua_pcall(L, 0, LUA_MULTRET, 0);
        } else {
            lua_error(L);
        }
    } else {
        lua_pushstring(L, "'import' requires filename argument.");
        lua_error(L);
    }
    return 0;
}


int Script::LuaLoad(lua_State* L) {
#ifndef _EMSCRIPTEN
    if (lua_gettop(L) > 0) {
        const stringc libname = lua_tostring(L, 1);
        sharedlib_t* lib = new sharedlib_t(libname.c_str());
        if (!lib->isopen()) {
            lua_pushstring(L, (stringc("Library '") + libname + "' could not be loaded.").c_str());
            lua_error(L);
            return 0;
        }
        int (*loader)(ColdSteelSDK*) = (int(*)(ColdSteelSDK*))lib->funcptr((libname + "_load").c_str());
        if (!loader) {
            lua_pushstring(L, (stringc("Library '") + libname + "' does not contain '" + libname + "_load' function.").c_str());
            lua_error(L);
            return 0;
        }
        if (!loader(&Get().mSDK)) {
            lua_pushstring(L, (stringc("Function '") + libname + "_load' returned 0.").c_str());
            lua_error(L);
            return 0;
        }
    } else {
        lua_pushstring(L, "'load' requires library argument.");
        lua_error(L);
    }
#else
    lua_pushstring(L, "'load' is not supported on the web.");
    lua_error(L);
#endif
    return 0;
}


ColdSteelSDK Script::GetSDK() {
    ColdSteelSDK sdk;
    sdk.RegisterFunction = RegisterFunction;
    sdk.GetBoolArg = GetBoolArg;
    sdk.GetIntArg = GetIntArg;
    sdk.GetFloatArg = GetFloatArg;
    sdk.GetStringArg = GetStringArg;
    sdk.GetPointerArg = GetPointerArg;
    sdk.PushBool = PushBool;
    sdk.PushInt = PushInt;
    sdk.PushFloat = PushFloat;
    sdk.PushString = PushString;
    sdk.PushPointer = PushPointer;
    return sdk;
}


void Script::RegisterFunction(const char* name, int (*f)(void*)) {
    lua_register(Get().mState, name, (lua_CFunction)f);
}


int Script::GetBoolArg(void* context, int index) {
    return lua_toboolean((lua_State*)context, index);
}


int Script::GetIntArg(void* context, int index) {
    return lua_tointeger((lua_State*)context, index);
}


float Script::GetFloatArg(void* context, int index) {
    return lua_tonumber((lua_State*)context, index);
}


const char* Script::GetStringArg(void* context, int index) {
    return lua_tostring((lua_State*)context, index);
}


const void* Script::GetPointerArg(void* context, int index) {
    return lua_topointer((lua_State*)context, index);
}


void Script::PushBool(void* context, int val) {
    lua_pushboolean((lua_State*)context, val);
}


void Script::PushInt(void* context, int val) {
    lua_pushinteger((lua_State*)context, val);
}


void Script::PushFloat(void* context, float val) {
    lua_pushnumber((lua_State*)context, val);
}


void Script::PushString(void* context, const char* val) {
    lua_pushstring((lua_State*)context, val);
}


void Script::PushPointer(void* context, void* val) {
    lua_pushlightuserdata((lua_State*)context, val);    
}
