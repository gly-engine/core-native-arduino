#ifndef H_GLYCORE
#define H_GLYCORE

#ifdef DOXYGEN
class GlyCore : public GlyDisplayTFT {};
class GlyCore { public: GlyLua51 lua_State; };
class GlyCore { public: GlyLua54 lua_State; };
#endif

#include <cstdint>
#include <WString.h>

#include "hooks.cpp"

typedef struct lua_State lua_State;

class GlyCore {
public:
    template<typename gameType, typename engineType>
    GlyCore(gameType game, engineType library) {
        loadEngine(library);
        loadGame(game);
    }
    template<typename gameType, typename engineType, typename displayType>
    GlyCore(gameType game, engineType library, displayType display) {
        gly_hook_display_lib(display);
        loadEngine(library);
        loadGame(game);
    }
    void init(uint16_t, uint16_t);
    bool update();
    void setFramerate(uint8_t);
    void setBtnDebounce(uint8_t);
    void setBtnKeyboard(uint8_t, uint8_t, const char *const);
    uint16_t getAvarageFPS();
    uint16_t getInstantFPS();
    uint32_t getLuaMemTotal();
    uint8_t getLuaMemPercentage();
    bool hasErrors() const;
    const String& getErrors() const;
    void clearErrors();

private:
    bool started = false;
    // load game control
    inline void loadEngine(const __FlashStringHelper* src) { engine_storage = src; }
    inline void loadEngine(const char* src) { engine_code = const_cast<char*>(src); }
    inline void loadEngine(char* src) { engine_code = (src); }
    inline void loadGame(const __FlashStringHelper* src) { game_storage = src; }
    inline void loadGame(const char* src) { game_code = const_cast<char*>(src); }
    inline void loadGame(char* src) { game_code = (src); }
    const __FlashStringHelper* engine_storage = nullptr;
    const __FlashStringHelper* game_storage = nullptr;
    char* engine_code = nullptr;
    char* game_code = nullptr;
    // framerate control
    uint16_t fps = 0;
    uint16_t count_frame = 0;
    unsigned long time_delta = 0;
    unsigned long time_frame = 0;
    unsigned long time_last_frame = 0;
    unsigned long time_last_frame_1s = 0;
    // lua
    lua_State *L;
    uint16_t Lver;
    int L_REGISTRYINDEX;
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
