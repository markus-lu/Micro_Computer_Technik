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

#include <types.h>
#include <stdbool.h>
#include "State.h"
#include "RTC.h"

struct clock {
    void (*init)(struct State *state);

    void (*loop_once)(struct State *state);
};

extern const struct clock Clock;

#endif
