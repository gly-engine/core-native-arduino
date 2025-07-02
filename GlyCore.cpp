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

void GlyCore::update()
{

}

void GlyCore::setBtnKeyboard(uint8_t, uint8_t, const char *const)
{

}
