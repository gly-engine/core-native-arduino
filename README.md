# Core Native Arduino

> Embed games and applications made in gly engine on your esp32, esp8266, raspbarry, stm.. and other devices using arduino ecosystem.

### Features

 * Supports Lua 5.1 and 5.4 _(include `GlyLua51.h` or `GlyLua54.h`)_
 * supports third-party graphics libraries (`Adafruit_GFX.h` or `TFT_eSPI.h`)
 * Supports MEMPROG chunked loading to avoid heap usage for scripts

### Example

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <GlyCore.h>
#include <GlyLua54.h>
#include <GlyEngine.h>
#include <GlyDisplayTFT.h>

#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  23
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_BL   4

const auto LuaCode = F(R"(
local App = {
  title = "Game",
  verison = "1.0.0",
}

function App.draw(std, props)
  std.draw.clear(std.color.green)
  std.draw.color(std.color.black)
  std.text.put(1, 1, "Hello!")
end

return App
)");

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
GlyCore engine(LuaCode, GlyEngine, &tft);

void setup() {
    Serial.begin(115200);

    engine.init(240, 135);
    engine.setFramerate(1);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
}
  
void loop() {
  if (engine.hasErrors()) {
    Serial.println(engine.getErrors());
    for(;;);
  }
  engine.update();
}
```