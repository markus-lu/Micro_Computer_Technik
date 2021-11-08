#include "I2CLEDs.h"

#define OUTPUT_P0 0b010
#define OUTPUT_P1 0b011

static void set_leds(uint32_t event_count) {
	uint8_t leds [2];
	leds[0]= OUTPUT_P0;
	leds[1]= 0xFF;
	uint8_t status = I2C.write(I2C.PCA9539_ADDRESS, leds, 2);

	printf("OK: %d\n", status);

    // TODO: Herausfinden, wie man Werte auf den Port-Expander PCA9539 schreibt
    // I2C.write(I2C.PCA9539_ADDR, ?)
}

const struct i2cleds I2CLEDs = {
        .set_leds = set_leds,
};
