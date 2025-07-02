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
    void update();
    void setFramerate(uint8_t);
    void setBtnDebounce(uint8_t);
    void setBtnKeyboard(uint8_t, uint8_t, const char *const);
    //lua_State *const getLua();

private:
    //lua_State *L;
};

#endif
