/*
 ===============================================================================
 Name        : Clock.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Logik für die Uhrzeit auf dem LEDKey und die Buttons darauf
 ===============================================================================
 */
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "LEDKey.h"

struct clock {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct clock Clock;

#endif
