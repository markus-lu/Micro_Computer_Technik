/*
 ===============================================================================
 Name        : LEDKey.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen der Uhrzeit und Datum + Auslesen der Taster

 ===============================================================================
 */

#ifndef _LEDKEY_H_
#define _LEDKEY_H_

#include "types.h"
#include "GPIO.h"

// digits
#define SEG_0 0b00111111
#define SEG_1 0b00000110
#define SEG_2 0b01011011
#define SEG_3 0b01001111
#define SEG_4 0b01100110
#define SEG_5 0b01101101
#define SEG_6 0b01111101
#define SEG_7 0b00000111
#define SEG_8 0b01111111
#define SEG_9 0b01101111

#define SEG_DOT 0b10000000

// commands
#define WRITE_DATA      0b01000000
#define READ_DATA       0b01000010
#define SET_ADDRESS     0b11000000
#define DISPLAY_CONTROL 0b10000000
#define DISPLAY_ON      0b00001000
#define MAX_BRIGHTNESS  0b00000111

#define BUTTON_0 (1 << 6)
#define BUTTON_1 (1 << 4)
#define BUTTON_2 (1 << 2)
#define BUTTON_3 (1 << 0)
#define BUTTON_4 (1 << 7)
#define BUTTON_5 (1 << 5)
#define BUTTON_6 (1 << 3)
#define BUTTON_7 (1 << 1)

void ledkey_init(void);

uint8_t ledkey_get_buttons(void);

void ledkey_set_display_data(uint8_t leds, const uint8_t *digits);

void ledkey_set_brightness(uint8_t brightness, bool display_on);

void ledkey_deinit(void);

#endif
