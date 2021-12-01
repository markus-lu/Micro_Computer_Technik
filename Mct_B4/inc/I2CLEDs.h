/*
 ===============================================================================
 Name        : I2CLEDs.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Ausgabe der Evenanzahl über LEDs
 ===============================================================================
 */

#ifndef _I2CLEDS_H_
#define _I2CLEDS_H_

#include "types.h"
#include "I2C.h"

/**
 *  \file    I2CLEDs.h
*/


/**
 *  \brief Bitmuster zum einschalten einer LED
 */
#define LED_ON  0b0

/**
 *  \brief Bitmuster zum ausschalten einer LED
 */
#define LED_OFF 0b1

void i2cleds_init(void);

void i2cleds_set_leds(uint32_t event_count);

#endif
