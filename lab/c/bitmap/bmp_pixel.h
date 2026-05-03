/**
 * BITMAP
 * © 2025 0x4248
 */

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} PIXEL24;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} PIXEL32;

typedef struct {
    uint16_t value;
} PIXEL16;

#pragma pack(pop)
