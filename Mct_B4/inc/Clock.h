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
#include "LEDKey.h"
#include "RTC.h"

/**
 *  \file    Clock.h
*/


/**
 *  \brief Bitmuster für den Taster zum Wechseln zwischen Datum und Uhrzeit
 */
#define BUTTON_TOGGLE_TIME_DATE BUTTON_0

/**
 *  \brief Bitmuster für den Taster zum Bearbeiten
 */
#define BUTTON_EDIT BUTTON_1

/**
 *  \brief Bitmuster für den Taster zum vorherigen Element wechseln
 */
#define BUTTON_PREVIOUS BUTTON_2

/**
 *  \brief Bitmuster für den Taster zum nächsten Element wechseln
 */
#define BUTTON_NEXT BUTTON_3

/**
 *  \brief Bitmuster für den Taster zum Hochzählen
 */
#define BUTTON_DEC BUTTON_4

/**
 *  \brief Bitmuster für den Taster zum Runterzälen
 */
#define BUTTON_INC BUTTON_5

/**
 *  \brief Bitmuster für den Taster zum Helligkeit runterregeln
 */
#define BUTTON_BRIGHTNESS_DOWN BUTTON_6

/**
 *  \brief Bitmuster für den Taster zum Helligkeit hochregeln
 */
#define BUTTON_BRIGHTNESS_UP BUTTON_7


/**
 *  \brief Bitmuster wenn Stunden ausgewählt sind
 */
#define SELECTED_HOUR (1 << 0)

/**
 *  \brief Bitmuster wenn Minuten ausgewählt sind
 */
#define SELECTED_MINUTE (1 << 1)

/**
 *  \brief Bitmuster wenn Sekunden ausgewählt sind
 */
#define SELECTED_SECOND (1 << 2)

/**
 *  \brief Bitmuster wenn Tage ausgewählt sind
 */
#define SELECTED_DAY (1 << 3)

/**
 *  \brief Bitmuster wenn Monate ausgewählt sind
 */
#define SELECTED_MONTH (1 << 4)

/**
 *  \brief Bitmuster wenn Jahre ausgewählt sind
 */
#define SELECTED_YEAR (1 << 5)

/**
 *  \brief Bitmuster wenn Wochentage ausgewählt sind
 */
#define SELECTED_WEEKDAY (1 << 6)

/**
 *  \brief Bitmuster wenn einer der Komponenten der Uhrzeit ausgewählt ist
 */
#define SELECTED_TIME (SELECTED_HOUR | SELECTED_MINUTE | SELECTED_SECOND)

/**
 *  \brief Bitmuster wenn einer der Komponenten des Datums ausgewählt ist
 */
#define SELECTED_DATE (SELECTED_DAY | SELECTED_MONTH | SELECTED_YEAR | SELECTED_WEEKDAY)

void clock_init(struct State *state);

void clock_loop_once(struct State *state);

#endif
