#include "RGBLED.h"

void rgbled_init() {
	// Initialisierung der GPIO PINS für RGB LED
    gpio_init_pin(&rgbled_red);
    gpio_init_pin(&rgbled_green);
    gpio_init_pin(&rgbled_blue);

    rgbled_set_red();
}

void rgbled_set_green() {
    gpio_set_high(&rgbled_red);
    gpio_set_high(&rgbled_blue);
    gpio_set_low(&rgbled_green);
}

void rgbled_set_red() {
	// Grün aus
    gpio_set_high(&rgbled_green);
    // Blau aus
    gpio_set_high(&rgbled_blue);
    // Rot an
    gpio_set_low(&rgbled_red);
}

void rgbled_set_blue() {
    gpio_set_high(&rgbled_green);
    gpio_set_high(&rgbled_red);
    gpio_set_low(&rgbled_blue);
}

void rgbled_off() {
    gpio_set_high(&rgbled_green);
    gpio_set_high(&rgbled_red);
    gpio_set_high(&rgbled_blue);
}

const struct GPIOPin rgbled_red = {
        .port = 0,
        .pin = 22,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};

const struct GPIOPin rgbled_green = {
        .port = 3,
        .pin = 25,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};

const struct GPIOPin rgbled_blue = {
        .port = 3,
        .pin = 26,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};
