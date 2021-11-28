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

#include "types.h"

#define MODE_GPIO 0

enum PinDir {
    INPUT = 0,
    OUTPUT = 1,
};

enum PinMode {
    PULL_UP = 0b00,
    REPEATER = 0b01,
    NONE = 0b10,
    PULL_DOWN = 0b11,
};

struct GPIOPin {
    const uint8_t port;
    const uint8_t pin;
    const enum PinDir dir;
    const enum PinMode mode;
    const bool open_drain;
};

void gpio_init_pin(const struct GPIOPin *pin);

void gpio_set_high(const struct GPIOPin *pin);

void gpio_set_low(const struct GPIOPin *pin);

void gpio_set(const struct GPIOPin *pin, bool state);

bool gpio_get(const struct GPIOPin *pin);

#endif
