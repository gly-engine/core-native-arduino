#include "GlyCore.h"

#include <math.h>
#include <Arduino.h>

extern "C" {
#include "vendor/lua54/lua.h"
#include "vendor/lua54/lualib.h"
#include "vendor/lua54/lauxlib.h"
int lua_iversion(lua_State* L);
}

#define GLY_HOOK_IMPL
#include "hooks.cpp"


static char lua_chunk_buffer[128];

static const char* progmemReader(lua_State*, void* data, size_t* size) {
  auto ptr = static_cast<const char**>(data);
  size_t i = 0;

  while (i < sizeof(lua_chunk_buffer)) {
    char c = pgm_read_byte(*ptr);
    if (c == '\0') break;
    lua_chunk_buffer[i++] = c;
    (*ptr)++;
  }

  *size = i;
  return (i > 0) ? lua_chunk_buffer : nullptr;
}

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
    Lver = lua_version(L);
    L_REGISTRYINDEX = Lver == 504? LUA_REGISTRYINDEX: (-10000);
    luaL_openlibs(L);

    for (const auto& fn : glylibs) {
        if (!fn.name) break;
        lua_register(L, fn.name, fn.func);
    }

    do {
        if (engine_storage == nullptr) {
            size_t engine_len = strlen(engine_code);
            if(luaL_loadbuffer(L, engine_code, engine_len, "E") != LUA_OK) {
                errors += luaL_checkstring(L, -1);
                break;
            }
        } else {
            const char* ctx = reinterpret_cast<const char*>(engine_storage);
            if(lua_load(L, progmemReader, &ctx, "E", "t") != LUA_OK){
                errors += luaL_checkstring(L, -1);
                break;
            }
        }

        if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        if (lua_getglobal(L, "native_callback_init") != LUA_TFUNCTION) {
            errors += "native_callback_init";
            break;
        }
        lua_pushnumber(L, width);
        lua_pushnumber(L, height);

        if (game_storage == nullptr) {
            size_t game_len = strlen(game_code);
            if(luaL_loadbuffer(L, game_code, game_len, "E") != LUA_OK) {
                errors += luaL_checkstring(L, -1);
                break;
            }
        } else {
            const char* ctx = reinterpret_cast<const char*>(game_storage);
            if(lua_load(L, progmemReader, &ctx, "E", "t") != LUA_OK){
                errors += luaL_checkstring(L, -1);
                break;
            }
        }

        if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }

        if(lua_pcall(L, 3, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
            break;
        }
        
        if (lua_getglobal(L, "native_callback_loop") != LUA_TFUNCTION) {
            errors += "native_callback_loop";
            break;
        }
        ref_native_callback_loop = luaL_ref(L, L_REGISTRYINDEX);

        if (lua_getglobal(L, "native_callback_draw") != LUA_TFUNCTION) {
            errors += "native_callback_draw";
            break;
        }
        ref_native_callback_draw = luaL_ref(L, L_REGISTRYINDEX);
        
        if (lua_getglobal(L, "native_callback_keyboard") != LUA_TFUNCTION) {
            errors += "native_callback_keyboard";
            break;
        }
        ref_native_callback_keyboard = luaL_ref(L, L_REGISTRYINDEX);

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
                lua_rawgeti(L, L_REGISTRYINDEX, ref_native_callback_keyboard);
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

        lua_rawgeti(L, L_REGISTRYINDEX, ref_native_callback_loop);
        lua_pushnumber(L, delta/1000);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            errors += luaL_checkstring(L, -1);
        }
    
        lua_rawgeti(L, L_REGISTRYINDEX, ref_native_callback_draw);
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

uint32_t GlyCore::getLuaMemTotal() {
    if (!L) return 0;

    int kb = lua_gc(L, LUA_GCCOUNT, 0);
    int b  = lua_gc(L, LUA_GCCOUNTB, 0);

    return (kb * 1024) + b;
}

uint8_t GlyCore::getLuaMemPercentage() {
    uint32_t total = getLuaMemTotal();
    uint32_t heap = 0; 

#if defined(ARDUINO_ARCH_ESP32)
    heap = ESP.getHeapSize();
#elif defined(ARDUINO_ARCH_ESP8266)
    heap = 80000;
#elif defined(PICO_RP2040)
    heap = 200000;
#elif defined(PICO_RP2350)
    heap = 500000;
#endif
    
    if (heap == 0) return 0;
    return (total * 100) / heap;
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
