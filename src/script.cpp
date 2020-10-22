#include "../lib/lua/lua.hpp"
#include "../lib/lua/lualib.h"
#include "../lib/lua/lauxlib.h"
#include "script.h"

extern "C" int luaopen_coldsteel(lua_State* l);

Script::Script() {
    // Create lua state and register libs
    mState = luaL_newstate();
    luaL_requiref(mState, "_G", luaopen_base, true); lua_pop(mState, 1);
    //luaL_requiref(mState, "string", luaopen_string, true); lua_pop(mState, 1);
    //luaL_requiref(mState, "table", luaopen_table, true); lua_pop(mState, 1);
    //luaL_requiref(mState, "math", luaopen_math, true); lua_pop(mState, 1);
    luaopen_coldsteel(mState);

    // Move definitions into global namespace
    luaL_dostring(mState, "for k,v in pairs(coldsteel) do _G[k]=v end");
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
