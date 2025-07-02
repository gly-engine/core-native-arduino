#include "GlyCore.h"
#define GLY_HOOK_IMPL
#include "hooks.cpp"

#include <Arduino.h>

void GlyCore::init(uint16_t width, uint16_t height, const char *const game_code)
{
    Serial.println("gly init start");
    gly_hook_display_init(width, height);
    //L = luaL_newstate();
    //luaL_openlibs(L);
    Serial.println("gly init end");
}

bool GlyCore::update()
{
    unsigned long now = micros();
    unsigned long delta = now - time_last_frame;

    if (delta >= time_frame) {
        count_frame++;
        time_delta = delta;
        time_last_frame = now;
    }

    if ((now - time_last_frame_1s) >= 1000000) {
        time_last_frame_1s = now;
        fps = count_frame;
        count_frame = 0;
    }

    return time_last_frame == now;
}

uint16_t GlyCore::getAvarageFPS()
{
    return fps;
}

uint16_t GlyCore::getInstantFPS()
{
    if (time_delta == 0) return 0;
    return 1000000 / time_delta;
}

void GlyCore::setFramerate(uint8_t frames_per_second)
{
    if (frames_per_second == 0) {
        time_frame = 0;
    } else { 
        time_frame = 1000000 / frames_per_second;
    }
}

void GlyCore::setBtnDebounce(uint8_t miliseconds)
{
    time_debounce = miliseconds;
}

void GlyCore::setBtnKeyboard(uint8_t, uint8_t, const char *const)
{

}
