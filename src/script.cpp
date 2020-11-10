#include "../lib/lua/lua.hpp"
#include "../lib/lua/lualib.h"
#include "../lib/lua/lauxlib.h"
#include "script.h"
#include "string.h"

extern "C" int luaopen_coldsteel(lua_State* l);

Script::Script() {
    // Create lua state and register libs
    mState = luaL_newstate();
    luaL_requiref(mState, "_G", luaopen_base, true); lua_pop(mState, 1);
    luaopen_coldsteel(mState);
    lua_register(mState, "import", Import);

    // Move definitions into global namespace
    luaL_dostring(mState, "for k,v in pairs(coldsteel) do _G[k]=v end");

    // Define __class table
    if (luaL_dostring(mState,   "__class = {} "
                                "function __class:new() local obj = {} setmetatable(obj, self) return obj end "
                                "__class.__index = __class")) {
        mError = lua_tostring(mState, -1);
    }

    // Define class function
    if (luaL_dostring(mState,   "function class(parent) "
                                "if parent == nil then parent = __class end "
                                "local c = {} "
                                "setmetatable(c, parent) "
                                "c.__index = c "
                                "return c "
                                "end")) {
        mError = lua_tostring(mState, -1);
    }
}

Script::~Script() {
    lua_close(mState);
}

bool Script::Load(const stringc& filename) {
    if (luaL_dofile(mState, filename.c_str())) {
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


int Script::Import(lua_State* L) {
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
        lua_pushstring(L, "import requires filename argument.");
        lua_error(L);
    }
    return 0;
}
