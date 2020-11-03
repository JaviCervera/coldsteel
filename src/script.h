#pragma once

#include "common.h"

struct lua_State;

class Script {
public:
    Script();
    ~Script();

    bool Load(const stringc& filename);
    bool FunctionExists(const stringc& name) const;
    bool CallVoidFunction(const stringc& name);
    const stringc& Error() const { return mError; }
private:
    lua_State* mState;
    stringc mError;
};
