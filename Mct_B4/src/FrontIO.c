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

static void init() {
    I2C.init();
}

static uint8_t get_buttons() {
    uint8_t data[1];
    I2C.read(I2C.PCF8574A_ADDRESS, data, 1);
    return data[0];
}

const struct frontio FrontIO = {
        .init = init,
        .get_buttons = get_buttons
};
