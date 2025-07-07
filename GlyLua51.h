
#ifndef H_GLY_LUA51
#define H_GLY_LUA51
#ifdef __cplusplus
extern "C" {
#endif

#define LUA_CORE
#define LUA_LIB
#define ltable_c
#define lvm_c

#include "vendor/lua51/lua.h"
#include "vendor/lua51/lualib.h"
#include "vendor/lua51/lauxlib.h"

#include "vendor/lua51/lapi.c"
#include "vendor/lua51/lauxlib.c"
#include "vendor/lua51/lbaselib.c"
#include "vendor/lua51/lcode.c"
#include "vendor/lua51/ldblib.c"
#include "vendor/lua51/ldebug.c"
#include "vendor/lua51/ldo.c"
#include "vendor/lua51/ldump.c"
#include "vendor/lua51/lfunc.c"
#include "vendor/lua51/lgc.c"
#include "vendor/lua51/linit.c"
#include "vendor/lua51/liolib.c"
#include "vendor/lua51/llex.c"
#include "vendor/lua51/lmathlib.c"
#include "vendor/lua51/lmem.c"
#include "vendor/lua51/loadlib.c"
#include "vendor/lua51/lobject.c"
#include "vendor/lua51/lopcodes.c"
#include "vendor/lua51/loslib.c"
#include "vendor/lua51/lparser.c"
#include "vendor/lua51/lstate.c"
#include "vendor/lua51/lstring.c"
#include "vendor/lua51/lstrlib.c"
#include "vendor/lua51/ltable.c"
#include "vendor/lua51/ltablib.c"
#include "vendor/lua51/ltests.c"
#include "vendor/lua51/ltm.c"
#include "vendor/lua51/lundump.c"
#include "vendor/lua51/lvm.c"
#include "vendor/lua51/lzio.c"

#undef lua_getglobal
#undef lua_setglobal

lua_Number lua_version(lua_State* L) {
    UNUSED(L);
    return 501;
}

int lua_getglobal(lua_State* L, const char *k) {
    lua_getfield(L, LUA_GLOBALSINDEX, k);
    return lua_type(L, -1);
}

void lua_setglobal(lua_State* L, const char *k) {
    lua_setfield(L, LUA_GLOBALSINDEX, k);
}

int luaL_loadbufferx(lua_State *L, const char *buff, size_t sz, const char *name, const char *mode) {
    return luaL_loadbuffer(L,buff,sz,name);
}

int lua_pcallk(lua_State *L, int nargs, int nresults, int errfunc, int ctx, void* k) {
    return lua_pcall(L, nargs, nresults, errfunc);
}

#ifdef __cplusplus
}
#endif
#endif
