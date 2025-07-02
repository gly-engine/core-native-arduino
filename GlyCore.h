#ifndef H_GLYCORE
#define H_GLYCORE

#include <cstdint>

//#include "vendor/lua.hpp"

#include "hooks.cpp"

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
    //lua_State *const getLua();

private:
    uint16_t fps = 0;
    uint16_t count_frame = 0;
    unsigned long time_debounce = 0;
    unsigned long time_delta = 0;
    unsigned long time_frame = 0;
    unsigned long time_last_frame = 0;
    unsigned long time_last_frame_1s = 0;

};

#endif
