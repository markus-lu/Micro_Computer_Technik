#include "I2CLEDs.h"

void init() {

}

void set_green() {

}

const struct i2cleds I2CLEDs = {
        .init = init,
        .loop_once = set_green,
};
