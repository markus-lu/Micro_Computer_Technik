#include "I2C.h"

static void init() {
    i2c_init(I2C.SPEED);
}

static uint8_t write(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    uint8_t status = i2c_write_byte(address | I2C_WRITE);
    if (status == ACK) {
        for (uint32_t i = 0; i < length; ++i) {
            // discard result, it's also not checked in the lcd library
            i2c_write_byte(data[i]);
        }
    }
    i2c_stop();
    return status;
}

static uint8_t read(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    uint8_t status = i2c_write_byte(address | I2C_READ);
    if (status == ACK) {
        for (uint32_t i = 0; i < length; ++i) {
            // discard result, it's also not checked in the lcd library
            data[i] = i2c_read_byte(i < (length - 1) ? ACK : NACK);
        }
    }
    i2c_stop();
    return status;
}


const struct i2c I2C = {
        .PCA9539_ADDRESS = 0xE8,
        .PCF8574A_ADDRESS = 0x7E,
        .PCF8574_LCD_ADDRESS = 0x4E,
        .DS3231_ADDRESS = 0x68,
        .SPEED = I2C_SM,
        .init = init,
        .write =write,
        .read = read,
        .deinit = i2c_exit,
};
