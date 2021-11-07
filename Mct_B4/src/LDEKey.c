#include "LEDKey.h"

void init() {

}

void loop_once() {

}

const struct ledkey LEDKey = {
        .clk = {
                .port = 0,
                .pin = 10,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .dio = {
                .port = 0,
                .pin = 11,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .stb = {
                .port = 0,
                .pin = 5,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .init = init,
        .loop_once = loop_once,
};
