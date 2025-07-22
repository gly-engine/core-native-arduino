#ifndef H_GLY_TYPES
#define H_GLY_TYPES

#if defined(ARDUINO_ARCH_RP2040) || !defined(__cplusplus)
#include <stdint.h>
#else
#include <cstdint>
#endif

//! @cond
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
typedef union { 
    uint32_t value;
    struct { uint8_t a, b, g, r;};
} color_u;
#else
typedef union { 
    uint32_t value;
    struct { uint8_t r, g, b, a;};
} color_u;
#endif
//! @endcond 

using nullptr_­t = decltype(nullptr);
#define null nullptr

#endif
