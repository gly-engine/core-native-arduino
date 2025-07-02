#ifdef GLY_HOOK_IMPL
#define CREATE_GLY_HOOK(ret, name, args) \
    __attribute__((weak)) ret name args {}
#else
#define CREATE_GLY_HOOK(ret, name, args) \
    __attribute__((weak)) ret name args;
#endif

#include <cstring>

CREATE_GLY_HOOK(void, gly_hook_display_lib, ())
CREATE_GLY_HOOK(void, gly_hook_display_init, (uint16_t, uint16_t))
