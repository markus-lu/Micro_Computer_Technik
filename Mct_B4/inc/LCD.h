/*
 ===============================================================================
 Name        : LCD.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : LCD Ausgabe
 ===============================================================================
 */

#ifndef _LCD_H_
#define _LCD_H_

#include "lcdlib_1769.h"

struct lcd {
    const uint8_t LINES;
    void (*init)(void);
    void (*clear_screen)(void);
    void (*write_char) (uint8_t ch);
    void (*write_string) (char *st);
    void (*write_4lines) (char *line1 ,char *line2 ,char *line3 ,char *line4);
    void (*write_string_xy) (char *st, uint8_t x, uint8_t y);
    void (*write_uint) (uint16_t num, uint8_t len);
    void (*write_int) (int16_t num, uint8_t len);
    void (*go_to_xy) (uint8_t x, uint8_t y);
    void (*cursor) (uint8_t cursor_mode);
    void (*backlight)(uint8_t brightness);
    void (*write_symbols)(uint8_t ram_address, uint8_t state);
};

extern const struct lcd LCD;

#endif
