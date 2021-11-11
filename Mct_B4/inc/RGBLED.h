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


#include "GPIO.h"

struct rgbled {
    const struct GPIOPin red;
    const struct GPIOPin green;
    const struct GPIOPin blue;
    void (*init)(void);
    void (*set_green)(void);
    void (*set_red)(void);
    void (*set_blue)(void);
    void (*off)(void);
};

extern const struct rgbled RGBLED;

#endif