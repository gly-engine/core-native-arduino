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
CREATE_GLY_HOOK(void, native_draw_start, ())
CREATE_GLY_HOOK(void, native_draw_flush, ())
CREATE_GLY_HOOK(void, native_draw_color, (uint32_t))
CREATE_GLY_HOOK(void, native_draw_clear, (uint32_t))
CREATE_GLY_HOOK(void, native_draw_rect, (uint8_t, float, float, float, float))
CREATE_GLY_HOOK(void, native_draw_line, (float, float, float, float))
CREATE_GLY_HOOK(void, native_text_print, (float, float, const char*))
CREATE_GLY_HOOK(void, native_text_mensure, (const char*, float*, float*))
CREATE_GLY_HOOK(void, native_text_font_size, (uint8_t))
CREATE_GLY_HOOK(void, native_text_font_name, (const char*))
CREATE_GLY_HOOK(void, native_text_font_default, (uint8_t))
CREATE_GLY_HOOK(void, native_text_font_previous, ())
CREATE_GLY_HOOK(void, native_image_draw, (float, float, const char*))
