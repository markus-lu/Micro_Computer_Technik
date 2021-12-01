/*
 ===============================================================================
 Name        : Clock.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Logik für die Uhrzeit auf dem LEDKey und die Buttons
 ===============================================================================
 */
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "types.h"
#include "State.h"
#include "RTC.h"

/**
 *  \file    Clock.h
*/


/**
 * Bitmuster für den Taster zum Wechseln zwischen Datum und Uhrzeit
 */
#define BUTTON_TOGGLE_TIME_DATE BUTTON_0

/**
 * Bitmuster für den Taster zum Bearbeiten
 */
#define BUTTON_EDIT BUTTON_1

/**
 * Bitmuster für den Taster zum vorherigen Element wechseln
 */
#define BUTTON_PREVIOUS BUTTON_2

/**
 * Bitmuster für den Taster zum nächsten Element wechseln
 */
#define BUTTON_NEXT BUTTON_3

/**
 * Bitmuster für den Taster zum Hochzählen
 */
#define BUTTON_DEC BUTTON_4

/**
 * Bitmuster für den Taster zum Runterzälen
 */
#define BUTTON_INC BUTTON_5

/**
 * Bitmuster für den Taster zum Helligkeit runterregeln
 */
#define BUTTON_BRIGHTNESS_DOWN BUTTON_6

/**
 * Bitmuster für den Taster zum Helligkeit hochregeln
 */
#define BUTTON_BRIGHTNESS_UP BUTTON_7


/**
 * Bitmuster wenn Stunden ausgewählt sind
 */
#define SELECTED_HOUR (1 << 0)

/**
 * Bitmuster wenn Minuten ausgewählt sind
 */
#define SELECTED_MINUTE (1 << 1)

/**
 * Bitmuster wenn Sekunden ausgewählt sind
 */
#define SELECTED_SECOND (1 << 2)

/**
 * Bitmuster wenn Tage ausgewählt sind
 */
#define SELECTED_DAY (1 << 3)

/**
 * Bitmuster wenn Monate ausgewählt sind
 */
#define SELECTED_MONTH (1 << 4)

/**
 * Bitmuster wenn Jahre ausgewählt sind
 */
#define SELECTED_YEAR (1 << 5)

/**
 * Bitmuster wenn Wochentage ausgewählt sind
 */
#define SELECTED_WEEKDAY (1 << 6)

/**
 * Bitmuster wenn einer der Komponenten der Uhrzeit ausgewählt ist
 */
#define SELECTED_TIME (SELECTED_HOUR | SELECTED_MINUTE | SELECTED_SECOND)

/**
 * Bitmuster wenn einer der Komponenten des Datums ausgewählt ist
 */
#define SELECTED_DATE (SELECTED_DAY | SELECTED_MONTH | SELECTED_YEAR | SELECTED_WEEKDAY)

void clock_init(struct State *state);

void clock_loop_once(struct State *state);

#endif
