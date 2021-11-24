#include <stdio.h>
#include "LEDKey.h"
#include "Serial.h"

static void init() {
    Serial.init();
}

static void announce_read() {
    uint8_t data[1];
    data[0] = READ_DATA;
    Serial.write(data, 1);
    Serial.wait_for_interrupt();
    Serial.wait_for_interrupt();
    Serial.wait_for_interrupt();
}

static uint8_t get_buttons() {
    announce_read();
    uint8_t data[4];
    Serial.read(data, 4);
    // TODO: Find out which button is where
    for (int i = 0; i < 4; ++i) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return 0;
}

static void announce_write() {
    uint8_t data[1];
    data[0] = WRITE_DATA;
    Serial.write(data, 1);
}

static void set_display_data(const uint8_t leds, const uint8_t *digits) {
    announce_write();
    uint8_t data[17];
    uint8_t segment_starting_address = 0;
    data[0] = SET_ADDRESS | segment_starting_address;

//    for (int i = 0; i < 17; ++i) {
//        if (i & 1) {
//            data[i] = digits[i >> 1];
//        } else {
//            int bit = 8 - (i >> 1);
//            data[i] = (leds >> bit) & 0b1;
//        }
//    }

    data[1] = digits[0];
    data[2] = (leds >> 7) & 0b1;
    data[3] = digits[1];
    data[4] = (leds >> 6) & 0b1;
    data[5] = digits[2];
    data[6] = (leds >> 5) & 0b1;
    data[7] = digits[3];
    data[8] = (leds >> 4) & 0b1;
    data[9] = digits[4];
    data[10] = (leds >> 3) & 0b1;
    data[11] = digits[5];
    data[12] = (leds >> 2) & 0b1;
    data[13] = digits[6];
    data[14] = (leds >> 1) & 0b1;
    data[15] = digits[7];
    data[16] = (leds >> 0) & 0b1;

    Serial.write(data, 17);
}

// uses only lower 3 bits
static void set_brightness(uint8_t brightness, bool display_on) {
    uint8_t data[1];
    data[0] = DISPLAY_CONTROL | (brightness & MAX_BRIGHTNESS);
    if (display_on) {
        data[0] |= DISPLAY_ON;
    }
    Serial.write(data, 1);
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
