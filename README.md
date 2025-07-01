# Core Native Arduino

> Embed games and applications made in gly engine on your esp32, esp8266, raspbary.. and other devices using arduino ecosystem.

```cpp
#include <GlyCore.h>
#include <GlyEngine.h>
#include <GlyST7789.h>
#include <GlySamplePong.h>

GlyCore engine;

void setup() {
    engine.init(135, 240, GlySamplePong);
    engine.setBtnKey(0, "up");
    engine.setBtnKey(35, "down");
}

void loop() {
    engine.update();
}
```
