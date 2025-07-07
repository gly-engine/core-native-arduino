
#include <SPI.h>
#include <GlyCore.h>
#include <GlyLua54.h>
#include <GlyEngine.h>
#include <GlySamplePong.h>
#include <GlyTFTeSPI.h>

#define BTN_A    0
#define BTN_B    35

TFT_eSPI tft;
GlyCore engine(GlySamplePong, GlyEngine, &tft);

void setup() {
    Serial.begin(115200);

    engine.init(240, 135);
    engine.setFramerate(40);
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