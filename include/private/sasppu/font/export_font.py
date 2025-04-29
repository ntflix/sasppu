#!/bin/python3

import fontforge
from PIL import Image
from io import BytesIO

def compress_image(data, bitdepth):
    rolling_result = 0
    output = []
    for bits in data:
        bits &= (1 << bitdepth) - 1
        if bits < rolling_result:
            next = bits + (1 << bitdepth) - rolling_result
        else:
            next = bits - rolling_result
        output.append(next)
        rolling_result += bits
        rolling_result &= (1 << bitdepth) - 1
    return output

def encode_image(data, bitdepth):
    compressed = []
    temp = 0
    bits_encoded = 0
    for bits in data:
        bits &= (1 << bitdepth) - 1
        temp |= (bits << bits_encoded)
        bits_encoded += bitdepth
        if bits_encoded == 8:
            compressed.append(temp)
            temp = 0
            bits_encoded = 0
    if bits_encoded > 0:
        compressed.append(temp)
    return bytes(compressed)

def decode_image(data, bitdepth):
    decompressed = []
    bits_encoded = 0
    for bits in data:
        bits_encoded = 8
        while bits_encoded > 0:
            decompressed.append(bits & ((1 << bitdepth) - 1))
            bits >>= bitdepth
            bits_encoded -= bitdepth
    return decompressed

font = fontforge.open("at01.ttf")
encoded_stream = bytes()
metadata = []
for code in range(0x20, 0x7F):
    glyph_found = False
    for name in font:
        glyph = font[name]
        if glyph.unicode == code:
            print("{" + chr(code) + "}:")
            w = int(glyph.width / 64) + 1
            h = int(glyph.vwidth / 64) + 1
            print("  width:", w)
            print("  vwidth:", h)
            filename = "export/" + name + ".png"
            glyph.export(filename, 16)
            with open(filename, 'rb') as f:
                data = f.read()

                # Load image from BytesIO
                im = Image.open(BytesIO(data))
                byte_data = [0 if pix == 0 else 1 for pix in list(im.getdata())]
                encoded = encode_image(byte_data, 1)
                decoded = decode_image(encoded, 1)

                #print(list(im.getdata()))
                #print(byte_data)
                #print(decoded[:(w*h)])

                assert byte_data == decoded[:(w*h)]

                metadata.append((w, h, len(encoded_stream)))
                encoded_stream += encoded
            glyph_found = True
            break
    if not glyph_found:
        raise RuntimeError(chr(code))
with open("encoded.bin", 'wb') as f:
    f.write(encoded_stream)
with open("metadata.h", 'w') as f:
    print("""
#pragma once
#include <stdint.h>
typedef struct
{
    uint8_t width;
    uint8_t height;
    size_t offset;
} CharacterData;""", file=f)
    print("static const CharacterData CHARACTER_DATA[] = {", file=f)
    for item in metadata:
        print("{.width=" + str(item[0]) + ",.height=" + str(item[1]) + ",.offset=" + str(item[2]) + "},", file=f)
    print("};", file=f)
