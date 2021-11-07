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


#include "I2C.h"

struct i2cleds {
    void (*set_leds)(uint32_t event_count);
};

extern const struct i2cleds I2CLEDs;

#endif
