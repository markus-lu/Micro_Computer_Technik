#include "LCD.h"

const struct lcd LCD = {
       .init = lcd_init,
       .clear_screen = lcd_clrscr,
       .write_char = lcd_write_char,
       .write_string = lcd_write_string,
       .write_4lines = lcd_write_4lines,
       .write_string_xy = lcd_write_string_xy,
       .write_uint = lcd_write_uint,
       .write_int = lcd_write_int,
       .go_to_xy = lcd_gotoxy,
       .cursor = lcd_cursor,
       .backlight = lcd_bglight,
       .write_symbols = lcd_write_symbols,
};
