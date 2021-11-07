#include "I2CLEDs.h"

void set_leds(uint32_t event_count) {
    // TODO: Herausfinden, wie man Werte auf den Port-Expander PCA9539 schreibt
    // I2C.write(I2C.PCA9539_ADDR, ?)
}

const struct i2cleds I2CLEDs = {
        .set_leds = set_leds,
};
