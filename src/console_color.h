#ifndef console_color_h
#define console_color_h
#include <stdio.h>

#define COLOR_BITS 5

typedef enum {
    FG_NONE         = 0  << 0,
    FG_BLACK        = 1  << 0,
    FG_DARK_RED     = 2  << 0,
    FG_DARK_GREEN   = 3  << 0,
    FG_DARK_YELLOW  = 4  << 0,
    FG_DARK_BLUE    = 5  << 0,
    FG_DARK_MAGENTA = 6  << 0,
    FG_DARK_CYAN    = 7  << 0,
    FG_GRAY         = 8  << 0,
    FG_DARK_GRAY    = 9  << 0,
    FG_RED          = 10 << 0,
    FG_GREEN        = 11 << 0,
    FG_YELLOW       = 12 << 0,
    FG_BLUE         = 13 << 0,
    FG_MAGENTA      = 14 << 0,
    FG_CYAN         = 15 << 0,
    FG_WHITE        = 16 << 0,

    BG_NONE         = 0  << COLOR_BITS,
    BG_BLACK        = 1  << COLOR_BITS,
    BG_DARK_RED     = 2  << COLOR_BITS,
    BG_DARK_GREEN   = 3  << COLOR_BITS,
    BG_DARK_YELLOW  = 4  << COLOR_BITS,
    BG_DARK_BLUE    = 5  << COLOR_BITS,
    BG_DARK_MAGENTA = 6  << COLOR_BITS,
    BG_DARK_CYAN    = 7  << COLOR_BITS,
    BG_GRAY         = 8  << COLOR_BITS,
    BG_DARK_GRAY    = 9  << COLOR_BITS,
    BG_RED          = 10 << COLOR_BITS,
    BG_GREEN        = 11 << COLOR_BITS,
    BG_YELLOW       = 12 << COLOR_BITS,
    BG_BLUE         = 13 << COLOR_BITS,
    BG_MAGENTA      = 14 << COLOR_BITS,
    BG_CYAN         = 15 << COLOR_BITS,
    BG_WHITE        = 16 << COLOR_BITS
} color;

#ifndef COMMON_LVB_LEADING_BYTE
#define COMMON_LVB_LEADING_BYTE    0x0100
#endif

#ifndef COMMON_LVB_TRAILING_BYTE
#define COMMON_LVB_TRAILING_BYTE   0x0200
#endif

#ifndef COMMON_LVB_GRID_HORIZONTAL
#define COMMON_LVB_GRID_HORIZONTAL 0x0400
#endif

#ifndef COMMON_LVB_GRID_LVERTICAL
#define COMMON_LVB_GRID_LVERTICAL  0x0800
#endif

#ifndef COMMON_LVB_GRID_RVERTICAL
#define COMMON_LVB_GRID_RVERTICAL  0x1000
#endif

#ifndef COMMON_LVB_REVERSE_VIDEO
#define COMMON_LVB_REVERSE_VIDEO   0x4000
#endif

#ifndef COMMON_LVB_UNDERSCORE
#define COMMON_LVB_UNDERSCORE      0x8000
#endif

/**
 * @param color {color} Console color. We can pass (FG | BG) as color.
 * @param stream {FILE*} `stdout` or `stderr`. Others will be passed to fprintf
 * without colors.
 * @param format {const char*} Format string fprintf will take.
 * @return {int} fprintf returned value.
 *
 * CAUTION(Yusuke Suzuki): bright FG & dark BG combination doesn't works
 * correctly on some terminals, but this is an well-known issue.
 */
int cc_fprintf(color color, FILE* stream, const char* format, ...);

#endif  /* console_color_h */
