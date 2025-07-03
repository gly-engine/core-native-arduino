#include "GlyAdafruitST7789.h"

static Adafruit_ST7789 *tft = nullptr;
static uint16_t current_color = 0;

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

void native_draw_start(void)
{

}
void native_draw_flush(void)
{

}

void native_draw_color(uint32_t color)
{
    if (!tft) return;
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    union { uint32_t value; struct { uint8_t a, b, g, r;};} rgba = { color };
#else
    union { uint32_t value; struct { uint8_t r, g, b, a;};} rgba = { color };
#endif    
    current_color = tft->color565(rgba.r, rgba.g, rgba.b);
}

void native_draw_clear(uint32_t color)
{
    if (!tft) return;

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    union { uint32_t value; struct { uint8_t a, b, g, r;};} rgba = { color };
#else
    union { uint32_t value; struct { uint8_t r, g, b, a;};} rgba = { color };
#endif    
    tft->fillScreen(tft->color565(rgba.r, rgba.g, rgba.b));
}

void native_draw_rect(uint8_t mode, int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (!tft) return;
    if (!mode) {
        tft->fillRect(x, y, w, h, current_color);
    } else {
        tft->drawRect(x, y, w, h, current_color);
    }
}

void native_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if (!tft) return;
    tft->drawLine(x1, y1, x2, y2, current_color);
}

void native_text_print(int16_t x, int16_t y, const char* text)
{
    if (!tft) return;
    tft->setCursor(x, y);
    tft->setTextColor(current_color);
    tft->print(text);
}

void native_text_mensure(const char* text, int16_t* w, int16_t* h)
{
    if (!tft) return;
    int16_t x, y;
    uint16_t width, height;

    tft->getTextBounds(text, 0, 0, &x, &y, &width, &height);

    if (w) *w =  width;
    if (h) *h =  height;
}

void native_text_font_size(uint8_t size)
{
    if (!tft) return;
    tft->setTextSize(size/3);
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

void native_image_draw(int16_t, int16_t, const char*)
{

}
