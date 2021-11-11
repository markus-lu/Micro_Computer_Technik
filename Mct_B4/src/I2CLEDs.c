#include "I2CLEDs.h"

#define OUTPUT_P0 0b010
#define OUTPUT_P1 0b011
#define Config_P0 0b110
#define Config_P1 0b111
#define Dir_IN 0b1
#define Dir_OUT 0b0

#define LED_ON  0b0
#define LED_OFF 0b1

static void init() {
	uint8_t configuration[2];
	configuration[0] = Config_P0;
	configuration[1] = Dir_OUT;
	I2C.write(I2C.PCA9539_ADDRESS, configuration, 2);

	configuration[0] = Config_P1;
	configuration[1] = Dir_OUT;
	I2C.write(I2C.PCA9539_ADDRESS, configuration, 2);
}

static void set_leds(uint32_t event_count) {
	uint16_t leds = 0;

	for (int i = 0; i < 16 - event_count; ++i) {
		leds |= (1 << i);
	}

	uint8_t cmd[2];
	cmd[0] = OUTPUT_P0;
	cmd[1] = leds;
	I2C.write(I2C.PCA9539_ADDRESS, cmd, 2);

	cmd[0] = OUTPUT_P1;
	cmd[1] = (leds >> 8);
	I2C.write(I2C.PCA9539_ADDRESS, cmd, 2);
}

const struct i2cleds I2CLEDs = { .init = init, .set_leds = set_leds, };