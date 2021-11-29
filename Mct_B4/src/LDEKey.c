#include "LEDKey.h"
#include "Serial.h"

void ledkey_init() {
	// Initialisierung Serielles Protokoll
    serial_init();
}

uint8_t ledkey_get_buttons() {
    uint8_t output = 0;
    uint8_t data[4];
    serial_read(READ_DATA_COMMAND, data, 4);
    for (int i = 0; i < 4; ++i) {
        output <<= 1;
        output |= (data[i] & 0x10) != 0;
        output <<= 1;
        output |= (data[i] & 0x01) != 0;
    }
    return output;
}

void ledkey_set_display_data(uint8_t leds, const uint8_t *digits) {
    serial_write_command(WRITE_DATA_COMMAND);
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

    serial_write(SET_ADDRESS_COMMAND, data, 16);
}

// uses only lower 3 bits
void ledkey_set_brightness(uint8_t brightness, bool display_on) {
	// Befehlsbyte zusammenbauen für Helligkeitssteuerung
    uint8_t command = DISPLAY_CONTROL_COMMAND | (brightness & MAX_BRIGHTNESS);
    if (display_on) {
        command |= DISPLAY_ON;
    }
    // Befehl senden
    serial_write_command(command);
}

void ledkey_deinit() {
    serial_deinit();
}
