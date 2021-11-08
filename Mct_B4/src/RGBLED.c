#include "RGBLED.h"

static void init() {
    GPIO.init_pin(&RGBLED.red);
    GPIO.init_pin(&RGBLED.green);
    GPIO.init_pin(&RGBLED.blue);
}

static void set_green() {
    GPIO.set(&RGBLED.red);
    GPIO.set(&RGBLED.blue);
    GPIO.clear(&RGBLED.green);
}

static void set_red() {
    GPIO.set(&RGBLED.green);
    GPIO.set(&RGBLED.blue);
    GPIO.clear(&RGBLED.red);
}

static void set_blue() {
    GPIO.set(&RGBLED.green);
    GPIO.set(&RGBLED.red);
    GPIO.clear(&RGBLED.blue);
}

static void off() {
    GPIO.set(&RGBLED.green);
    GPIO.set(&RGBLED.red);
    GPIO.set(&RGBLED.blue);
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
        .blue = {
                .port = 3,
                .pin = 26,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .init = init,
        .set_green = set_green,
        .set_red = set_red,
		.set_blue = set_blue,
		.off = off,
};
