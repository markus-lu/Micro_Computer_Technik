/*
 ===============================================================================
 Name        : I2CLEDs.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Ausgabe der Evenanzahl
 ===============================================================================
 */

#include "I2C.h"

struct i2cleds {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct i2cleds I2CLEDs;

