/*
 ===============================================================================
 Name        : LEDKey.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen der Uhrzeit und Datum/auslesen von taster ← Protokoll
			   selber schreiben

 ===============================================================================
 */

#ifndef _LEDKEY_H_
#define _LEDKEY_H_

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

// hex
#define SEG_A 0b01110111
#define SEG_B 0b00011111
#define SEG_C 0b01001110
#define SEG_D 0b00111101
#define SEG_E 0b01001111
#define SEG_F 0b01000111

// commands
#define WRITE_DATA      0b01000000
#define READ_DATA       0b01000010
#define SET_ADDRESS     0b11000000
#define DISPLAY_CONTROL 0b10000000
#define DISPLAY_ON      0b00001000
#define MAX_BRIGHTNESS  0b00000111

#include "GPIO.h"

struct ledkey {
    void (*init)(void);

    uint8_t (*get_buttons)(void);

    void (*set_display_data)(const uint8_t leds, const uint8_t *digits);

    void (*set_brightness)(uint8_t brightness, bool display_on);

    void (*deinit)(void);
};

extern const struct ledkey LEDKey;

#endif
