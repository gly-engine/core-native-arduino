# Core Native Arduino

<img src="https://raw.githubusercontent.com/RodrigoDornelles/RodrigoDornelles/refs/heads/master/media/gly-pong-esp32.gif" alt="GlyEngine pong sample on esp32" align="right" width="30%"/>

> Embed games and applications made in gly engine on your esp32, esp8266, raspbarry... and other devices using arduino ecosystem.

### Features

 * Supports Lua 5.1 and 5.4 _(include `GlyLua51.h` or `GlyLua54.h`)_
 * Supports 60+ FPS _(depends on display capabilities and communication speed)_
 * Supports third-party graphics libraries _(`Adafruit_GFX.h` or `TFT_eSPI.h`)_
 * Supports MEMPROG chunked loading to avoid heap usage for scripts

## Install

To install the Gly Core Native for Arduino, open the Arduino IDE, go to **Sketch > Include Library > Manage Libraries**, then search for **"GlyEngine"** in the Library Manager. Click Install next to the library name.

<div align="center"><img src="https://raw.githubusercontent.com/gly-engine/core-native-arduino/refs/heads/main/.github/install.png" alt="how to install GlyEngine on Arduino IDE"/></div>

## Example

GlyEngine Arduino Core is highly customizable in C++, letting you adjust framerate, hardware acceleration, video drivers, and input debounce. It’s versatile for desktop, web, and adaptable to embedded devices. See other examples for more options.

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
  version = "1.0.0",
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
