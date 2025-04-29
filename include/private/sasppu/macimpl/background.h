#include "sasppu/sasppu.h"
#include "sasppu/internal.h"

#include "assert.h"

// SASPPU_HANDLE_BACKGROUND(IDENT, BG_INDEX)
#ifndef IDENT
#define IDENT handle_background_0
#endif

#ifndef BG_INDEX
#define BG_INDEX 0
#endif

inline void IDENT(uint16x8_t *const scanline, int16_t y)
{
#if BG_INDEX
        Background *state = &SASPPU_bg1_state;
#else
        Background *state = &SASPPU_bg0_state;
#endif
        size_t y_pos = (((size_t)(y + state->y)) >> 3) & ((MAP_HEIGHT)-1);
        size_t x_pos = (((size_t)(240 - 8 + state->x)) >> 3) & ((MAP_WIDTH)-1);
        size_t offset_x = ((size_t)(state->x) & 0x7);
        size_t offset_y = ((size_t)(y + state->y) & 0x7);

#if USE_INLINE_ASM
        asm volatile inline("wur.sar_byte %[offset_x]" : : [offset_x] "r"(offset_x << 1));
#endif

#if BG_INDEX
        uint16_t bg_map = SASPPU_bg1[y_pos * MAP_WIDTH + x_pos];
#else
        uint16_t bg_map = SASPPU_bg0[y_pos * MAP_WIDTH + x_pos];
#endif

        uint16x8_t *bg_1_p;

        if ((bg_map & 0b10) > 0)
        {
                bg_1_p = &SASPPU_background[(size_t)(bg_map >> 3) + ((7 - offset_y) * (BG_WIDTH >> 3))];
        }
        else
        {
                bg_1_p = &SASPPU_background[(size_t)(bg_map >> 3) + (offset_y * (BG_WIDTH >> 3))];
        };

#if USE_GCC_SIMD
        uint16x8_t bg_1;
        uint16x8_t bg_2;
        uint16x8_t bg;
#endif

        if ((bg_map & 0b01) > 0)
        {
#if USE_INLINE_ASM
                asm volatile inline("                      \n\t \
            ld.qr q1, %[bg_1_p], 0 \n\t \
            ee.vzip.16 q0, q1      \n\t \
            ee.vzip.16 q1, q0      \n\t \
            ee.vzip.16 q0, q1      \n\t \
            ee.vzip.16 q1, q0      \n\t \
            " : : [bg_1_p] "r"(bg_1_p));
#endif
#if USE_GCC_SIMD
                bg_1 = SHUFFLE_1(*bg_1_p, REVERSE_MASK);
#endif
#if VERIFY_INLINE_ASM
                CHECK_SIMD_Q0(bg_1);
#endif
        }
        else
        {
#if USE_INLINE_ASM
                asm volatile inline("ld.qr q0, %[bg_1_p], 0" : : [bg_1_p] "r"(bg_1_p));
#endif
#if USE_GCC_SIMD
                bg_1 = *bg_1_p;
#endif
#if VERIFY_INLINE_ASM
                CHECK_SIMD_Q0(bg_1);
#endif
        }

        ssize_t x = (240 / 8) - 1;
        do
        {
#if USE_INLINE_ASM
                asm volatile inline("mv.qr q1, q0");
#endif
#if USE_GCC_SIMD
                bg_2 = bg_1;
#endif
#if VERIFY_INLINE_ASM
                CHECK_SIMD_Q1(bg_2);
#endif
                x_pos = (x_pos - 1) & ((MAP_WIDTH)-1);

#if BG_INDEX
                bg_map = SASPPU_bg1[y_pos * MAP_WIDTH + x_pos];
#else
                bg_map = SASPPU_bg0[y_pos * MAP_WIDTH + x_pos];
#endif

                if ((bg_map & 0b10) > 0)
                {
                        bg_1_p = &SASPPU_background[(size_t)(bg_map >> 3) + ((7 - offset_y) * (BG_WIDTH >> 3))];
                }
                else
                {
                        bg_1_p = &SASPPU_background[(size_t)(bg_map >> 3) + (offset_y * (BG_WIDTH >> 3))];
                };

                if ((bg_map & 0b01) > 0)
                {
#if USE_INLINE_ASM
                        asm volatile inline("                      \n\t \
                ld.qr q2, %[bg_1_p], 0 \n\t \
                ee.vzip.16 q0, q2      \n\t \
                ee.vzip.16 q2, q0      \n\t \
                ee.vzip.16 q0, q2      \n\t \
                ee.vzip.16 q2, q0      \n\t \
                " : : [bg_1_p] "r"(bg_1_p));
#endif
#if USE_GCC_SIMD
                        bg_1 = SHUFFLE_1(*bg_1_p, REVERSE_MASK);
#endif
#if VERIFY_INLINE_ASM
                        CHECK_SIMD_Q0(bg_1);
#endif
                }
                else
                {
#if USE_INLINE_ASM
                        asm volatile inline("ld.qr q0, %[bg_1_p], 0" : : [bg_1_p] "r"(bg_1_p));
#endif
#if USE_GCC_SIMD
                        bg_1 = *bg_1_p;
#endif
#if VERIFY_INLINE_ASM
                        CHECK_SIMD_Q0(bg_1);
#endif
                }

#if USE_INLINE_ASM
                asm volatile inline("                                           \n\t \
            mv.qr q2, q0                                \n\t \
            ee.src.q.ld.ip q3, %[cmath_bit], 0, q2, q1     \n\t \
            " : : [cmath_bit] "r"(&CMATH_BIT));
#endif
#if USE_GCC_SIMD
                bg = SHUFFLE_2(bg_1, bg_2, VECTOR_SHUFFLES[offset_x]);
#endif
#if VERIFY_INLINE_ASM
                CHECK_SIMD_Q2(bg);
#endif

#if BG_INDEX
                if ((SASPPU_bg1_state.flags & BG_C_MATH) > 0)
#else
                if ((SASPPU_bg0_state.flags & BG_C_MATH) > 0)
#endif
                {
#if USE_INLINE_ASM
                        asm volatile inline("ee.orq q2, q2, q3");
#endif
#if USE_GCC_SIMD
                        bg |= CMATH_BIT;
#endif
#if VERIFY_INLINE_ASM
                        CHECK_SIMD_Q2(bg);
#endif
                }

#if BG_INDEX
                HANDLE_WINDOW_LOOKUP[SASPPU_bg1_state.windows]
#else
                HANDLE_WINDOW_LOOKUP[SASPPU_bg0_state.windows]
#endif
#if USE_INLINE_ASM
                    (scanline, x);
#else
                    (scanline, x, bg);
#endif
        } while ((--x) >= 0);
}

#undef IDENT
#undef BG_INDEX
