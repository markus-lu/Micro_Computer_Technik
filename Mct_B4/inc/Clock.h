/*
 ===============================================================================
 Name        : Clock.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Logik für die Uhrzeit auf dem LEDKey und die Buttons darauf
 ===============================================================================
 */

#include "LEDKey.h"

struct clock {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct clock Clock;
