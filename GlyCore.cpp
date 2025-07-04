#include "GlyCore.h"

#include <math.h>
#include <Arduino.h>

#define GLY_HOOK_IMPL
#include "hooks.cpp"

void GlyCore::init(uint16_t width, uint16_t height)
{
    if (started) return;

    static const luaL_Reg glylibs[] = {
        {"native_draw_start", [](lua_State *L) {
            native_draw_start();
            lua_settop(L, 0);
            return 0;
        }},
        {"native_draw_flush", [](lua_State *L) {
            native_draw_flush();
            lua_settop(L, 0);
            return 0;
        }},
        {"native_draw_clear", [](lua_State *L) {
            native_draw_clear(luaL_checkinteger(L, 1));
            lua_settop(L, 0);
            return 0;
        }},
        {"native_draw_color", [](lua_State *L) {
            native_draw_color(luaL_checkinteger(L, 1));
            lua_settop(L, 0);
            return 0;
        }},
        {"native_draw_rect", [](lua_State *L) {
            native_draw_rect(
                luaL_checkinteger(L, 1),
                luaL_checknumber(L, 2),
                luaL_checknumber(L, 3),
                luaL_checknumber(L, 4),
                luaL_checknumber(L, 5)
            );
            lua_settop(L, 0);
            return 0;
        }},
        {"native_draw_line", [](lua_State *L) {
            native_draw_line(
                luaL_checknumber(L, 1),
                luaL_checknumber(L, 2),
                luaL_checknumber(L, 3),
                luaL_checknumber(L, 4)
            );
            lua_settop(L, 0);
            return 0;
        }},
        {"native_text_print", [](lua_State *L) {
            native_text_print(
                luaL_checknumber(L, 1),
                luaL_checknumber(L, 2),
                luaL_checkstring(L, 3)
            );
            lua_settop(L, 0);
            return 0;
        }},
        {"native_text_mensure", [](lua_State *L) {
            int16_t w = 1, h = 1;
            native_text_mensure(luaL_checkstring(L, 1), &w, &h);
            lua_settop(L, 0);
            lua_pushnumber(L, w);
            lua_pushnumber(L, h);
            return 2;
        }},
        {"native_text_font_size", [](lua_State *L) {
            native_text_font_size(floorf(luaL_checknumber(L, 1)));
            lua_settop(L, 0);
            return 0;
        }},
        {"native_text_font_name", [](lua_State *L) {
            native_text_font_name(luaL_checkstring(L, 1));
            lua_settop(L, 0);
            return 0;
        }},
        {"native_text_font_default", [](lua_State *L) {
            native_text_font_default(luaL_checkinteger(L, 1));
            lua_settop(L, 0);
            return 0;
        }},
        {"native_text_font_previous", [](lua_State *L) {
            native_text_font_previous();
            lua_settop(L, 0);
            return 0;
        }},
        {NULL, NULL}
    };

    gly_hook_display_init(width, height);

    L = luaL_newstate();
    luaL_openlibs(L);

    for (const auto& fn : glylibs) {
        if (!fn.name) break;
        lua_register(L, fn.name, fn.func);
    }

    do {
        size_t game_len = 0;
        size_t engine_len = 0;

        if (engine_storage) {
            engine_len = strlen_P(reinterpret_cast<PGM_P>(engine_storage));
            engine_code = new char[engine_len];
        } else {
            engine_len = strlen(engine_code);
        }

        if (engine_code == nullptr || engine_len == 0) {
            errors += "empty engine";
            break;
        }

        if (engine_storage) {
            strncpy_P(engine_code, reinterpret_cast<PGM_P>(engine_storage), engine_len);
            engine_code[engine_len] = '\0';
        }

        if(luaL_loadbuffer(L, engine_code, engine_len, "E") != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        if(engine_storage) {
            delete[] engine_code;
        }

        if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        lua_getglobal(L, "native_callback_init");
        lua_pushnumber(L, width);
        lua_pushnumber(L, height);

        if (game_storage) {
            game_len = strlen_P(reinterpret_cast<PGM_P>(game_storage));
            game_code = new game_code[game_len];
        } else {
            game_len = strlen(game_code);
        }

        if (game_code == nullptr || game_len == 0) {
            errors += "empty game";
            break;
        }

        if (game_storage) {
            strncpy_P(game_code, reinterpret_cast<PGM_P>(game_storage), game_len);
            game_code[game_len] = '\0';
        }

        if(luaL_loadbuffer(L, game_code, game_len, "G") != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        if(game_storage) {
            delete[] game_code;
        }

        if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        if(lua_pcall(L, 3, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        lua_getglobal(L, "native_callback_loop");
        ref_native_callback_loop = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_getglobal(L, "native_callback_draw");
        ref_native_callback_draw = luaL_ref(L, LUA_REGISTRYINDEX);
        
        lua_getglobal(L, "native_callback_keyboard");
        ref_native_callback_keyboard = luaL_ref(L, LUA_REGISTRYINDEX);

        started = true;
    }
    while(0);
}

bool GlyCore::update()
{
    if(!started) return false;

    unsigned long now = micros();
    unsigned long delta = now - time_last_frame;

    for (uint8_t i = 0; i < numButtons; ++i) {
        ButtonInfo& btn = buttons[i];
        bool state = (digitalRead(btn.pin) == (btn.activeLow ? LOW : HIGH));
        if (state != btn.lastState && (now - btn.lastChange >= time_debounce)) {
            btn.lastState = state;
            btn.lastChange = now;
            if (ref_native_callback_keyboard != 0) {
                lua_rawgeti(L, LUA_REGISTRYINDEX, ref_native_callback_keyboard);
                lua_pushstring(L, btn.name);
                lua_pushboolean(L, state);
                if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
                    errors = lua_tostring(L, -1);
                    lua_pop(L, 1);
                }
            }
        }
    }

    if (delta >= time_frame) {
        count_frame++;
        time_delta = delta;
        time_last_frame = now;

        lua_rawgeti(L, LUA_REGISTRYINDEX, ref_native_callback_loop);
        lua_pushnumber(L, delta/1000);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
        }
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref_native_callback_draw);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
        }
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

bool GlyCore::hasErrors() const {
    return !errors.isEmpty();
}
const String& GlyCore::getErrors() const {
    return errors;
}
void GlyCore::clearErrors() {
    errors = "";
}

void GlyCore::setBtnDebounce(uint8_t ms)
{
    time_debounce = static_cast<uint32_t>(ms) * 1000;
}

void GlyCore::setBtnKeyboard(uint8_t pin, uint8_t pinModeType, const char *const name)
{
    if (numButtons >= MAX_BUTTONS) {
        errors += "maximum number of buttons exceeded";
        return;
    }

    pinMode(pin, pinModeType);
    bool activeLow = (pinModeType == INPUT_PULLUP);

    buttons[numButtons++] = {
        pin,
        activeLow,
        name,
        digitalRead(pin) == (activeLow ? LOW : HIGH),
        micros()
    };
}
