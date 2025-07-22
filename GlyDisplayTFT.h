#ifndef H_GLY_DISPLAY_TFT
#define H_GLY_DISPLAY_TFT

#include "GlyTypes.h"

#ifdef DOXYGEN
class GlyDisplayTFT: private Adafruit_ST7789, private Adafruit_ST7796S, private TFT_eSPI {};
#endif

//! @cond
namespace GlyDisplay {
#if defined(_ADAFRUIT_ST7789H_)
    Adafruit_ST7789 *tft = nullptr;
#elif defined(_ADAFRUIT_ST7796S_H_)
    Adafruit_ST7796S *tft = nullptr;
#elif defined(_TFT_eSPIH_)
    TFT_eSPI *tft = nullptr;
#else
#error "You must include a TFT library like <Adafruit_ST7789.h> or <TFT_eSPI.h> before including <GlyDisplayTFT.h>"
#endif
    uint16_t current_color = 0;
}
//! @endcond

void gly_hook_display_lib(decltype(GlyDisplay::tft) lib)
{
    GlyDisplay::tft = lib;
}

void gly_hook_display_init(uint16_t width, uint16_t height)
{
    if (!GlyDisplay::tft) return;
    if (width > height) {
#if defined(_TFT_eSPIH_)
        GlyDisplay::tft->init();
        GlyDisplay::tft->setRotation(3);
#else
        GlyDisplay::tft->init(height, width);
        GlyDisplay::tft->setRotation(1);
#endif
    } else {

#if defined(_TFT_eSPIH_)
        GlyDisplay::tft->init();
#else
        GlyDisplay::tft->init(height, width);
#endif
        GlyDisplay::tft->setRotation(0);
    }
}

void native_draw_start()
{
#if defined(_TFT_eSPIH_)
    GlyDisplay::tft->startWrite();
#endif
}

void native_draw_flush()
{
#if defined(_TFT_eSPIH_)
    GlyDisplay::tft->endWrite();
#endif
}

void native_draw_color(uint32_t color)
{
    if (!GlyDisplay::tft) return;
    color_u rgba = { color };
    GlyDisplay::current_color = GlyDisplay::tft->color565(rgba.r, rgba.g, rgba.b);
}

void native_draw_clear(uint32_t color)
{
    if (!GlyDisplay::tft) return;
    color_u rgba = { color };  
    GlyDisplay::tft->fillScreen(GlyDisplay::tft->color565(rgba.r, rgba.g, rgba.b));
}

void native_draw_rect(uint8_t mode, int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (!GlyDisplay::tft) return;
    if (!mode) {
        GlyDisplay::tft->fillRect(x, y, w, h, GlyDisplay::current_color);
    } else {
        GlyDisplay::tft->drawRect(x, y, w, h, GlyDisplay::current_color);
    }
}

void native_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if (!GlyDisplay::tft) return;
    GlyDisplay::tft->drawLine(x1, y1, x2, y2, GlyDisplay::current_color);
}

void native_text_print(int16_t x, int16_t y, const char* text)
{
    if (!GlyDisplay::tft) return;
    GlyDisplay::tft->setCursor(x, y);
    GlyDisplay::tft->setTextColor(GlyDisplay::current_color);
    GlyDisplay::tft->print(text);
}

void native_text_mensure(const char* text, int16_t* w, int16_t* h)
{
    if (!GlyDisplay::tft) return;
    int16_t x, y;
    uint16_t width = 1, height = 1;

#if !defined(_TFT_eSPIH_)
//! @todo support TFT_eSPI.h
    GlyDisplay::tft->getTextBounds(text, 0, 0, &x, &y, &width, &height);
#endif

    if (w) *w =  width;
    if (h) *h =  height;
}

void native_text_font_size(uint8_t size)
{
    if (!GlyDisplay::tft) return;
    GlyDisplay::tft->setTextSize(size/3);
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
