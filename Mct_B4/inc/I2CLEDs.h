/*
 ===============================================================================
 Name        : I2CLEDs.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Ausgabe der Evenanzahl
 ===============================================================================
 */

#ifndef _I2CLEDS_H_
#define _I2CLEDS_H_

#include "types.h"
#include "I2C.h"

void i2cleds_init(void);

void i2cleds_set_leds(uint32_t event_count);

#endif
