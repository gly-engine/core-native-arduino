
#include <SPI.h>
#include <GlyCore.h>
#include <GlyLua54.h>
#include <GlyEngine.h>
#include <GlySamplePong.h>
#include <GlyAdafruitST7789.h>

#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  23
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_BL   4
#define BTN_A    0
#define BTN_B    35

SPIClass spiTFT(VSPI);
Adafruit_ST7789 tft(&spiTFT, TFT_CS, TFT_DC, TFT_RST);
GlyCore engine(GlySamplePong, GlyEngine, &tft);

void setup() {
    Serial.begin(115200);
    spiTFT.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

    engine.init(240, 135);
    engine.setFramerate(30);
    engine.setBtnDebounce(10);
    engine.setBtnKeyboard(BTN_A, INPUT_PULLUP, "up");
    engine.setBtnKeyboard(BTN_B, INPUT_PULLUP, "down");
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
}
  
void loop() {
  if (engine.hasErrors()) {
    Serial.println("Lua MEM:" + String(engine.getLuaMemTotal()) + " " + String(engine.getLuaMemPercentage()) + "%");
    Serial.println(engine.getErrors());
    for(;;);
  }
  if (engine.update()) {
    Serial.print(engine.getInstantFPS());
    Serial.print(" ");
    Serial.println(engine.getAvarageFPS());
  }
}