#include "Serial.h"

void init() {

}

void write(uint8_t address, uint8_t *data, uint32_t length) {

}

void read(uint8_t address, uint8_t *data, uint32_t length) {

}


void deinit() {

}

const struct serial Serial = {
        .init = init,
        .write = write,
        .read = read,
        .deinit = deinit,
};