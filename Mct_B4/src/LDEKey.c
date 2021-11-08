#include "LEDKey.h"

static void init() {

}

static void loop_once() {

}

const struct ledkey LEDKey = {
        .init = init,
        .loop_once = loop_once,
};
