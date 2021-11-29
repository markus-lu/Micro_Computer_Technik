#include "FrontIO.h"
#include "I2C.h"

#define LED0 (1 << 0)
#define LED1 (1 << 1)
#define LED2 (1 << 2)
#define LED3 (1 << 3)
#define TA0 (1 << 4)
#define TA1 (1 << 5)
#define TA2 (1 << 6)
#define TA3 (1 << 7)

void frontio_init() {
    i2c_init(I2C_SPEED);
}

// lower 4 bits for LEDs
void frontio_set_leds(uint8_t leds) {
    uint8_t data[1];
    data[0] = ~(leds & 0x0F);
    i2c_write(PCF8574A_ADDRESS, data, 1);
}

// lower 4 bits for buttons
uint8_t frontio_get_buttons() {
    uint8_t data[1];
    i2c_read(PCF8574A_ADDRESS, data, 1);
    return ((~data[0]) >> 4) & 0x0F;
}
