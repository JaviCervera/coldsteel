#pragma once

#include "common.h"

struct lua_State;

class Script {
public:
    Script();
    ~Script();

    bool Load(const stringc& filename);
    const stringc& Error() const { return mError; }
private:
    lua_State* mState;
    stringc mError;
};
