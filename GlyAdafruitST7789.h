#ifndef H_GLY_ADAFRUIT_ST7789
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

void gly_hook_display_lib(Adafruit_ST7789 *lib);
void gly_hook_display_init(uint16_t width, uint16_t height);

#define GLY_HOOK_NATIVE
#include "hooks.cpp"

#endif
