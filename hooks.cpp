
#if defined(GLY_HOOK_NATIVE)
#define CREATE_GLY_HOOK(ret, name, args) \
    ret name args;
#elif defined(GLY_HOOK_IMPL)
#define CREATE_GLY_HOOK(ret, name, args) \
    __attribute__((weak)) ret name args {}
#else
#define CREATE_GLY_HOOK(ret, name, args) \
    __attribute__((weak)) ret name args;
#endif

#include "GlyTypes.h"

#ifndef GLY_HOOK_NATIVE
CREATE_GLY_HOOK(void, gly_hook_display_lib, ())
CREATE_GLY_HOOK(void, gly_hook_display_lib, (nullptr_­t))
CREATE_GLY_HOOK(void, gly_hook_display_init, (uint16_t, uint16_t))
#endif

CREATE_GLY_HOOK(void, native_draw_start, ())
CREATE_GLY_HOOK(void, native_draw_flush, ())
CREATE_GLY_HOOK(void, native_draw_color, (uint32_t))
CREATE_GLY_HOOK(void, native_draw_clear, (uint32_t))
CREATE_GLY_HOOK(void, native_draw_rect, (uint8_t, int16_t, int16_t, int16_t, int16_t))
CREATE_GLY_HOOK(void, native_draw_line, (int16_t, int16_t, int16_t, int16_t))
CREATE_GLY_HOOK(void, native_text_print, (int16_t, int16_t, const char*))
CREATE_GLY_HOOK(void, native_text_mensure, (const char*, int16_t*, int16_t*))
CREATE_GLY_HOOK(void, native_text_font_size, (uint8_t))
CREATE_GLY_HOOK(void, native_text_font_name, (const char*))
CREATE_GLY_HOOK(void, native_text_font_default, (uint8_t))
CREATE_GLY_HOOK(void, native_text_font_previous, ())
CREATE_GLY_HOOK(void, native_image_draw, (int16_t, int16_t, const char*))

#undef CREATE_GLY_HOOK
