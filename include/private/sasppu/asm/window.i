.ifndef SASPPU_WINDOW_MACROS
.set SASPPU_WINDOW_MACROS, 1

.set WM_NOT_TOUCHED_1, q0
.set WM_NOT_TOUCHED_2, q1
.set WM_NOT_TOUCHED_3, q3

.set WM_COLOUR_IN, q2

.set WM_MAINOUT, q4
.set WM_SUBOUT, q5
.set WM_WINDOW_1_MASK, q4
.set WM_WINDOW_2_MASK, q5
.set WM_OUTPUT_TRUE, q6
.set WM_OUTPUT_FALSE, q7

.macro window_macro_load_w1 window_cache_reg
  ld.qr q4, \window_cache_reg, 0
.endm
.macro window_macro_load_w2 window_cache_reg
  ld.qr q5, \window_cache_reg, 16
.endm
.macro window_macro_load_mainout maincol_reg
  ld.qr q4, \maincol_reg, 0
.endm
.macro window_macro_load_subout subcol_reg
  ld.qr q5, \subcol_reg, 0
.endm
.macro window_macro_store_mainout maincol_reg
  st.qr q4, \maincol_reg, 0
.endm
.macro window_macro_store_subout subcol_reg
  st.qr q5, \subcol_reg, 0
.endm

// WM_OUTPUT_TRUE is the result & !WM_OUTPUT_FALSE
// WM_OUTPUT_FALSE is !WM_OUTPUT_TRUE
.macro window_macro_q wincase, qreg, window_cache_reg
.if \wincase == 15
  ee.notq q6, \qreg
.if \qreg != q7
  ee.notq q7, q6
.endif
.endif
.if \wincase == 14
  window_macro_load_w2 \window_cache_reg
  ee.notq q6, q5
  window_macro_load_w1 \window_cache_reg
  ee.andq q6, q6, q4
  ee.orq q7, q6, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 13
  window_macro_load_w1 \window_cache_reg
  ee.notq q6, q4
  window_macro_load_w2 \window_cache_reg
  ee.andq q6, q6, q5
  ee.orq q7, q6, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 12
  window_macro_load_w1 \window_cache_reg
  window_macro_load_w2 \window_cache_reg
  ee.xorq q6, q4, q5
  ee.orq q7, q6, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 11
  window_macro_load_w1 \window_cache_reg
  window_macro_load_w2 \window_cache_reg
  ee.andq q6, q4, q5
  ee.orq q7, q6, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 10
  window_macro_load_w1 \window_cache_reg
  ee.orq q7, q4, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 9
  window_macro_load_w2 \window_cache_reg
  ee.orq q7, q5, \qreg
  ee.notq q6, q7
.endif
.if \wincase == 8
  window_macro_load_w1 \window_cache_reg
  ee.orq q7, \qreg, q4
  window_macro_load_w2 \window_cache_reg
  ee.orq q7, q5, q7
  ee.notq q6, q7
.endif
.if \wincase == 7
  window_macro_load_w1 \window_cache_reg
  ee.notq q7, \qreg
  window_macro_load_w2 \window_cache_reg
  ee.orq q6, q4, q5
  ee.andq q6, q6, q7
  ee.notq q7, q6
.endif
.if \wincase == 6
  window_macro_load_w2 \window_cache_reg
  ee.notq q7, \qreg
  ee.andq q6, q5, q7
  ee.notq q7, q6
.endif
.if \wincase == 5
  window_macro_load_w1 \window_cache_reg
  ee.notq q7, \qreg
  ee.andq q6, q4, q7
  ee.notq q7, q6
.endif
.if \wincase == 4
  window_macro_load_w1 \window_cache_reg
  window_macro_load_w2 \window_cache_reg
  ee.andq q6, q5, q4
  ee.notq q7, \qreg
  ee.andq q6, q6, q7
  ee.notq q7, q6
.endif
.if \wincase == 3
  window_macro_load_w1 \window_cache_reg
  window_macro_load_w2 \window_cache_reg
  ee.xorq q6, q4, q5
  ee.notq q7, \qreg
  ee.andq q6, q6, q7
  ee.notq q7, q6
.endif
.if \wincase == 2
  window_macro_load_w1 \window_cache_reg
  ee.orq q7, \qreg, q4
  window_macro_load_w2 \window_cache_reg
  ee.notq q6, q7
  ee.andq q6, q6, q5
  ee.notq q7, q6
.endif
.if \wincase == 1
  window_macro_load_w2 \window_cache_reg
  ee.orq q7, \qreg, q5
  window_macro_load_w1 \window_cache_reg
  ee.notq q6, q7
  ee.andq q6, q6, q4
  ee.notq q7, q6
.endif
.endm

// both window settings are the same.
.macro window_macro_both wincase, window_cache_reg, maincol_reg, subcol_reg
.if \wincase == 0
.exitm
.endif

  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q2, q7
  window_macro_q \wincase, q7, \window_cache_reg
  window_macro_load_mainout \maincol_reg
  ee.andq q4, q4, q7
  window_macro_load_subout \subcol_reg
  ee.andq q5, q5, q7
  ee.andq q2, q2, q6
  ee.orq q4, q4, q2
  window_macro_store_mainout \maincol_reg
  ee.orq q5, q5, q2
  window_macro_store_subout \subcol_reg
.endm

// Calculates the windows
.macro window_macro mainwincase, subwincase, window_cache_reg, maincol_reg, subcol_reg
.if \mainwincase == \subwincase
  window_macro_both \mainwincase, \window_cache_reg, \maincol_reg, \subcol_reg
.else

.if \subwincase == 0
  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q2, q7
  window_macro_q \mainwincase, q7, \window_cache_reg
  window_macro_load_mainout \maincol_reg
  ee.andq q4, q4, q7
  ee.andq q2, q2, q6
  ee.orq q4, q4, q2
  window_macro_store_mainout \maincol_reg
.else
.if \mainwincase == 0
  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q2, q7
  window_macro_q \subwincase, q7, \window_cache_reg
  window_macro_load_subout \subcol_reg
  ee.andq q5, q5, q7
  ee.andq q2, q2, q6
  ee.orq q5, q5, q2
  window_macro_store_subout \subcol_reg
.else
  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q2, q7
  window_macro_q \mainwincase, q7, \window_cache_reg
  window_macro_load_mainout \maincol_reg
  ee.andq q4, q4, q7
  ee.andq q7, q2, q6
  ee.orq q4, q4, q7
  window_macro_store_mainout \maincol_reg

  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q2, q7
  window_macro_q \subwincase, q7, \window_cache_reg
  window_macro_load_subout \subcol_reg
  ee.andq q5, q5, q7
  ee.andq q7, q2, q6
  ee.orq q5, q5, q7
  window_macro_store_subout \subcol_reg
.endif
.endif

.endif
.endm

.endif // SASPPU_WINDOW_MACROS
