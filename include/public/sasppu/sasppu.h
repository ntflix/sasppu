/**
 * @file sasppu.h
 * @author John Hunter <moliveofscratch@gmail.com>
 * @brief The main include file for SASPPU.
 * @version 0.1
 * @date 2025-04-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SASPPU_SASPPU_H_
#define SASPPU_SASPPU_H_

#include "stdint.h"
#include "stdbool.h"

#define SASPPU_VERSION_MAJOR 1
#define SASPPU_VERSION_MINOR 1
#define SASPPU_VERSION_PATCH 1

// #define SASPPU_VERSION "SASPPU_VERSION_MAJOR.SASPPU_VERSION_MINOR.SASPPU_VERSION_PATCH"

#define BG_WIDTH_POWER (8)
#define BG_HEIGHT_POWER (8)
#define BG_WIDTH (1 << BG_WIDTH_POWER)
#define BG_HEIGHT (1 << BG_HEIGHT_POWER)

#define SPRITE_COUNT (256)
#define SPRITE_CACHE (16)

#define SPR_WIDTH_POWER (8)
#define SPR_HEIGHT_POWER (8)
#define SPR_WIDTH (1 << SPR_WIDTH_POWER)
#define SPR_HEIGHT (1 << SPR_HEIGHT_POWER)

#define MAP_WIDTH_POWER (6)
#define MAP_HEIGHT_POWER (6)
#define MAP_WIDTH (1 << MAP_WIDTH_POWER)
#define MAP_HEIGHT (1 << MAP_HEIGHT_POWER)

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t width;
    uint8_t height;
    uint8_t graphics_x;
    uint8_t graphics_y;
    uint8_t windows;
    uint8_t flags;
} Sprite;

#define SPR_ENABLED (1 << 0)
#define SPR_PRIORITY (1 << 1)
#define SPR_FLIP_X (1 << 2)
#define SPR_FLIP_Y (1 << 3)
#define SPR_C_MATH (1 << 4)
#define SPR_DOUBLE (1 << 5)

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t windows;
    uint8_t flags;
} Background;

#define BG_C_MATH (1 << 0)

typedef struct
{
    uint16_t mainscreen_colour;
    uint16_t subscreen_colour;
    int16_t window_1_left;
    int16_t window_1_right;
    int16_t window_2_left;
    int16_t window_2_right;
    uint8_t bgcol_windows;
    uint8_t flags;
} MainState;

#define CMATH_HALF_MAIN_SCREEN (1 << 0)
#define CMATH_DOUBLE_MAIN_SCREEN (1 << 1)
#define CMATH_HALF_SUB_SCREEN (1 << 2)
#define CMATH_DOUBLE_SUB_SCREEN (1 << 3)
#define CMATH_ADD_SUB_SCREEN (1 << 4)
#define CMATH_SUB_SUB_SCREEN (1 << 5)
#define CMATH_FADE_ENABLE (1 << 6)
#define CMATH_CMATH_ENABLE (1 << 7)

typedef struct
{
    uint16_t screen_fade;
    uint8_t flags;
} CMathState;

#define MAIN_SPR0_ENABLE (1 << 0)
#define MAIN_SPR1_ENABLE (1 << 1)
#define MAIN_BG0_ENABLE (1 << 2)
#define MAIN_BG1_ENABLE (1 << 3)
#define MAIN_CMATH_ENABLE (1 << 4)
#define MAIN_BGCOL_WINDOW_ENABLE (1 << 5)

#define WINDOW_A (0b0001)
#define WINDOW_B (0b0010)
#define WINDOW_AB (0b0100)
#define WINDOW_X (0b1000)
#define WINDOW_ALL (0b1111)

typedef uint16_t uint16x8_t __attribute__((vector_size(16)));
typedef int16_t int16x8_t __attribute__((vector_size(16)));
typedef uint16_t mask16x8_t __attribute__((vector_size(16)));

extern MainState SASPPU_main_state;
extern Background SASPPU_bg0_state;
extern Background SASPPU_bg1_state;
extern CMathState SASPPU_cmath_state;
extern uint8_t SASPPU_hdma_enable;

extern Sprite SASPPU_oam[SPRITE_COUNT];
extern uint16_t SASPPU_bg0[MAP_WIDTH * MAP_HEIGHT];
extern uint16_t SASPPU_bg1[MAP_WIDTH * MAP_HEIGHT];

extern uint16x8_t SASPPU_background[BG_WIDTH * BG_HEIGHT / 8];
extern uint16x8_t SASPPU_sprites[SPR_WIDTH * SPR_HEIGHT / 8];

extern Sprite *SASPPU_sprite_cache[2][SPRITE_CACHE];

#if __STDC_VERSION__ >= 202000
typedef enum : uint8_t
#else
typedef enum
#endif
{
    HDMA_NOOP = 0,
    HDMA_WRITE_MAIN_STATE,
    HDMA_WRITE_BG0_STATE,
    HDMA_WRITE_BG1_STATE,
    HDMA_WRITE_CMATH_STATE,
    HDMA_WRITE_OAM,
    HDMA_DISABLE,
} HDMACommand;

typedef union
{
    MainState main;
    Background background;
    CMathState cmath;
    Sprite oam;
} HDMAData;

typedef struct
{
    HDMACommand command;
    uint8_t oam_index;
    HDMAData data;
} HDMAEntry;

#define SASPPU_HDMA_TABLE_COUNT (8)

extern HDMAEntry SASPPU_hdma_tables[SASPPU_HDMA_TABLE_COUNT][240];

void SASPPU_render(uint16x8_t *fb, uint8_t section);

#endif // SASPPU_SASPPU_H_
