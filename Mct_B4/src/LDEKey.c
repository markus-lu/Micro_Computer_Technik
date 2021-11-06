#include "LEDKey.h"

void init() {

}

void set_green() {

}

const struct ledkey LEDKey = {
        .init = init,
        .loop_once = set_green,
};
