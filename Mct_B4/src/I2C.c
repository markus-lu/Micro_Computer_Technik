#include "I2C.h"

void init(){
    i2c_init(I2C_SM);
}

void write(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    i2c_write_byte(address | I2C_WRITE);
    for (uint32_t i = 0; i < length; ++i) {
        // discard result, it's also not checked in the lcd library
        i2c_write_byte(data[i]);
    }
    i2c_stop();
}

void read(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    i2c_write_byte(address | I2C_READ);
    for (uint32_t i = 0; i < length; ++i) {
        // discard result, it's also not checked in the lcd library
        data[i] = i2c_read_byte(i < (length - 1) ? ACK : NACK);
    }
    i2c_stop();
}


const struct i2c I2C = {
        .init = init,
        .write =write,
        .read = read,
        .deinit = i2c_exit,
};