#include "RGBLED.h"

void init() {
    GPIO.init_pin(&RGBLED.red);
    GPIO.init_pin(&RGBLED.green);
}

void set_green() {
    GPIO.clear(&RGBLED.red);
    GPIO.set(&RGBLED.green);
}

void set_red() {
    GPIO.clear(&RGBLED.green);
    GPIO.set(&RGBLED.red);
}

const struct rgbled RGBLED = {
        .red = {
                .port = 0,
                .pin = 22,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .green = {
                .port = 3,
                .pin = 25,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .init = init,
        .set_green = set_green,
        .set_red = set_red,
};