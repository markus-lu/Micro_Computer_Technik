/*
 ===============================================================================
 Name        : Clock.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Logik für die Uhrzeit auf dem LEDKey und die Buttons darauf
 ===============================================================================
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include <LPC17xx.h>

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

struct gpio {
    void (*init_pin)(const struct GPIOPin *pin);
    void (*set_high)(const struct GPIOPin *pin);
    void (*set_low)(const struct GPIOPin *pin);
    void (*set)(const struct GPIOPin *pin, bool state);
    bool (*get)(const struct GPIOPin *pin);
};

extern const struct gpio GPIO;

#endif
