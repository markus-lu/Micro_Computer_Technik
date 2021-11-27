/*
 ===============================================================================
 Name        : RGBLED.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : RGB LED Treiber
 ===============================================================================
 */

#ifndef _RGBLED_H_
#define _RGBLED_H_


#include "types.h"
#include "GPIO.h"

extern const struct GPIOPin rgbled_red;
extern const struct GPIOPin rgbled_green;
extern const struct GPIOPin rgbled_blue;

void rgbled_init(void);

void rgbled_set_green(void);

void rgbled_set_red(void);

void rgbled_set_blue(void);

void rgbled_off(void);

#endif
