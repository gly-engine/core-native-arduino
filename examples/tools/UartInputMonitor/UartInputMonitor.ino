#include <HardwareSerial.h>
#include <GlyInputUart.h>

#define RXD1 17

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

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD1, -1);
  buttons.setTimeout(800);
}

void loop() {
  buttons.update();
  for (auto i = 0; i < buttons.getCount(); i++) {
    if (buttons.isUpdated(i)) {
        auto name = buttons.getName(i);
        auto press = buttons.isPressed(i);
        Serial.print(press ? '+' : '-');
        Serial.println(name);
    }
  }
}
