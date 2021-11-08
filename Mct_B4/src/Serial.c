#include "Serial.h"

static void init() {
    GPIO.init_pin(&Serial.clk);
    GPIO.init_pin(&Serial.dio);
    GPIO.init_pin(&Serial.stb);
}

static void write(uint8_t address, uint8_t *data, uint32_t length) {
    // TODO: Rausfinden, wie man Daten schreibt (Timer2?)
}

static void read(uint8_t address, uint8_t *data, uint32_t length) {
    // TODO: Rausfinden, wie man Daten liest (Timer2?)
}

static void deinit() {

}

const struct serial Serial = {
        .clk = {
                .port = 0,
                .pin = 10,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .dio = {
                .port = 0,
                .pin = 11,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .stb = {
                .port = 0,
                .pin = 5,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .init = init,
        .write = write,
        .read = read,
        .deinit = deinit,
};
