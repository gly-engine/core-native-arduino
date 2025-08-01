
#ifndef H_GLY_LUA54
#define H_GLY_LUA54

#ifdef DOXYGEN
class GlyLua54 { int; };
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LUA_CORE
#define LUA_LIB
#define ltable_c
#define lvm_c

#include "vendor/lua54/lua.h"
#include "vendor/lua54/lualib.h"
#include "vendor/lua54/lauxlib.h"

#include "vendor/lua54/lzio.c"
#include "vendor/lua54/lctype.c"
#include "vendor/lua54/lopcodes.c"
#include "vendor/lua54/lmem.c"
#include "vendor/lua54/lundump.c"
#include "vendor/lua54/ldump.c"
#include "vendor/lua54/lstate.c"
#include "vendor/lua54/lgc.c"
#include "vendor/lua54/llex.c"
#include "vendor/lua54/lcode.c"
#include "vendor/lua54/lparser.c"
#include "vendor/lua54/ldebug.c"
#include "vendor/lua54/lfunc.c"
#include "vendor/lua54/lobject.c"
#include "vendor/lua54/ltm.c"
#include "vendor/lua54/lstring.c"
#include "vendor/lua54/ltable.c"
#include "vendor/lua54/ldo.c"
#include "vendor/lua54/lvm.c"
#include "vendor/lua54/lapi.c"

#include "vendor/lua54/lbaselib.c"
#include "vendor/lua54/lcorolib.c"
#include "vendor/lua54/ldblib.c"
#include "vendor/lua54/liolib.c"
#include "vendor/lua54/lmathlib.c"
#include "vendor/lua54/loadlib.c"
#include "vendor/lua54/loslib.c"
#include "vendor/lua54/lstrlib.c"
#include "vendor/lua54/ltablib.c"
#include "vendor/lua54/lutf8lib.c"
#include "vendor/lua54/linit.c"
#include "vendor/lua54/lauxlib.c"

#ifdef __cplusplus
}
#endif
#endif
