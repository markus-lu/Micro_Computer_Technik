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

#include "types.h"
#include "State.h"
#include "RTC.h"

#define BUTTON_TOGGLE_TIME_DATE BUTTON_0
#define BUTTON_EDIT BUTTON_1
#define BUTTON_PREVIOUS BUTTON_2
#define BUTTON_NEXT BUTTON_3
#define BUTTON_DEC BUTTON_4
#define BUTTON_INC BUTTON_5
#define BUTTON_BRIGHTNESS_DOWN BUTTON_6
#define BUTTON_BRIGHTNESS_UP BUTTON_7



#define SELECTED_HOUR (1 << 0)
#define SELECTED_MINUTE (1 << 1)
#define SELECTED_SECOND (1 << 2)
#define SELECTED_DAY (1 << 3)
#define SELECTED_MONTH (1 << 4)
#define SELECTED_YEAR (1 << 5)
#define SELECTED_WEEKDAY (1 << 6)

#define SELECTED_TIME (SELECTED_HOUR | SELECTED_MINUTE | SELECTED_SECOND)
#define SELECTED_DATE (SELECTED_DAY | SELECTED_MONTH | SELECTED_YEAR | SELECTED_WEEKDAY)

void clock_init(struct State *state);

void clock_loop_once(struct State *state);

#endif
