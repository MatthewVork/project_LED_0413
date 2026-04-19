/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/Matthew/SquareLine/assets/NotoSansSC-VariableFont_wght.ttf -o C:/Users/Matthew/SquareLine/assets\ui_font_Font1.c --format lvgl -r 0x20-0x7f --symbols 红黄绿蓝色火焰流星关闭律动彩虹呼吸白光亮度连接网络断开蓝牙已经当前时间模式 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONT1
#define UI_FONT_FONT1 1
#endif

#if UI_FONT_FONT1

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0x90,

    /* U+0022 "\"" */
    0xb6, 0xd0,

    /* U+0023 "#" */
    0x24, 0x89, 0x27, 0xf4, 0x89, 0x12, 0x7e, 0x49,
    0x12, 0x24, 0x80,

    /* U+0024 "$" */
    0x20, 0x87, 0xa0, 0x82, 0x4, 0x8, 0x18, 0x20,
    0x41, 0xb, 0xe2, 0x8,

    /* U+0025 "%" */
    0x70, 0x44, 0x84, 0x22, 0x21, 0x12, 0x8, 0x92,
    0x49, 0x69, 0xd2, 0x20, 0x91, 0x8, 0x88, 0x44,
    0x44, 0x24, 0x20, 0xe0,

    /* U+0026 "&" */
    0x18, 0x12, 0x9, 0x5, 0x3, 0x1, 0x81, 0x43,
    0x12, 0x8d, 0x43, 0x21, 0x8f, 0x30,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x29, 0x49, 0x24, 0x92, 0x48, 0x91,

    /* U+0029 ")" */
    0x91, 0x24, 0x49, 0x24, 0xa4, 0xa4,

    /* U+002A "*" */
    0x25, 0x5c, 0xa0,

    /* U+002B "+" */
    0x10, 0x20, 0x40, 0x8f, 0xe2, 0x4, 0x8,

    /* U+002C "," */
    0xd8,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x4, 0x10, 0x82, 0x8, 0x41, 0x4, 0x20, 0x82,
    0x10, 0x41, 0x8, 0x20,

    /* U+0030 "0" */
    0x38, 0x8a, 0x14, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x9, 0x11, 0xc0,

    /* U+0031 "1" */
    0x61, 0x8, 0x42, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+0032 "2" */
    0x7a, 0x20, 0x41, 0x4, 0x20, 0x84, 0x20, 0x84,
    0x3f,

    /* U+0033 "3" */
    0x3c, 0x84, 0x8, 0x10, 0x43, 0x1, 0x1, 0x2,
    0x6, 0x13, 0xc0,

    /* U+0034 "4" */
    0x4, 0xc, 0xc, 0x14, 0x24, 0x24, 0x44, 0x84,
    0xff, 0x4, 0x4, 0x4,

    /* U+0035 "5" */
    0x7c, 0x81, 0x4, 0xb, 0x88, 0x80, 0x81, 0x2,
    0x6, 0x13, 0xc0,

    /* U+0036 "6" */
    0x3c, 0x82, 0x4, 0x9, 0x9c, 0xa0, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0037 "7" */
    0xfc, 0x20, 0x84, 0x10, 0x42, 0x8, 0x20, 0x82,
    0x8,

    /* U+0038 "8" */
    0x3c, 0x89, 0xa, 0x14, 0x46, 0x9b, 0x42, 0x83,
    0x6, 0x13, 0xc0,

    /* U+0039 "9" */
    0x39, 0x8a, 0x14, 0x18, 0x30, 0xde, 0x81, 0x4,
    0x8, 0x13, 0xc0,

    /* U+003A ":" */
    0x80, 0x80,

    /* U+003B ";" */
    0x40, 0x0, 0x58,

    /* U+003C "<" */
    0x6, 0x73, 0x2, 0x3, 0x1, 0xc0,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0xc0, 0x70, 0x18, 0x21, 0x9c, 0x0,

    /* U+003F "?" */
    0x74, 0x42, 0x11, 0x8, 0x84, 0x20, 0x0, 0x40,

    /* U+0040 "@" */
    0x7, 0x80, 0xc3, 0x8, 0x4, 0x80, 0x24, 0x38,
    0xc4, 0x46, 0x42, 0x32, 0x21, 0x91, 0x14, 0x88,
    0xa3, 0xb9, 0x80, 0x4, 0x0, 0x10, 0x0, 0x5e,
    0x0,

    /* U+0041 "A" */
    0x8, 0xc, 0x5, 0x2, 0x82, 0x41, 0x10, 0x88,
    0x7c, 0x41, 0x20, 0xa0, 0x50, 0x10,

    /* U+0042 "B" */
    0xf8, 0x84, 0x82, 0x82, 0x84, 0xf8, 0x86, 0x81,
    0x81, 0x81, 0x82, 0xfc,

    /* U+0043 "C" */
    0x1e, 0x61, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x40, 0x41, 0x3e,

    /* U+0044 "D" */
    0xf8, 0x84, 0x82, 0x82, 0x81, 0x81, 0x81, 0x81,
    0x82, 0x82, 0x84, 0xf8,

    /* U+0045 "E" */
    0xfd, 0x2, 0x4, 0x8, 0x1f, 0xa0, 0x40, 0x81,
    0x2, 0x7, 0xf0,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0x83, 0xf8, 0x20, 0x82, 0x8,
    0x20,

    /* U+0047 "G" */
    0x1f, 0x10, 0x50, 0x10, 0x8, 0x4, 0x2, 0x1f,
    0x1, 0x80, 0xa0, 0x48, 0x23, 0xe0,

    /* U+0048 "H" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81,
    0x81, 0x81, 0x81, 0x81,

    /* U+0049 "I" */
    0xff, 0xf0,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x4, 0x10, 0x41, 0x4, 0x18,
    0x9e,

    /* U+004B "K" */
    0x82, 0x84, 0x88, 0x90, 0x90, 0xb0, 0xc8, 0x88,
    0x84, 0x82, 0x82, 0x81,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x3f,

    /* U+004D "M" */
    0x80, 0xe0, 0xf0, 0x78, 0x5a, 0x2d, 0x16, 0x93,
    0x29, 0x94, 0xcc, 0x62, 0x30, 0x10,

    /* U+004E "N" */
    0x81, 0xc1, 0xc1, 0xa1, 0xa1, 0x91, 0x89, 0x89,
    0x85, 0x85, 0x83, 0x81,

    /* U+004F "O" */
    0x1e, 0x8, 0x44, 0xa, 0x2, 0x80, 0x60, 0x18,
    0x6, 0x1, 0x80, 0x90, 0x22, 0x10, 0x78,

    /* U+0050 "P" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xbe, 0x40, 0x81,
    0x2, 0x4, 0x0,

    /* U+0051 "Q" */
    0x1e, 0x8, 0x44, 0xa, 0x2, 0x80, 0x60, 0x18,
    0x6, 0x1, 0x80, 0x90, 0x24, 0x10, 0xf8, 0x4,
    0x1, 0x0, 0x3c,

    /* U+0052 "R" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xbe, 0x48, 0x89,
    0xa, 0x14, 0x10,

    /* U+0053 "S" */
    0x79, 0xa, 0x4, 0x4, 0x6, 0x3, 0x2, 0x2,
    0x6, 0x13, 0xc0,

    /* U+0054 "T" */
    0xff, 0x84, 0x2, 0x1, 0x0, 0x80, 0x40, 0x20,
    0x10, 0x8, 0x4, 0x2, 0x1, 0x0,

    /* U+0055 "U" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+0056 "V" */
    0x81, 0x81, 0x41, 0x42, 0x42, 0x42, 0x24, 0x24,
    0x24, 0x18, 0x18, 0x8,

    /* U+0057 "W" */
    0x84, 0x18, 0x41, 0x8a, 0x28, 0xa2, 0x8a, 0x28,
    0xa2, 0x51, 0x45, 0x14, 0x51, 0x46, 0x14, 0x60,
    0xc2, 0x8,

    /* U+0058 "X" */
    0x82, 0x42, 0x24, 0x24, 0x18, 0x8, 0x18, 0x28,
    0x24, 0x42, 0x42, 0x81,

    /* U+0059 "Y" */
    0x83, 0x5, 0x12, 0x24, 0x85, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+005A "Z" */
    0x7e, 0x2, 0x4, 0x4, 0x8, 0x10, 0x10, 0x20,
    0x20, 0x40, 0x80, 0xff,

    /* U+005B "[" */
    0xd2, 0x49, 0x24, 0x92, 0x49, 0x27,

    /* U+005C "\\" */
    0x82, 0x8, 0x10, 0x40, 0x82, 0x8, 0x10, 0x41,
    0x2, 0x8, 0x20, 0x41,

    /* U+005D "]" */
    0x64, 0x92, 0x49, 0x24, 0x92, 0x4f,

    /* U+005E "^" */
    0x10, 0xc5, 0x12, 0x4a, 0x18, 0x40,

    /* U+005F "_" */
    0xff, 0x80,

    /* U+0060 "`" */
    0x11, 0x0,

    /* U+0061 "a" */
    0x3c, 0x88, 0x8, 0x13, 0xf8, 0x60, 0xc1, 0x7e,

    /* U+0062 "b" */
    0x81, 0x2, 0x4, 0x8, 0x17, 0x31, 0x41, 0x83,
    0x6, 0xc, 0x28, 0x5f, 0x0,

    /* U+0063 "c" */
    0x3c, 0x82, 0x4, 0x8, 0x10, 0x20, 0x20, 0x3c,

    /* U+0064 "d" */
    0x2, 0x4, 0x8, 0x10, 0x27, 0x51, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x27, 0xc0,

    /* U+0065 "e" */
    0x38, 0x8a, 0xc, 0x1f, 0xf0, 0x20, 0x20, 0x3c,

    /* U+0066 "f" */
    0x34, 0x44, 0x4f, 0x44, 0x44, 0x44, 0x44,

    /* U+0067 "g" */
    0x3f, 0xc2, 0x82, 0x82, 0x44, 0x78, 0x40, 0x40,
    0x3c, 0x43, 0x81, 0x82, 0x7c,

    /* U+0068 "h" */
    0x82, 0x8, 0x20, 0xbb, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x84,

    /* U+0069 "i" */
    0xc2, 0xaa, 0xaa,

    /* U+006A "j" */
    0x20, 0x2, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2c,

    /* U+006B "k" */
    0x82, 0x8, 0x20, 0x8a, 0x29, 0x28, 0xd3, 0x48,
    0xa2, 0x84,

    /* U+006C "l" */
    0xaa, 0xaa, 0xaa, 0xa0,

    /* U+006D "m" */
    0xb9, 0xd8, 0xc6, 0x10, 0xc2, 0x18, 0x43, 0x8,
    0x61, 0xc, 0x21, 0x84, 0x20,

    /* U+006E "n" */
    0xbb, 0x18, 0x61, 0x86, 0x18, 0x61, 0x84,

    /* U+006F "o" */
    0x3c, 0x42, 0x82, 0x81, 0x81, 0x81, 0x82, 0x42,
    0x3c,

    /* U+0070 "p" */
    0xb9, 0x8a, 0xc, 0x18, 0x30, 0x61, 0x42, 0xf9,
    0x2, 0x4, 0x8, 0x0,

    /* U+0071 "q" */
    0x3a, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xa1, 0x3e,
    0x4, 0x8, 0x10, 0x20,

    /* U+0072 "r" */
    0xbc, 0x88, 0x88, 0x88, 0x80,

    /* U+0073 "s" */
    0x7a, 0x8, 0x10, 0x30, 0x20, 0x41, 0xf8,

    /* U+0074 "t" */
    0x42, 0x11, 0xe4, 0x21, 0x8, 0x42, 0x10, 0x60,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x63, 0x74,

    /* U+0076 "v" */
    0x83, 0x5, 0x12, 0x24, 0x45, 0xa, 0x14, 0x10,

    /* U+0077 "w" */
    0x84, 0x30, 0xc5, 0x28, 0xa5, 0x14, 0x94, 0x52,
    0x8c, 0x51, 0x8c, 0x10, 0x80,

    /* U+0078 "x" */
    0x84, 0x88, 0xa1, 0x81, 0x5, 0x12, 0x22, 0x84,

    /* U+0079 "y" */
    0x83, 0x5, 0x12, 0x24, 0x45, 0xa, 0xc, 0x10,
    0x20, 0x41, 0xe, 0x0,

    /* U+007A "z" */
    0x7c, 0x20, 0x84, 0x20, 0x84, 0x20, 0xfc,

    /* U+007B "{" */
    0x24, 0x44, 0x44, 0x48, 0x44, 0x44, 0x44, 0x43,

    /* U+007C "|" */
    0xff, 0xff, 0xe0,

    /* U+007D "}" */
    0x42, 0x22, 0x22, 0x21, 0x22, 0x22, 0x22, 0x28,

    /* U+007E "~" */
    0x41, 0x68, 0x30,

    /* U+4EAE "亮" */
    0x2, 0x0, 0x4, 0xf, 0xff, 0xc0, 0x0, 0x3f,
    0xf0, 0x80, 0x43, 0xff, 0x0, 0x0, 0xff, 0xfe,
    0x0, 0x18, 0xfc, 0x42, 0x10, 0x10, 0x40, 0x41,
    0x16, 0x7, 0xc0,

    /* U+5149 "光" */
    0x1, 0x0, 0x2, 0x10, 0x84, 0x40, 0x88, 0x81,
    0x10, 0x0, 0x20, 0x0, 0x40, 0x7f, 0xff, 0x8,
    0x40, 0x10, 0x80, 0x21, 0x0, 0x82, 0x1, 0x4,
    0x24, 0x8, 0x70, 0x1f, 0x80,

    /* U+5173 "关" */
    0x0, 0x0, 0x40, 0x80, 0x84, 0x0, 0x10, 0x7f,
    0xf8, 0x8, 0x0, 0x20, 0x0, 0x80, 0xff, 0xfc,
    0xc, 0x0, 0x50, 0x1, 0x20, 0x8, 0x40, 0x40,
    0x8e, 0x1, 0xc0,

    /* U+524D "前" */
    0x0, 0x0, 0x40, 0x80, 0x84, 0x3f, 0xff, 0x0,
    0x0, 0x0, 0x27, 0x80, 0x91, 0x22, 0x7c, 0x89,
    0x12, 0x24, 0x48, 0x9f, 0x2, 0x44, 0x9, 0x10,
    0x24, 0xc3, 0x80,

    /* U+52A8 "动" */
    0x0, 0x21, 0xf8, 0x80, 0x2, 0x0, 0x8, 0x0,
    0xff, 0xf8, 0x92, 0x2, 0x48, 0x11, 0x44, 0x45,
    0x11, 0x14, 0x68, 0x7e, 0xa1, 0x1, 0x4, 0x4,
    0xe0,

    /* U+5438 "吸" */
    0xf7, 0xe2, 0x48, 0x89, 0x22, 0x24, 0x88, 0x92,
    0x3a, 0x54, 0x29, 0x50, 0xa5, 0x24, 0xfc, 0x92,
    0x21, 0x88, 0x86, 0x4, 0x24, 0x23, 0xc,

    /* U+547C "呼" */
    0x0, 0xd, 0xef, 0xe2, 0x41, 0x4, 0x82, 0x49,
    0x24, 0x92, 0x49, 0x24, 0x10, 0x48, 0x20, 0x97,
    0xfd, 0xe0, 0x82, 0x1, 0x4, 0x2, 0x0, 0x4,
    0x0, 0x78, 0x0,

    /* U+5DF2 "已" */
    0xff, 0xf0, 0x0, 0x44, 0x1, 0x10, 0x4, 0x40,
    0x11, 0xff, 0xc4, 0x1, 0x10, 0x0, 0x40, 0x1,
    0x0, 0x14, 0x0, 0x50, 0x1, 0x7f, 0xf8,

    /* U+5EA6 "度" */
    0x0, 0x0, 0x1, 0x0, 0xff, 0xf9, 0x10, 0x82,
    0x21, 0x7, 0xff, 0xc8, 0x84, 0x11, 0x8, 0x23,
    0xf0, 0x40, 0x0, 0xff, 0xe3, 0x0, 0x44, 0x21,
    0x8, 0x3c, 0x10, 0x48, 0x4b, 0xf,

    /* U+5F00 "开" */
    0x7f, 0xfc, 0x10, 0x40, 0x20, 0x80, 0x41, 0x0,
    0x82, 0x1f, 0xff, 0xc2, 0x8, 0x4, 0x10, 0x10,
    0x20, 0x20, 0x40, 0x80, 0x81, 0x1, 0xc, 0x2,
    0x0,

    /* U+5F0F "式" */
    0x0, 0x80, 0x2, 0x60, 0x8, 0x3f, 0xff, 0x0,
    0x80, 0x2, 0xf, 0xf8, 0x4, 0x10, 0x10, 0x40,
    0x41, 0x1, 0x4, 0x1, 0x89, 0x78, 0x14, 0x0,
    0x30,

    /* U+5F53 "当" */
    0x2, 0x0, 0x21, 0x42, 0x12, 0x22, 0x2, 0x40,
    0x20, 0xff, 0xf0, 0x1, 0x0, 0x10, 0x1, 0x7f,
    0xf0, 0x1, 0x0, 0x10, 0x1, 0xff, 0xf0,

    /* U+5F69 "彩" */
    0x3, 0x82, 0x3c, 0x4, 0x0, 0x98, 0x24, 0xa0,
    0x21, 0x0, 0x4, 0x2, 0x4, 0x4, 0x7f, 0xc8,
    0xe, 0x30, 0x15, 0x1, 0x24, 0x82, 0x44, 0x4,
    0x4, 0x8, 0x4, 0x30,

    /* U+5F8B "律" */
    0x0, 0x40, 0x40, 0x81, 0x9f, 0xf4, 0x2, 0x20,
    0x4, 0x42, 0xff, 0xcc, 0x11, 0x2b, 0xfe, 0x90,
    0x40, 0x2f, 0xf8, 0x41, 0x0, 0x82, 0x1, 0xff,
    0xe2, 0x8, 0x4, 0x10, 0x0,

    /* U+63A5 "接" */
    0x20, 0x40, 0x40, 0x0, 0x87, 0xe7, 0xc8, 0x82,
    0x9, 0x4, 0xff, 0xc8, 0x0, 0x14, 0x40, 0x77,
    0xff, 0x42, 0x10, 0x8a, 0x41, 0x3, 0x2, 0x1d,
    0x9c, 0x40, 0x80,

    /* U+65AD "断" */
    0x88, 0x7a, 0xae, 0xa, 0x90, 0x22, 0x40, 0xff,
    0x2, 0x27, 0xf9, 0x91, 0x2b, 0x44, 0xab, 0x13,
    0x24, 0x48, 0x91, 0x20, 0x44, 0xff, 0x10, 0x0,
    0x40,

    /* U+65F6 "时" */
    0x0, 0x13, 0xe0, 0x48, 0x81, 0x23, 0xff, 0x88,
    0x12, 0x28, 0x4f, 0x91, 0x22, 0x44, 0x88, 0x92,
    0x20, 0x4f, 0x81, 0x20, 0x4, 0x0, 0x10, 0x3,
    0xc0,

    /* U+661F "星" */
    0x3f, 0xf8, 0x40, 0x10, 0x80, 0x21, 0xff, 0xc2,
    0x0, 0x87, 0xff, 0x4, 0x40, 0x10, 0x80, 0x3f,
    0xfd, 0x82, 0x0, 0xff, 0xe0, 0x8, 0x0, 0x10,
    0x1f, 0xff, 0xc0,

    /* U+6A21 "模" */
    0x21, 0x0, 0x84, 0x82, 0x7f, 0xc8, 0x48, 0xf9,
    0x0, 0x9b, 0xe3, 0x7f, 0x9d, 0x2, 0x6c, 0xa,
    0x9f, 0xea, 0x8, 0x2b, 0xff, 0x21, 0x40, 0x88,
    0x82, 0xc1, 0x80,

    /* U+6D41 "流" */
    0x40, 0x80, 0x82, 0x0, 0xff, 0xc0, 0x80, 0x4,
    0x11, 0x1f, 0xe0, 0x0, 0x41, 0x24, 0x14, 0x90,
    0x92, 0x42, 0x49, 0x11, 0x24, 0x48, 0x96, 0x40,
    0x70,

    /* U+706B "火" */
    0x1, 0x0, 0x2, 0x0, 0x4, 0x0, 0x8, 0x22,
    0x10, 0x84, 0x21, 0x10, 0x44, 0x21, 0x88, 0x2,
    0x80, 0x5, 0x0, 0x11, 0x0, 0x41, 0x1, 0x1,
    0xc, 0x1, 0xc0,

    /* U+7130 "焰" */
    0x10, 0x0, 0x20, 0x80, 0x41, 0xf0, 0x84, 0x45,
    0x50, 0x8b, 0x41, 0x24, 0x8, 0x8, 0xef, 0x11,
    0x2, 0x22, 0x4, 0x47, 0xb9, 0x48, 0x12, 0x50,
    0x28, 0x3f, 0xe0, 0x40, 0x80,

    /* U+7259 "牙" */
    0x7f, 0xfc, 0x0, 0x40, 0x0, 0x81, 0x1, 0x2,
    0x2, 0x7, 0xff, 0xc0, 0x28, 0x0, 0x90, 0x2,
    0x20, 0x8, 0x40, 0x60, 0x81, 0x1, 0x4, 0x1e,
    0x0,

    /* U+767D "白" */
    0x4, 0x1, 0x0, 0x20, 0x7f, 0xf8, 0x3, 0x0,
    0x60, 0xc, 0x1, 0xff, 0xf0, 0x6, 0x0, 0xc0,
    0x18, 0x3, 0xff, 0xe0, 0x8,

    /* U+7EA2 "红" */
    0x0, 0x0, 0x20, 0x0, 0x8f, 0xf9, 0x1, 0x4,
    0x42, 0x11, 0x4, 0x3c, 0x8, 0x8, 0x10, 0x20,
    0x20, 0xb8, 0x42, 0x80, 0x80, 0x1, 0x0, 0xc2,
    0x1e, 0x7f, 0xc0,

    /* U+7ECF "经" */
    0x10, 0x0, 0x8f, 0xe2, 0x1, 0x10, 0x4, 0x48,
    0x62, 0x42, 0x8f, 0x31, 0x88, 0x1, 0x40, 0x2,
    0xf, 0xf3, 0x84, 0x10, 0x10, 0x8, 0x43, 0xc1,
    0x0, 0x7f, 0xc0,

    /* U+7EDC "络" */
    0x0, 0x0, 0x41, 0x0, 0x83, 0xfa, 0x8, 0x24,
    0xa8, 0x92, 0x8a, 0x3c, 0x8, 0x10, 0x2c, 0x41,
    0x86, 0xa3, 0xfa, 0x84, 0x10, 0x8, 0x23, 0x10,
    0x48, 0x3f, 0x80, 0x41, 0x0,

    /* U+7EFF "绿" */
    0x10, 0x0, 0x43, 0xf8, 0x80, 0x11, 0x0, 0x24,
    0x1f, 0xd1, 0x0, 0x9c, 0xff, 0x88, 0x10, 0x21,
    0x22, 0x91, 0x5a, 0xc0, 0x80, 0x7, 0xc3, 0x12,
    0x48, 0x44, 0x0, 0x38, 0x0,

    /* U+7F51 "网" */
    0xff, 0xfc, 0x0, 0x60, 0x3, 0x44, 0x5a, 0x62,
    0xca, 0xa6, 0x23, 0x31, 0x9, 0x8c, 0x4c, 0x86,
    0x68, 0x43, 0x44, 0x18, 0x0, 0xc0, 0x1c,

    /* U+8272 "色" */
    0x0, 0x0, 0x10, 0x0, 0x3f, 0x0, 0x84, 0x2,
    0x8, 0xf, 0xff, 0x28, 0x42, 0x10, 0x84, 0x21,
    0x8, 0x7f, 0xf0, 0x80, 0x21, 0x0, 0x2, 0x0,
    0x24, 0x0, 0x4f, 0xff, 0x0,

    /* U+84DD "蓝" */
    0x8, 0x43, 0xff, 0xf0, 0x84, 0x0, 0x20, 0x48,
    0xf9, 0x25, 0x4, 0x94, 0x12, 0x8, 0x0, 0x0,
    0xff, 0xc4, 0x49, 0x11, 0x24, 0x44, 0x93, 0xff,
    0xf0,

    /* U+8679 "虹" */
    0x10, 0x0, 0x20, 0x0, 0x47, 0xfb, 0xf1, 0x5,
    0x22, 0xa, 0x44, 0x14, 0x88, 0x29, 0x10, 0x7e,
    0x20, 0xa0, 0x40, 0x40, 0x80, 0xa1, 0x1, 0xe2,
    0xc, 0x7f, 0xc0,

    /* U+8FDE "连" */
    0x0, 0x1, 0x4, 0x2, 0xff, 0x80, 0x80, 0x2,
    0x40, 0x11, 0xe, 0x7f, 0x88, 0x10, 0x20, 0x40,
    0x81, 0x2, 0xff, 0xc8, 0x10, 0x60, 0x43, 0x61,
    0x8, 0xff, 0xc0,

    /* U+95ED "闭" */
    0x0, 0x2, 0x7f, 0xc8, 0x2, 0x0, 0x18, 0x8,
    0xc0, 0x46, 0x7f, 0xf0, 0x11, 0x81, 0x8c, 0x14,
    0x61, 0x23, 0x31, 0x1a, 0x8, 0xc3, 0xc6, 0x0,
    0x60,

    /* U+95F4 "间" */
    0x0, 0x2, 0x7f, 0xc0, 0x2, 0x0, 0x18, 0x0,
    0xcf, 0xc6, 0x42, 0x32, 0x11, 0x9f, 0x8c, 0x84,
    0x64, 0x23, 0x21, 0x19, 0xf8, 0xc0, 0x6, 0x0,
    0xe0,

    /* U+9EC4 "黄" */
    0x8, 0x20, 0x10, 0x41, 0xff, 0xf0, 0x41, 0x0,
    0x82, 0x1f, 0xff, 0xc0, 0x40, 0x1f, 0xfc, 0x21,
    0x8, 0x7f, 0xf0, 0x84, 0x21, 0xff, 0xc0, 0x0,
    0x1, 0x2, 0x1c, 0x3, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 56, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 70, .box_w = 1, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 96, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 5, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 133, .box_w = 6, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 28, .adv_w = 225, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 159, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 59, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 63, .adv_w = 77, .box_w = 3, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 69, .adv_w = 77, .box_w = 3, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 75, .adv_w = 109, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 78, .adv_w = 133, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 85, .adv_w = 59, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 86, .adv_w = 83, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 87, .adv_w = 59, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 101, .box_w = 6, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 100, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 133, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 133, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 133, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 133, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 59, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 59, .box_w = 2, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 209, .adv_w = 133, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 215, .adv_w = 133, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 220, .adv_w = 133, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 226, .adv_w = 111, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 227, .box_w = 13, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 259, .adv_w = 147, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 162, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 158, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 297, .adv_w = 169, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 144, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 133, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 169, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 179, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 66, .box_w = 1, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 129, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 366, .adv_w = 155, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 130, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 197, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 178, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 183, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 153, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 439, .adv_w = 183, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 458, .adv_w = 151, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 480, .adv_w = 147, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 178, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 506, .adv_w = 136, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 216, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 133, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 548, .adv_w = 123, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 152, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 77, .box_w = 3, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 577, .adv_w = 101, .box_w = 6, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 589, .adv_w = 77, .box_w = 3, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 595, .adv_w = 133, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 601, .adv_w = 141, .box_w = 9, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 603, .adv_w = 150, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 11},
    {.bitmap_index = 605, .adv_w = 137, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 152, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 626, .adv_w = 126, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 634, .adv_w = 152, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 647, .adv_w = 135, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 655, .adv_w = 71, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 662, .adv_w = 136, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 675, .adv_w = 147, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 63, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 688, .adv_w = 63, .box_w = 4, .box_h = 16, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 696, .adv_w = 128, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 706, .adv_w = 65, .box_w = 2, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 228, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 148, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 730, .adv_w = 150, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 739, .adv_w = 152, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 751, .adv_w = 152, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 763, .adv_w = 87, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 768, .adv_w = 113, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 86, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 783, .adv_w = 148, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 790, .adv_w = 119, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 798, .adv_w = 190, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 811, .adv_w = 111, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 819, .adv_w = 120, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 831, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 77, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 846, .adv_w = 62, .box_w = 1, .box_h = 19, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 849, .adv_w = 77, .box_w = 4, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 857, .adv_w = 133, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 860, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 887, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 916, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 943, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 970, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 995, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1018, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1045, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1068, .adv_w = 256, .box_w = 15, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1098, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1123, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1148, .adv_w = 256, .box_w = 12, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1171, .adv_w = 256, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1199, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1228, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1255, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1280, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1305, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1332, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1359, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1384, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1411, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1440, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1465, .adv_w = 256, .box_w = 11, .box_h = 15, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1486, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1513, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1540, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1569, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1598, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1621, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1650, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1675, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1702, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1729, .adv_w = 256, .box_w = 13, .box_h = 15, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1754, .adv_w = 256, .box_w = 13, .box_h = 15, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1779, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x29b, 0x2c5, 0x39f, 0x3fa, 0x58a, 0x5ce, 0xf44,
    0xff8, 0x1052, 0x1061, 0x10a5, 0x10bb, 0x10dd, 0x14f7, 0x16ff,
    0x1748, 0x1771, 0x1b73, 0x1e93, 0x21bd, 0x2282, 0x23ab, 0x27cf,
    0x2ff4, 0x3021, 0x302e, 0x3051, 0x30a3, 0x33c4, 0x362f, 0x37cb,
    0x4130, 0x473f, 0x4746, 0x5016
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 20142, .range_length = 20503, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 36, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 3, 4, 3,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 6, 6, 0, 0, 0,
    0, 0, 7, 8, 9, 10, 11, 12,
    13, 0, 0, 14, 15, 16, 0, 0,
    10, 17, 10, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 2, 27, 0, 0,
    0, 0, 28, 29, 30, 0, 31, 32,
    33, 34, 0, 0, 35, 36, 34, 34,
    29, 29, 37, 38, 39, 40, 37, 41,
    42, 43, 44, 45, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 0, 0, 0,
    2, 0, 3, 4, 0, 5, 6, 5,
    7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 8, 8, 0, 0, 0,
    9, 0, 10, 0, 11, 0, 0, 0,
    11, 0, 0, 12, 0, 0, 0, 0,
    11, 0, 11, 0, 13, 14, 15, 16,
    17, 18, 19, 20, 0, 21, 3, 0,
    0, 0, 22, 0, 23, 23, 23, 24,
    25, 0, 26, 27, 0, 0, 28, 28,
    23, 28, 23, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, -28, 0, 0, 0,
    0, -12, 0, -23, -3, 0, 0, 0,
    0, -3, 0, 0, 0, 0, -11, 0,
    0, 0, 0, 0, -6, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 23, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -20, 0, -32, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -28, -4, -16,
    -8, 0, -22, 0, 0, 0, -3, 0,
    0, 0, 5, 0, 0, -9, 0, -4,
    -3, 0, -6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    -3, -11, 0, -5, -3, -6, -16, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, -3, -23, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, -12, 0,
    -3, 11, 11, 0, 0, 5, -6, 0,
    0, 0, 0, 0, 0, 0, 0, -18,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, -29, 0, 0, 0, 0,
    -5, -3, -3, 0, 0, -12, -5, -3,
    0, 3, -3, -3, -11, 8, 0, -3,
    0, 0, 0, 0, 8, -3, -3, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, -4,
    -3, -6, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -3, -3, -3, -3, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    0, 0, -6, -3, -6, -6, -3, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    -6, 0, 0, 0, 0, -6, -3, -3,
    -3, 0, -3, 0, 0, 0, 0, -12,
    0, 0, -6, 0, 0, -3, 0, -8,
    0, -6, 0, -3, -3, -3, -6, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, -6, 0, -3,
    0, -6, -3, 0, 0, 0, 0, 0,
    -16, 0, -24, 0, 0, -10, -3, -40,
    -4, 0, 0, 3, 3, -6, 4, -8,
    0, -8, -3, 0, -6, 0, 0, -6,
    -4, -3, -4, -3, -2, -5, -2, -4,
    0, 0, 0, -9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -6, 0, 0, 0, 0, 0, 0, -11,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -11, 0, -4, 0, 0,
    0, -3, -4, 0, 0, -3, -3, -2,
    -3, 0, -2, 0, 0, 0, 0, 0,
    0, 0, -3, 0, 0, -5, -5, -3,
    -2, -2, -3, -3, 0, -14, 0, -39,
    0, -17, 0, 0, -5, 3, -5, 0,
    -5, -34, -6, -21, -15, 0, -19, 0,
    -23, 0, -3, -3, 0, 0, 0, 0,
    0, -4, -3, -6, -8, 0, -6, 0,
    0, 0, 0, 0, -27, -11, -27, 0,
    0, -14, 0, -42, -3, -9, 0, 0,
    0, -6, -3, -28, 0, -14, -7, 0,
    -12, 0, 0, 0, -3, 0, 0, 0,
    0, -3, 0, -6, 0, 0, 0, -3,
    0, -9, 0, 0, 0, 0, 0, -6,
    -4, -3, 0, 5, 3, 0, 3, -3,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 3, 0, 0,
    0, 0, 0, -3, -3, -6, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0,
    -28, -23, -30, -6, 0, -11, -6, -37,
    -14, 0, 0, 0, 0, -6, -3, -17,
    0, -23, -16, -6, -23, 0, 0, -14,
    -16, -6, -14, -11, -12, -14, -11, -24,
    0, 0, 0, 0, -2, 0, -12, 0,
    0, -6, 0, -11, -3, 0, 0, 0,
    0, -3, -3, 0, 0, 0, 0, 0,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -16, -5, -17, 0, 0, -3, -3, -23,
    -3, 0, -3, 3, 0, 0, 0, -5,
    0, -3, -3, 0, -6, 0, 0, -6,
    -2, 0, -8, -2, -2, -3, -2, -8,
    0, 0, 0, 0, -8, -3, -13, 0,
    0, 0, -3, -22, -3, 0, 0, 0,
    0, 0, 0, -3, 0, -6, 0, 0,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, -3, 0, -3,
    0, -6, 0, 0, 0, 3, -3, 3,
    -3, -6, -3, 0, 0, 0, 0, 0,
    0, -3, 0, -2, 0, 0, 0, 0,
    0, -2, -3, -2, -3, -3, -2, -3,
    0, 0, 0, 0, -21, -16, -23, -5,
    0, -3, -3, -28, -4, -3, -3, 0,
    0, 0, 0, -8, 0, -17, -9, 0,
    -17, 0, 0, -11, -9, -3, -8, -3,
    -6, -8, -3, -11, 0, 0, 0, 0,
    0, -12, 0, 0, 0, -3, -6, -11,
    -8, 0, -3, -3, -3, 0, -3, -6,
    0, -6, -8, -6, -5, 0, 0, 0,
    0, -3, -8, -6, -6, -6, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -28, -11, -17,
    -11, 0, -23, 0, 0, 0, 0, 0,
    11, 0, 23, 0, 0, 0, 0, -6,
    -3, 0, 6, 0, 0, 0, 0, -17,
    0, 0, 0, 0, 1, 0, 0, 0,
    0, -6, 0, -2, 0, 0, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -11, 0, -11, -3, 3, 0, 0,
    1, 0, 0, 0, 0, -16, 0, -5,
    0, 0, -11, 0, -5, -5, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -6, 0, -3, 0, 0, -6, 0,
    0, -3, -5, 0, -3, 0, 0, 0,
    0, -3, 0, 3, 3, 5, 3, 0,
    0, 0, 0, -12, 0, 3, 0, 0,
    0, 0, 0, -6, -6, -6, 0, -2,
    -3, 0, -5, 0, -3, -5, 0, 0,
    -3, 0, 0, 0, 0, -3, 0, 4,
    4, 0, 4, 2, 6, 11, 17, 0,
    -11, -3, -6, 0, 8, 0, 0, 0,
    0, 14, 0, 20, 14, 11, 20, 0,
    24, -6, -3, 0, -5, 0, -3, 0,
    0, 0, 0, 6, 0, 0, 0, -3,
    0, 0, 6, -11, 0, 0, 18, 0,
    -8, 0, 0, 0, 0, -5, 0, 0,
    0, 0, -3, 0, 0, -4, -3, 0,
    0, 0, 8, 0, 0, 0, 0, 0,
    0, 0, 7, -3, 0, 0, 0, -12,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, -3, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 6, -4, 0, 6,
    1, 0, 0, 0, 0, -6, 0, 0,
    0, 0, -2, 0, 0, -3, -3, 0,
    -3, 0, -3, 0, 0, -3, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -3,
    0, 0, -8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -9, -6, -12, 11, 0, -3, 0, -12,
    0, 0, 0, 0, 0, 0, 0, -3,
    6, -6, -3, 0, -3, 0, 0, 0,
    0, 0, 0, 11, 8, 0, 11, 0,
    0, 0, 0, -18, 0, 3, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -3,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, -5, 6, 6, -5, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -6, -3, 0, -3, 0, 0, 0,
    -3, -6, 0, 0, 0, -6, 0, 0,
    0, 0, 0, -3, -4, 0, -6, 0,
    0, -3, 0, -11, 0, -6, 0, -2,
    0, 0, -3, -3, 0, -6, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 0, 0, -3,
    -2, 0, -3, 0, 0, 0, 0, -8,
    0, -6, 0, -2, 0, -3, -6, 0,
    0, -3, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    -3, 0, 0, -7, 0, -3, 0, 0,
    0, 0, -3, 0, 0, -11, 0, -3,
    0, -3, -8, 0, 0, -3, -3, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0,
    -4, 0, -6, 0, 0, 0, 0, -11,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, -6, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, -3, 0, -6,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, 0, -11, 0, 0, -8, -3, 0,
    -3, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 45,
    .right_class_cnt     = 36,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Font1 = {
#else
lv_font_t ui_font_Font1 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_FONT1*/

