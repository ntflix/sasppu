#!/bin/python3
import sys

def generate_window_jump_table(f):
    idents = []
    for window_sub in range(16):
        for window_main in range(16):
            ident = ("handle_window_" +
                    str(window_sub) + "_" +
                    str(window_main)) 

            idents.append(ident)

            f.write("#define IDENT " + str(ident) + "\n")
            f.write("#define LOGIC_MAIN " + str(window_main) + "\n")
            f.write("#define LOGIC_SUB " + str(window_sub) + "\n")
            f.write("#include \"sasppu/macimpl/window.h\"\n")

    f.write("const HandleWindowType HANDLE_WINDOW_LOOKUP[256] = {\n")
    for ident in idents:
        f.write(ident+",\n")
    f.write("};\n")

def generate_sprite_jump_table(f):
    idents = []
    for double in range(2):
        for c_math in range(2):
            for flip_y in range(2):
                for flip_x in range(2):
                    ident = ("handle_sprite_" +
                            str(double) + "_" +
                            str(c_math) + "_" +
                            str(flip_y) + "_" +
                            str(flip_x))

                    idents.append(ident)

                    f.write("#define IDENT " + str(ident) + "\n")
                    f.write("#define DOUBLE " + str(double) + "\n")
                    f.write("#define CMATH " + str(c_math) + "\n")
                    f.write("#define FLIP_Y " + str(flip_y) + "\n")
                    f.write("#define FLIP_X " + str(flip_x) + "\n")
                    f.write("#include \"sasppu/macimpl/sprite.h\"\n")

    f.write("const HandleSpriteType HANDLE_SPRITE_LOOKUP[16] = {\n")
    for ident in idents:
        f.write(ident+",\n")
    f.write("};\n")

def generate_cmath_jump_table(f):
    idents = []
    for cmath_enable in range(2):
        for fade_enable in range(2):
            for sub_ss in range(2):
                for add_ss in range(2):
                    for ss_double in range(2):
                        for ss_half in range(2):
                            for ms_double in range(2):
                                for ms_half in range(2):
                                    ident = ("handle_cmath_" +
                                            str(cmath_enable) + "_" +
                                            str(fade_enable) + "_" +
                                            str(sub_ss) + "_" +
                                            str(add_ss) + "_" +
                                            str(ss_double) + "_" +
                                            str(ss_half) + "_" +
                                            str(ms_double) + "_" +
                                            str(ms_half))

                                    idents.append(ident)

                                    f.write("#define IDENT " + str(ident) + "\n")
                                    f.write("#define CMATH_ENABLE " + str(cmath_enable) + "\n")
                                    f.write("#define FADE_ENABLE " + str(fade_enable) + "\n")
                                    f.write("#define SUB_SUB_SCREEN " + str(sub_ss) + "\n")
                                    f.write("#define ADD_SUB_SCREEN " + str(add_ss) + "\n")
                                    f.write("#define DOUBLE_SUB_SCREEN " + str(ss_double) + "\n")
                                    f.write("#define HALF_SUB_SCREEN " + str(ss_half) + "\n")
                                    f.write("#define DOUBLE_MAIN_SCREEN " + str(ms_double) + "\n")
                                    f.write("#define HALF_MAIN_SCREEN " + str(ms_half) + "\n")
                                    f.write("#include \"sasppu/macimpl/cmath.h\"\n")

    f.write("const HandleCMathType HANDLE_CMATH_LOOKUP[256] = {\n")
    for ident in idents:
        f.write(ident+",\n")
    f.write("};\n")

def generate_scanline_jump_table(f):
    idents = []
    for bgcol_window_enable in range(2):
        for cmath_enable in range(2):
            for bg1_enable in range(2):
                for bg0_enable in range(2):
                    for spr1_enable in range(2):
                        for spr0_enable in range(2):
                            ident = ("handle_scanline_" +
                                    str(bgcol_window_enable) + "_" +
                                    str(cmath_enable) + "_" +
                                    str(bg1_enable) + "_" +
                                    str(bg0_enable) + "_" +
                                    str(spr1_enable) + "_" +
                                    str(spr0_enable))

                            idents.append(ident)

                            f.write("#define IDENT " + str(ident) + "\n")
                            f.write("#define BGCOL_ENABLE " + str(bgcol_window_enable) + "\n")
                            f.write("#define CMATH_ENABLE " + str(cmath_enable) + "\n")
                            f.write("#define BG1_ENABLE " + str(bg1_enable) + "\n")
                            f.write("#define BG0_ENABLE " + str(bg0_enable) + "\n")
                            f.write("#define SPR1_ENABLE " + str(spr1_enable) + "\n")
                            f.write("#define SPR0_ENABLE " + str(spr0_enable) + "\n")
                            f.write("#include \"sasppu/macimpl/scanline.h\"\n")

    f.write("const HandleScanlineType HANDLE_SCANLINE_LOOKUP[64] = {\n")
    for ident in idents:
        f.write(ident+",\n")
    f.write("};\n")

def generate_background(f):
    f.write("#define IDENT handle_bg0\n")
    f.write("#define BG_INDEX 0\n")
    f.write("#include \"sasppu/macimpl/background.h\"\n")
    f.write("#define IDENT handle_bg1\n")
    f.write("#define BG_INDEX 1\n")
    f.write("#include \"sasppu/macimpl/background.h\"\n")

with open(sys.argv[1], "w") as f:
    generate_window_jump_table(f)
    generate_sprite_jump_table(f)
    generate_cmath_jump_table(f)
    generate_scanline_jump_table(f)
    generate_background(f)
