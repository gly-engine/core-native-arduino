#include "GlyAdafruitST7789.h"

static Adafruit_ST7789 *tft = nullptr;

void gly_hook_display_lib(Adafruit_ST7789 *lib)
{
    tft = lib;
}

void gly_hook_display_init(uint16_t width, uint16_t height)
{
    Serial.println("GlyAdafruitST7789 hook gly_hook_init");
    if (!tft) return;
    Serial.println("GlyAdafruitST7789 hook gly_hook_init called!");
    tft->init(width, height);
    tft->setRotation(1);
    tft->fillScreen(ST77XX_GREEN);
    tft->setCursor(10, 10);
    tft->setTextColor(ST77XX_WHITE);
    tft->setTextSize(2);
    tft->println("Hello T-Diseeeeplay!");
    Serial.println("GlyAdafruitST7789 hook gly_hook_init ended!!");
}
