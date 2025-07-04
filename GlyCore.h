#ifndef H_GLYCORE
#define H_GLYCORE

#include <cstdint>
#include <WString.h>

#include "vendor/lua.h"
#include "hooks.cpp"

extern const char engine_lua[];
extern const unsigned int engine_lua_len;

class GlyCore {
public:
    GlyCore() {}
    template <typename T>
    GlyCore(T displayLib) {
        gly_hook_display_lib(displayLib);
    }
    void init(uint16_t, uint16_t, const char *const);
    bool update();
    void setFramerate(uint8_t);
    void setBtnDebounce(uint8_t);
    void setBtnKeyboard(uint8_t, uint8_t, const char *const);
    uint16_t getAvarageFPS();
    uint16_t getInstantFPS();
    bool hasErrors() const;
    const String& getErrors() const;
    void clearErrors();

private:
    // framerate control
    uint16_t fps = 0;
    uint16_t count_frame = 0;
    unsigned long time_delta = 0;
    unsigned long time_frame = 0;
    unsigned long time_last_frame = 0;
    unsigned long time_last_frame_1s = 0;
    // lua
    lua_State *L;
    String errors;
    int ref_native_callback_loop = 0;
    int ref_native_callback_draw = 0;
    int ref_native_callback_keyboard = 0;
    // buttons
    struct ButtonInfo {
        uint8_t pin;
        bool activeLow;
        const char* name;
        bool lastState;
        unsigned long lastChange;
    };
    static constexpr uint8_t MAX_BUTTONS = 12;
    ButtonInfo buttons[MAX_BUTTONS];
    uint8_t numButtons = 0;
    unsigned long time_debounce = 0;
};

#endif
