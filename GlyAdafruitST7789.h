#ifndef H_GLY_ADAFRUIT_ST7789
#define H_GLY_ADAFRUIT_ST7789

#include <cstdint>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

namespace GlyAdafruitST7789 {
    Adafruit_ST7789 *tft = nullptr;
    uint16_t current_color = 0;
}

void gly_hook_display_lib(Adafruit_ST7789 *lib)
{
    GlyAdafruitST7789::tft = lib;
}

void gly_hook_display_init(uint16_t width, uint16_t height)
{
    if (!GlyAdafruitST7789::tft) return;
    if (width > height) {
        GlyAdafruitST7789::tft->init(height, width);
        GlyAdafruitST7789::tft->setRotation(1);
    } else {
        GlyAdafruitST7789::tft->init(height, width);
        GlyAdafruitST7789::tft->setRotation(0);
    }
}

void native_draw_start()
{

}
void native_draw_flush()
{

}

void native_draw_color(uint32_t color)
{
    if (!GlyAdafruitST7789::tft) return;
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    union { uint32_t value; struct { uint8_t a, b, g, r;};} rgba = { color };
#else
    union { uint32_t value; struct { uint8_t r, g, b, a;};} rgba = { color };
#endif    
    GlyAdafruitST7789::current_color = GlyAdafruitST7789::tft->color565(rgba.r, rgba.g, rgba.b);
}

void native_draw_clear(uint32_t color)
{
    if (!GlyAdafruitST7789::tft) return;

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    union { uint32_t value; struct { uint8_t a, b, g, r;};} rgba = { color };
#else
    union { uint32_t value; struct { uint8_t r, g, b, a;};} rgba = { color };
#endif    
    GlyAdafruitST7789::tft->fillScreen(GlyAdafruitST7789::tft->color565(rgba.r, rgba.g, rgba.b));
}

void native_draw_rect(uint8_t mode, int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (!GlyAdafruitST7789::tft) return;
    if (!mode) {
        GlyAdafruitST7789::tft->fillRect(x, y, w, h, GlyAdafruitST7789::current_color);
    } else {
        GlyAdafruitST7789::tft->drawRect(x, y, w, h, GlyAdafruitST7789::current_color);
    }
}

void native_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if (!GlyAdafruitST7789::tft) return;
    GlyAdafruitST7789::tft->drawLine(x1, y1, x2, y2, GlyAdafruitST7789::current_color);
}

void native_text_print(int16_t x, int16_t y, const char* text)
{
    if (!GlyAdafruitST7789::tft) return;
    GlyAdafruitST7789::tft->setCursor(x, y);
    GlyAdafruitST7789::tft->setTextColor(GlyAdafruitST7789::current_color);
    GlyAdafruitST7789::tft->print(text);
}

void native_text_mensure(const char* text, int16_t* w, int16_t* h)
{
    if (!GlyAdafruitST7789::tft) return;
    int16_t x, y;
    uint16_t width, height;

    GlyAdafruitST7789::tft->getTextBounds(text, 0, 0, &x, &y, &width, &height);

    if (w) *w =  width;
    if (h) *h =  height;
}

void native_text_font_size(uint8_t size)
{
    if (!GlyAdafruitST7789::tft) return;
    GlyAdafruitST7789::tft->setTextSize(size/3);
}

void native_text_font_name(const char*)
{

}
void native_text_font_default(uint8_t)
{

}

void native_text_font_previous()
{

}

void native_image_draw(int16_t, int16_t, const char*)
{

}

#endif
