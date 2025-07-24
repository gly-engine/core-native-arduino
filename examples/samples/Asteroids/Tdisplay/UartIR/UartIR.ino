#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <GlyCore.h>
#include <GlyLua54.h>
#include <GlyEngineLite.h>
#include <GlySampleAsteroids.h>
#include <GlyInputUart.h>
#include <GlyDisplayTFT.h>

#define RXD_IR   17
#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  23
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_BL   4
#define BTN_A    0
#define BTN_B    35

GlyInputUart buttons(&Serial1, {
  {"up",    0x043B80},
  {"down",    0x043B81},
  {"right",    0x043B9C},
  {"left",    0x043B96},
  {"a",    0x043B87},
  {"a",    0x043B89},
  {"b",    0x043B8B},
  {"c",    0x043B99},
  {"d",    0x043B88},
});

SPIClass spiTFT(VSPI);
Adafruit_ST7789 tft(&spiTFT, TFT_CS, TFT_DC, TFT_RST);
GlyCore engine(GlySampleAsteroids, GlyEngineLite, &tft, &buttons);

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, RXD_IR, -1); 
    spiTFT.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

    engine.init(240, 135);
    engine.setFramerate(20);
    buttons.setTimeout(100);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
}
  
void loop() {
  engine.update();
  if (engine.hasErrors()) {
    Serial.println(engine.getErrors());
    while(1) delay(1000);
  }
}
