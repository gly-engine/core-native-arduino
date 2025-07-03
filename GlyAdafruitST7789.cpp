#include "GlyAdafruitST7789.h"

static Adafruit_ST7789 *tft = nullptr;

void gly_hook_display_lib(Adafruit_ST7789 *lib)
{
    tft = lib;
}

void gly_hook_display_init(uint16_t width, uint16_t height)
{
    if (!tft) return;
    if (width > height) {
        tft->init(height, width);
        tft->setRotation(1);
    } else {
        tft->init(height, width);
        tft->setRotation(0);
    }
}

void gly_hook_display_rot(uint8_t rot)
{
    if (!tft) return;
}

void native_draw_start(void) {}
void native_draw_flush(void) {}

void native_draw_color(uint32_t color)
{

}

void native_draw_clear(uint32_t)
{
    if (!tft) return;
    tft->fillScreen(ST77XX_BLACK);
}

void native_draw_rect(uint8_t mode, float x, float y, float w, float h)
{
    if (!tft) return;
    if (!mode) {
        tft->fillRect((int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, tft->color565(255, 255, 255));
    } else {
        tft->drawRect((int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, tft->color565(255, 255, 255));
    }
}

void native_draw_line(float, float, float, float)
{

}

void native_text_print(float, float, const char*)
{

}

void native_text_mensure(const char*, float*, float*)
{

}

void native_text_font_size(uint8_t)
{

}

void native_text_font_name(const char*)
{

}
void native_text_font_default(uint8_t)
{

}

void native_text_font_previous(void)
{

}

void native_image_draw(float, float, const char*)
{

}
