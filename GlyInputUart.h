#ifndef GLYINPUTUART_H
#define GLYINPUTUART_H

#include <Arduino.h>
#include <vector>
#include <initializer_list>

class GlyInputBase {
public:
    virtual ~GlyInputBase() = default;
    virtual void update() = 0;
    virtual size_t getCount() const = 0;
    virtual const char* getName(size_t i) const = 0;
    virtual bool isUpdated(size_t i) const = 0;
    virtual bool isPressed(size_t i) const = 0;
};

//! @cond
namespace GlyInput {
    GlyInputBase* buttons = nullptr;
}
//! @endcond

void gly_hook_input_lib(GlyInputBase* lib) {
    GlyInput::buttons = lib;
}

void gly_hook_input_loop(void (*callback)(const char*, bool)) {
    auto* btns = GlyInput::buttons;
    if (!btns || !callback) return;
    btns->update();
    for (size_t i = 0; i < btns->getCount(); i++) {
        if (btns->isUpdated(i)) {
            callback(const_cast<char*>(btns->getName(i)), btns->isPressed(i));
        }
    }
}

template<typename TSerial>
class GlyInputUart: public GlyInputBase {
public:
    struct Button {
        const char* name;
        uint32_t code;
        bool pressed = false;
        bool updated = false;
        unsigned long last_pressed = 0;
    };

    GlyInputUart(TSerial* serial, std::initializer_list<Button> buttons) 
      : serial(serial), buttons(buttons.begin(), buttons.end()) {}

    void update() {
        unsigned long current_time = micros();
        while (serial->available()) {
            int read_byte = serial->read();
            if (read_byte != -1) {
                buffer_packet = (buffer_packet << 8) | (read_byte & 0xFF);
                buffer_packet &= 0xFFFFFF;
                if (buffer_count < 3) {
                    buffer_count++;
                }
            }
        }

        for (auto& button : buttons) {
            if (buffer_count >= 3 && buffer_packet == button.code) {
                button.last_pressed = current_time;
                button.updated = button.pressed != true;
                button.pressed = true;
                buffer_count = 0;
                continue;
            }
            
            bool button_timeout = (current_time - button.last_pressed) >= timeout;
            if (button.pressed && button_timeout) {
                button.updated = true;
                button.pressed = false;
                continue;
            }
            
            button.updated = false;
        }
    }

    void setTimeout(uint16_t ms) {
        timeout = static_cast<decltype(timeout)>(ms) * 1000;
    }

    size_t getCount() const { return buttons.size(); }
    
    const char* getName(size_t i) const { 
      return i < buttons.size() ? buttons[i].name : nullptr;
    }
    
    bool isUpdated(size_t i) const {
      return i < buttons.size() ? buttons[i].updated : false;
    }
    
    bool isPressed(size_t i) const {
      return i < buttons.size() ? buttons[i].pressed : false;
    }

private:
    uint8_t buffer_count = 0;
    uint32_t buffer_packet = 0;
    unsigned long timeout = 0;
    TSerial* serial;
    std::vector<Button> buttons;
};

#endif