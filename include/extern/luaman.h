#ifndef LUAMAN_H
#define LUAMAN_H

#include "lauxlib.h"
#include "lua.h"
#include <lualib.h>

#include <assert.h>

#define LM_VERSION_MAJOR 0
#define LM_VERSION_MINOR 0
#define LM_VERSION_PATCH 2
#define LM_VERSION "0.0.2"

#ifndef LMAPI
    #define LMAPI extern
#endif // LMAPI

typedef struct {
    lua_State* state;
} LuaMan;

LMAPI void LMInit(LuaMan* lm);
LMAPI void LMClose(LuaMan* lm);

LMAPI void LMPushFunction(LuaMan* lm, lua_CFunction fun, const char* name);
LMAPI void LMPushNumber(LuaMan* lm, double number, const char* name);
LMAPI void LMPushString(LuaMan* lm, const char* string, const char* name);

LMAPI double LMGetNumber(LuaMan* lm, const char* name);
LMAPI const char* LMGetString(LuaMan* lm, const char* name);
LMAPI double LMGetTableFieldNumber(LuaMan* lm, const char* table, const char* field);
LMAPI const char* LMGetTableFieldString(LuaMan* lm, const char* table, const char* field);

#define LMRunString(lm, string) (luaL_dostring((lm)->state, string) == LUA_OK)
#define LMRunFile(lm, path) (luaL_dofile((lm)->state, path) == LUA_OK) 

#define LMCallFunctionBegin(lm, func, arg_count, ret_count) \
        lua_getglobal((lm)->state, func); \
        if(lua_isfunction((lm)->state, -1)) { \
            const int __arguments_count__ = arg_count; \
            const int __returnvalues_count__ = ret_count; \

#define LMCallFunctionEnd(lm) \
            lua_pcall((lm)->state, __arguments_count__, __returnvalues_count__, 0); \
        }

#define LMParamNumber(lm, num) LMPushNumber(lm, num, NULL);
#define LMParamString(lm, str) LMPushString(lm, str, NULL);

#endif // LUAMAN_H

#ifdef LUAMAN_IMPLEMENTATION
LMAPI void LMInit(LuaMan* lm)
{
    lm->state = luaL_newstate();
    luaL_openlibs(lm->state);
}

LMAPI void LMClose(LuaMan* lm)
{
    lua_close(lm->state);
}

LMAPI void LMPushNumber(LuaMan* lm, double number, const char* name)
{
    lua_pushnumber(lm->state, number);
    if(name != NULL)
        lua_setglobal(lm->state, name);
}

LMAPI void LMPushString(LuaMan* lm, const char* string, const char* name)
{
    lua_pushstring(lm->state, string);
    if(name != NULL)
        lua_setglobal(lm->state, name);
}

LMAPI void LMPushFunction(LuaMan* lm, lua_CFunction fun, const char* name)
{
    assert(name);
    lua_pushcfunction((lm)->state, fun);
    lua_setglobal((lm)->state, name); 
}

LMAPI double LMGetNumber(LuaMan* lm, const char* name)
{
    if(name != NULL)
        lua_getglobal(lm->state, name);
    return lua_tonumber(lm->state, -1);
}

LMAPI const char* LMGetString(LuaMan* lm, const char* name)
{
    if(name != NULL)
        lua_getglobal(lm->state, name);
    return lua_tostring(lm->state, -1);
}


LMAPI double LMGetTableFieldNumber(LuaMan* lm, const char* table, const char* field)
{
    assert(table);
    assert(field);

    lua_getglobal(lm->state, table);
    if (lua_istable(lm->state, -1))  {
        lua_getfield(lm->state, -1, field);
        return lua_tonumber(lm->state, -1);
    }

    return 2147483647; // INT_MAX
}
    
LMAPI const char* LMGetTableFieldString(LuaMan* lm, const char* table, const char* field)
{
    assert(table);
    assert(field);

    lua_getglobal(lm->state, table);
    if (lua_istable(lm->state, -1))  {
        lua_getfield(lm->state, -1, field);
        return lua_tostring(lm->state, -1);
    } 

    return NULL;
}

#endif // LUAMAN_IMPLEMENTATION
