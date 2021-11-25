#include "LEDKey.h"
#include "Serial.h"

static void init() {
    Serial.init();
}

static uint8_t get_buttons() {
    uint8_t output = 0;
    uint8_t data[4];
    Serial.read(READ_DATA, data, 4);
    for (int i = 0; i < 4; ++i) {
        output <<= 1;
        output |= (data[i] & 0x10) != 0;
        output <<= 1;
        output |= (data[i] & 0x01) != 0;
    }
    return output;
}

static void set_display_data(const uint8_t leds, const uint8_t *digits) {
    Serial.write_command(WRITE_DATA);
    uint8_t data[16];

    data[0] = digits[0];
    data[1] = (leds >> 7) & 0b1;
    data[2] = digits[1];
    data[3] = (leds >> 6) & 0b1;
    data[4] = digits[2];
    data[5] = (leds >> 5) & 0b1;
    data[6] = digits[3];
    data[7] = (leds >> 4) & 0b1;
    data[8] = digits[4];
    data[9] = (leds >> 3) & 0b1;
    data[10] = digits[5];
    data[11] = (leds >> 2) & 0b1;
    data[12] = digits[6];
    data[13] = (leds >> 1) & 0b1;
    data[14] = digits[7];
    data[15] = (leds >> 0) & 0b1;

    Serial.write(SET_ADDRESS, data, 16);
}

// uses only lower 3 bits
static void set_brightness(uint8_t brightness, bool display_on) {
    uint8_t command = DISPLAY_CONTROL | (brightness & MAX_BRIGHTNESS);
    if (display_on) {
        command |= DISPLAY_ON;
    }
    Serial.write_command(command);
}

static void deinit() {
    Serial.deinit();
}

const struct ledkey LEDKey = {
        .init = init,
        .get_buttons = get_buttons,
        .set_display_data = set_display_data,
        .set_brightness = set_brightness,
        .deinit = deinit,
};
