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

#include <stdint.h>
#include <stdbool.h>
#include "RTC.h"

struct ClockState {
    struct DateTime time;
    uint8_t sel_field;
    bool edit_mode;
};

struct clock {
    void (*init)(struct ClockState* state);
    void (*loop_once)(struct ClockState* state);
};

extern const struct clock Clock;

#endif
