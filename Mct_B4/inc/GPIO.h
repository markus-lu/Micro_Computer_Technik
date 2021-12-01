/*
 ===============================================================================
 Name        : Clock.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Funktionen und Structe zur Interaktion mit GPIO
 ===============================================================================
 */

#ifndef _GPIO_H_
#define _GPIO_H_

/**
 *  \file     GPIO.h
*/

#include "types.h"

/**
 * PinMode Einstellung
 */
#define MODE_GPIO 0

/*********************************************************************/
/**

Dieser Enum beschreibt, ob ein GPIOPin als Ein- oder Ausgang verwendet werden soll.

\version 12.11.2021
**********************************************************************/
enum PinDir {
    INPUT = 0,
    OUTPUT = 1,
};

/*********************************************************************/
/**

Dieser Enum beschreibt, ob ein Pullup-Widerstand,
ein Pulldown-Widerstand oder die Repeater-Funktion aktiviert werden soll.

\version 12.11.2021
**********************************************************************/
enum PinMode {
    PULL_UP = 0b00,
    REPEATER = 0b01,
    NONE = 0b10,
    PULL_DOWN = 0b11,
};

/*********************************************************************/
/**

\brief Dieses Struct repräsentiert die Konfiguration eines GPIO Pins.

\version 12.11.2021
**********************************************************************/
struct GPIOPin {
    /**
     * Gibt an, an welchem Port dieser Pin anliegt.
     *
     * (0 - 4)
     */
    const uint8_t port;
    /**
     * Gibt an, welcher Pin an diesem Port konfiguriert werden soll.
     *
     * (0 - 31)
     */
    const uint8_t pin;
    /**
     * Gibt an, ob dieser Pin ein Ein- oder Ausgang ist.
     *
     * (0 - 1, siehe Enum PinDir )
     */
    const enum PinDir dir;
    /**
     * Gibt an, ob ein Pullup-Widerstand, ein Pulldown-Widerstand, oder die Repeater-Funktion aktiviert werden soll.
     *
     * (0 - 3, diese Enum PinMode )
     */
    const enum PinMode mode;
    /**
     * Gibt an, ob dieser Pin im OpenDrain-Modus arbeiten soll.
     *
     * (0 - 1)
     */
    const bool open_drain;
};

void gpio_init_pin(const struct GPIOPin *pin);

void gpio_set_high(const struct GPIOPin *pin);

void gpio_set_low(const struct GPIOPin *pin);

void gpio_set(const struct GPIOPin *pin, bool state);

bool gpio_get(const struct GPIOPin *pin);

#endif
