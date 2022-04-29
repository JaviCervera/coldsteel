#pragma once

#include "common.h"

struct lua_State;

class Script {
public:
    static Script& Get();
    bool Load(const stringc& filename);
    bool FunctionExists(const stringc& name) const;
    bool CallVoidFunction(const stringc& name);
    const stringc& Error() const { return mError; }
protected:
    Script();
    ~Script();
private:
    lua_State* mState;
    stringc mError;
    
    static int Import(lua_State* L);
};
